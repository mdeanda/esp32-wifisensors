#include "mymqttwrapper.h"

MyMqttWrapper::MyMqttWrapper()
{
  disabled = true;
}

MyMqttWrapper::MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient, MY_MQTT_CALLBACK_SIGNATURE)
{
  index = 0;
  this->mqttClient = mqttClient;
  this->timeClient = timeClient;
  topic = "unknown";
  inTopic = "inTopic";
  disabled = false;
  this->callback = callback;
}

void MyMqttWrapper::mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void MyMqttWrapper::setup() {
  this->mqttClient->setCallback(&MyMqttWrapper::mqttCallback);
}

void MyMqttWrapper::setTopic(String topic)
{
  this->topic = topic;
}

void MyMqttWrapper::setInTopic(String inTopic)
{
  this->inTopic = inTopic;
}

void MyMqttWrapper::setClientName(String clientName)
{
  this->clientName = clientName;
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

/**
 * verify connection, if not connected return false
 */
bool MyMqttWrapper::loop()
{
  if (!mqttClient->connected()) {
    if (!reconnectMqtt()) {
      return false;
    }
  }
  mqttClient->loop();
  return true;
}

void MyMqttWrapper::sayHello()
{
    StaticJsonDocument<400> doc;
    doc["message"] = "hello world";

    publish(doc);
}


bool MyMqttWrapper::reconnectMqtt()
{
  // Loop until we're reconnected
  Serial.print("mqtt...");
  if (!mqttClient->connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(clientName.c_str());

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient->connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sayHello();
      // ... and resubscribe
      mqttClient->subscribe(this->inTopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttClient->state());
      return false;
    }
  }

  return mqttClient->connected();
}

