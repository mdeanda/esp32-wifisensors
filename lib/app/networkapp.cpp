#include "networkapp.h"
#include "../../config.h"

void NetworkApp::setup()
{
  mqttClient.setServer(MQTT_SERVER, 1883);
  config.autoReconnect = true;
  config.principle = AC_PRINCIPLE_RSSI;
  config.portalTimeout = 30000;
  config.retainPortal = true;
  this->justReconnected = false;
  nextOnlineMessage = millis() + onlineMessageInterval;

  portal.config(config);
  setupAcPages();
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

  ACText(header, "MQTT broker settings");
  ACText(caption1, "Publishing the WiFi...");
  ACSubmit(save, "SAVE", "/mqtt_settings");
  AutoConnectAux  aux1("/mqtt_setting", "MQTT Setting", true, { header, caption1, save });

  ACText(caption2, "Save parameters");
  ACSubmit(start, "START", "/mqtt_start"); 
  AutoConnectAux  aux2("/mqtt_save", "MQTT Setting", false, { caption2, start });

  AutoConnectAux  aux3("/mqtt_start", "MQTT Start");

void NetworkApp::setupAcPages()
{
  //portal.join({ aux1, aux2, aux3 });
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

  if (this->justReconnected) {
    this->justReconnected = false;
  }

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
      this->justReconnected = true;
      Serial.println("Reconnected!");
    }

    sendOnlineMessage();
  }

  return WiFi.status() == WL_CONNECTED;
}

void NetworkApp::sendOnlineMessage()
{
  unsigned long now = millis();
  if (nextOnlineMessage < now) {
    nextOnlineMessage = now + onlineMessageInterval;
    myMqttWrapper.sayOnline();
  }
}

void NetworkApp::updateStatus(const char * statusType, JsonDocument &doc)
{
  this->myMqttWrapper.publish(statusType, doc);
}

bool NetworkApp::isReconnected()
{
  return this->justReconnected;
}
