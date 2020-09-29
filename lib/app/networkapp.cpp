#include "networkapp.h"
#include "../../config.h"

void NetworkApp::setup()
{
  mqttClient.setServer(MQTT_SERVER, 1883);
  config.autoReconnect = true;
  config.principle = AC_PRINCIPLE_RSSI;
  config.portalTimeout = 30000;
  //config.apid = "ESP-" + String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  portal.config(config);
  if (portal.begin()) {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
    String localIp = WiFi.localIP().toString();
    localIp.replace(".", "_");
  
    Serial.println(localIp);
    myMqttWrapper.setTopic(localIp);
    myMqttWrapper.setInTopic(localIp + "/cmd");
    myMqttWrapper.setClientName(localIp);

    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  } else {
    Serial.println("offline mode, reboot in a few minutes");
  }
  myMqttWrapper.setListener(this);
  myMqttWrapper.setup();
  timeClient.begin(); 
}

void NetworkApp::trackDisconnect(int code)
{
  if (disconnectTime == 0) {
    Serial.println("Not connected to WiFi: " + String(code));
    disconnectTime = millis();
  }
}

void NetworkApp::rebootWatchdog() 
{
  // do we really want this?
  if (disconnectTime!=0 && millis() > disconnectTime + rebootTime) {
    Serial.println("WiFi disconnected, auto-reboot");
    disconnectTime = 0;
    delay(1000);
    ESP.restart();
  }
}

bool NetworkApp::loop()
{
  rebootWatchdog();

  server.handleClient();
  portal.handleClient();

  //TODO: we're getting past this before connection is fully established (clientId is blank)
  //TODO: if it reconnected, reset client id

  bool tryAgain = false;
  wl_status_t status = WiFi.status();
  bool connected = status == WL_CONNECTED;

  if (!connected) {
    this->trackDisconnect(0);

    unsigned long now = millis();
    if (lastReconnectAttempt == 0) {
      lastReconnectAttempt = now;
    } else if (lastReconnectAttempt + reconnectAttemptInterval < now) {
      lastReconnectAttempt = now;
      Serial.println("Reconnect");
      WiFi.reconnect();
      tryAgain = true;
    }

  }

  if (disconnectTime==0 || tryAgain || connected) {
    if (!timeClient.update()) {
      this->trackDisconnect(1);
      return false;
    }

    if (!myMqttWrapper.loop()) {
      this->trackDisconnect(2);
      return false;
    }

    if (disconnectTime!=0) {
      disconnectTime = 0;
      lastReconnectAttempt = 0;
      Serial.println("Reconnected!");
    }
  }

  return WiFi.status() == WL_CONNECTED;
}

void NetworkApp::updateStatus(char statusType[], char value[])
{
  StaticJsonDocument<256> doc;
  doc["value"] = value;
  this->myMqttWrapper.publish(statusType, doc);
}
