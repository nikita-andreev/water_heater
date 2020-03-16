#include <DS18B20.h>

#define FLOW_SENSOR_PIN 3
#define TEMPERATURE_SENSOR_PIN 4
#define PUMP_CONTROL_PIN 7
#define MAX_FLOW = 100;

const float max_flow_millis = 40;
const float max_temp = 48;

unsigned long last_seen_flow, pre_last_seen_flow = 0;
float water_flow = 0;
float current_temp;
bool light = true;
DS18B20 ds(4);
uint8_t address[8];

void setup() {
//  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
  pinMode(PUMP_CONTROL_PIN, OUTPUT);
  digitalWrite(PUMP_CONTROL_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_PIN, HIGH);
  
  ds.getAddress(address);
  ds.select(address);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flow_interrupt, FALLING); 
}

void loop() {
  if (millis() - last_seen_flow > 2000) {
    water_flow = 0;
    last_seen_flow = 0;
    pre_last_seen_flow = 0;
  } else {
    if (last_seen_flow != 0 && pre_last_seen_flow !=0) {
      water_flow = max_flow_millis / (last_seen_flow - pre_last_seen_flow);      
    } else {
      water_flow = 0;
    }

  }
  if (water_flow > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

//  Serial.println(water_flow);
  
  current_temp = ds.getTempC();
  if (water_flow > 0 && current_temp < max_temp) {
    digitalWrite(PUMP_CONTROL_PIN, LOW);
  } else {
    digitalWrite(PUMP_CONTROL_PIN, HIGH);
    if(water_flow < 0.1 ){
      delay(3000);
    }
  }
}

void flow_interrupt() {
  pre_last_seen_flow = last_seen_flow;
  last_seen_flow = millis();
  light = !light;
//  digitalWrite(LED_BUILTIN, light);
}
