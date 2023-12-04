#define WIFI_SSID "TP_LINK_ddd"
#define WIFI_PASS "noipbuaa2012"
#define HOSTNAME  "cyrcar"


#include "webStream.h"
#include <WebServer.h>

using namespace eloq;
using namespace yccy::car;

WiFiServer server(82, 1);
#define led 33
#define pwm_0 4
#define pwm_1 2

uint8_t pins[] = {12, 13, 14, 15};

/**
 * 
 */
void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("___MJPEG STREAM SERVER___");

    // camera settings
    // replace with your own model!
    camera.pinout.aithinker();
    camera.brownout.disable();
    camera.resolution.qvga();
    camera.quality.low();

    // init camera
    while (!camera.begin().isOk())
        Serial.println(camera.exception.toString());
    Serial.println("Camera OK");

    // connect to WiFi
    while (!wifi.connect().isOk())
        Serial.println(wifi.exception.toString());
    Serial.println("WiFi OK");

    // start mjpeg http server
    while (!webstream.begin().isOk())
        Serial.println(webstream.exception.toString());
    Serial.println("WebStream OK");
    //set pins
  
    pin_init();
    analogWriteResolution(10);
    server.begin();
    digitalWrite(pins[3], LOW);
    Serial.println(webstream.address());
}
    


void loop() {
  String mes = "";
  WiFiClient client = server.accept();
  if (client)
  {
    // Serial.println(a);
    Serial.println("\n[Control Client connected]");
    while (client.connected())
    {
      if (client.available()) {
        // while (client.available()){ 
        //   mes += char(client.read());
        // }
        mes = client.readStringUntil('\n');
        Serial.println(mes);
        run(mes);
        mes = "";
      }
    }
    client.stop();
    pin_init();
    Serial.println("[Control Client disconnected]");
  }
}


void pin_init(){
  pinMode(led, OUTPUT);
  pinMode(pwm_0, OUTPUT);
  pinMode(pwm_1, OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(pwm_0, LOW);
  digitalWrite(pwm_1, LOW);
  int len = sizeof(pins) / sizeof(pins[0]);
  for (int i = 0; i < len; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
}


void run(String mes){
  int index = 0;
  int i = 0;
  int temp, x;
  int len = mes.length();
  int val[3];
  while (index < len){
    temp = mes.indexOf(',', index);
    if (temp == -1) temp = len;
    val[i++] = mes.substring(index, temp).toInt();
    index = temp + 1;
  }
  car_pwm(val[0], pins[0], pins[1], pwm_0); 
  car_pwm(val[1], pins[2], pins[3], pwm_1);
}


void car(int x, uint8_t pin_1, uint8_t pin_2){
  if (x > 0) {
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, HIGH);
  } else if (x < 0){
    digitalWrite(pin_1, HIGH);
    digitalWrite(pin_2, LOW);
  } else {
    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, LOW);
  }
}

void car_pwm(int x, uint8_t pin_1, uint8_t pin_2, uint8_t pwm){
    analogWrite(pwm, (x != 0) * (abs(x) * 0.13 + 410));
    car(x, pin_1, pin_2);

}