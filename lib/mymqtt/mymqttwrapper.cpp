#include "mymqttwrapper.h"

std::vector<MyMqttWrapper *> MyMqttWrapper::listeners;

MyMqttWrapper::MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient)
{
  index = 0;
  this->mqttClient = mqttClient;
  this->timeClient = timeClient;
  topic = "unknown";
  inTopic = "inTopic";
  disabled = false;

  String clientId = "ESP32Client-";
  clientId += String(random(millis()), HEX);
}

void MyMqttWrapper::mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  int size = MyMqttWrapper::listeners.size();
  for (int i=0; i<size; i++) {
      MyMqttWrapper * wrapper = MyMqttWrapper::listeners.at(i);
      wrapper->messageReceived(topic, payload, length);
  }
}

void MyMqttWrapper::messageReceived(char* topic, byte* payload, unsigned int length)
{
  if (this->listener != NULL) {
    char chars[length+1];
    //TODO: find a better way to convert byte* to char*
    for (int i=0;i<length;i++) {
      chars[i] = (char) payload[i];
    }
    chars[length] = '\0';
    this->listener->onMessage(topic, chars, length);
  }
}

void MyMqttWrapper::setup() 
{
  this->mqttClient->setCallback(&MyMqttWrapper::mqttCallback); 
  MyMqttWrapper::listeners.push_back(this);
}

void MyMqttWrapper::setTopic(String topic)
{
  this->topic = topic;
}

void MyMqttWrapper::setInTopic(String inTopic)
{
  this->inTopic = inTopic;
}

void MyMqttWrapper::setListener(MqttListener * listener)
{
  this->listener = listener;
}

void MyMqttWrapper::setClientName(String clientName)
{
  this->clientName = clientName;
}

void MyMqttWrapper::publish(JsonDocument& document)
{
  this->publish(NULL, document);
}

void MyMqttWrapper::publish(const char *topic, JsonDocument& document)
{
  String topicValue;
  if (topic != NULL) {
    topicValue = this->topic + "/" + topic;
  } else {
    topicValue = this->topic;
  }

  char json[2048];
  
  index += 1;
  document["id"] = index;
  document["ts"] = timeClient->getEpochTime();
  serializeJson(document, json, 2048);

  if (!mqttClient->connected()) {
    Serial.print("not connected: ");
    Serial.println(json);
    return;
  } else {
    Serial.println(json);
  }

  if (!disabled) {
    mqttClient->publish(topicValue.c_str(), json);
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
  return mqttClient->loop();
}

void MyMqttWrapper::sayOnline()
{
    //StaticJsonDocument<400> doc;
    //doc["status"] = "ONLINE";

    //publish("status", doc);
}


bool MyMqttWrapper::reconnectMqtt()
{
  // Loop until we're reconnected
  Serial.print("mqtt...");
  if (!mqttClient->connected()) {
    Serial.print("attempting connection...");
    Serial.print(clientName.c_str());

    String topic = clientName.c_str() + String("/status");
    if (mqttClient->connect(clientId.c_str(), topic.c_str(), 0, true, "{\"status\":\"OFFLINE\"}")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sayOnline();
      mqttClient->publish(topic.c_str(), "{\"status\":\"ONLINE\"}", true);
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

