#include "mymqttwrapper.h"

MyMqttWrapper::MyMqttWrapper()
{
  disabled = true;
}

MyMqttWrapper::MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient)
{
  index = 0;
  this->mqttClient = mqttClient;
  this->timeClient = timeClient;
  topic = "unknown";
  disabled = false;
}

void MyMqttWrapper::setTopic(String topic)
{
  this->topic = topic;
}

void MyMqttWrapper::publish(JsonDocument& document)
{
  String json;
  
  index += 1;
  document["id"] = index;
  document["ts"] = timeClient->getEpochTime();
  serializeJson(document, json);

  if (!mqttClient->connected()) {
    Serial.print("not connected: ");
    Serial.println(json);
    return;
  } else {
    Serial.println(json);
  }

  if (!disabled) {
    mqttClient->publish(topic.c_str(), json.c_str());
  } else {
    Serial.println("disabled");
  }
}
