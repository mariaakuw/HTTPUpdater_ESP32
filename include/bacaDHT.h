#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11

// DHT Sensor
uint8_t DHTPin = 17;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;