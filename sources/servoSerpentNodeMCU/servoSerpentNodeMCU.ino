#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define N 5
#define MIN_SERVO_PULSE 600
#define MAX_SERVO_PULSE 2500

Servo s[N];

const int sm[N] = {16, 5, 4, 0, 2};

const int initVal = 90;
const float lag = 0.5712;
const float pi =3.14159;
const int frequency = 6;
const int amplitude = 40;
const int rightofset = -5;
const int leftofset = 5;
const int ofset = 6;
const int delayTime = 3;
const int startPause = 5000;
const int test = -3;
const char *ssid = "Snake Bot BMSCE";
const int port = 8080;

IPAddress ip(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);

ESP8266WebServer server(port);

int counter = 0;
String serVal = "S";
String preVal = "S";

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid);
  server.on("/move", HTTP_GET, handleMoveRequest);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Started Server");
  sstop();
  delay(2000);
}

void loop()
{
  server.handleClient();
  if(!serVal.equals(preVal))
  {
    counter = 0;
  }
  if(serVal.equals("F"))
    forward();
  else if(serVal.equals("B"))
    backward();
  else if(serVal.equals("R"))
    right();
  else if(serVal.equals("L"))
    left();
  else if(serVal.equals("S"))
    sstop();
  preVal = serVal;
}

void handleNotFound()
{
  server.send(404, "text / plain", "404: Not found");
}

void handleMoveRequest()
{
  if (!server.hasArg("dir"))
  {
    server.send(404, "text / plain", "Move: undefined");
    return;
  }
  String dir = server.arg("dir");
  Serial.println(dir);
  server.send(200, "text / plain", "Move: recieved");
  serVal = dir;
}

void sstop(void)
{
  for(int i = 0; i < N; i++)
  {
    s[i].attach(sm[i], 600, 2500);
    s[i].write(initVal);
  }
}

void forward(void)
{
  delay(delayTime);
  for(int i = 0; i < N; i++)
  {
    if(i == 0)
      s[i].write(initVal+ofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
    else
      s[i].write(initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
  }
  if(counter >= 360) {
    counter = 0;
  }
  else {
    counter++;
  }
}

void backward(void)
{
  delay(delayTime);
  for(int i = 0; i < N; i++)
  {
    if(i == 0)
      s[i].write(initVal+ofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
    else
      s[i].write(initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
  }
  if(counter <= 0) {
    counter = 360;
  }
  else {
    counter--;
  }
}

void right(void)
{
  delay(delayTime);
  if(counter < 10)
  {
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal-ofset+.1*counter*rightofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*counter*rightofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  else if(counter < 350)
  {
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal-ofset+rightofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+rightofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  else if(counter < 360)
  {
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal-ofset+.1*(360-counter)*rightofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*(360-counter)*rightofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  if(counter >= 360) {
    counter = 0;
  }
  else {
    counter++;
  }
}

void left(void)
{
  delay(delayTime);
  if(counter < 10)
  {
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+ofset+.1*counter*leftofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*counter*leftofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  else if(counter < 350)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+ofset+leftofset+20*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+leftofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  else if(counter < 360)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+ofset+.1*(360-counter)*leftofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*(360-counter)*leftofset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  if(counter >= 360) {
    counter = 0;
  }
  else {
    counter++;
  }
}
