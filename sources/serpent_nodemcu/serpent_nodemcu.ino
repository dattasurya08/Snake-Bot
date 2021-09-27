#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH 600
#define MAX_PULSE_WIDTH 2400
#define FREQUENCY 50
#define N 5

int initVal = 90;
int counter = 0;
float lag = 0.5712;
float pi =3.14159;
int frequency = 1;
int amplitude = 30;
int rightOffset = -5;
int leftOffset = 5;
int offset = 6;
int delayTime = 3;
int startPause = 5000;
int test = -3;

char serVal = 's';

const char* ssid = "Serpent Bot";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
ESP8266WebServer server(80);

char wifiReadCMD(void)
{
  char buf;
  if(Serial.available() > 0)
  {
    serVal = Serial.read();
    while(Serial.available() > 0)
    {
      buf = Serial.read();
    }
  }
  return serVal;
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

void setServoAngle(int motor, int angle){
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  pwm.setPWM(motor, 0, analog_value);
}

void stop(void)
{
  for(int i = 0; i < N; i++)
  {
    setServoAngle(i, initVal);
  }
}

void forward(void)
{
  for(counter = 0; counter < 360; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
}

void backward(void)
{
  for(counter = 360; counter > 0; counter--)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
}

void right(void)
{
  for(counter = 0; counter < 10; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+.1*counter*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+.1*counter*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 350; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 360; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+.1*(360-counter)*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+.1*(360-counter)*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
}

void left(void)
{
  for(counter = 0; counter < 10; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+.1*counter*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+.1*counter*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 350; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 360; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        setServoAngle(i, initVal+offset+.1*(360-counter)*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        setServoAngle(i, initVal+.1*(360-counter)*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
}

void setup()
{
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  stop();
  delay(2000);
  Serial.begin(9600);
  Serial.println("f: forward, b: backward, l: left, r: right, s: stop");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();
}

void loop()
{
  serVal = wifiReadCMD();

  if(serVal == 'f')
  {
    forward();
  }
  else if(serVal == 'b')
  {
    backward();
  }
  else if(serVal == 'r')
  {
    right();
  }
  else if(serVal == 'l')
  {
    left();
  }
  else
  {
    stop();
  }
}
