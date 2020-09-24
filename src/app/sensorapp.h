#ifndef SENSOR_APP_H
#define SENSOR_APP_H


class SensorApp {
  private:

  public:
    SensorApp();
    SensorApp(PubSubClient * mqttClient, NTPClient * timeClient);
    void setup();
    bool loop();


};


#endif
