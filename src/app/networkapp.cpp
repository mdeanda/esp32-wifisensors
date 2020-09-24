#include "networkapp.h"
#include "../../config.h"

void NetworkApp::setup()
{
  mqttClient.setServer(MQTT_SERVER, 1883);
  config.autoReconnect = true;
  config.principle = AC_PRINCIPLE_RSSI;
  //config.apid = "ESP-" + String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  portal.config(config);
  if (portal.begin()) {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
    String localIp = WiFi.localIP().toString();
    localIp.replace(".", "_");
  
    Serial.println(localIp);
    myMqttWrapper.setTopic(localIp);
    myMqttWrapper.setClientName(localIp);

    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  myMqttWrapper.setup();

  timeClient.begin();
  
}

bool NetworkApp::loop()
{
  server.handleClient();
  portal.handleClient();

  timeClient.update();

  //TODO: we're getting past this before connection is fully established (clientId is blank)
  while (WiFi.status() == WL_IDLE_STATUS) {
    Serial.println("Not connected to WiFi");
    // do we really want this?
    delay(500);
    //ESP.restart();
    delay(500);
    return;
  }
  
  if (!myMqttWrapper.loop()) {
    delay(5000);
    return;
  }

}
