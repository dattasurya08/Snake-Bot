#include <Servo.h>

#define N 5

Servo s[N];

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

char serialReadCMD(void)
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

void stop(void)
{
  for(int i = 0; i < N; i++)
  {
    s[i].attach(13 - i);
    s[i].write(initVal);
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
        s[i].write(initVal+offset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
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
        s[i].write(initVal+offset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
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
        s[i].write(initVal+offset+.1*counter*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*counter*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 350; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+offset+rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 360; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+offset+.1*(360-counter)*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*(360-counter)*rightOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
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
        s[i].write(initVal+offset+.1*counter*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*counter*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 350; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+offset+leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
  for(; counter < 360; counter++)
  {
    delay(delayTime);
    for(int i = 0; i < N; i++)
    {
      if(i == 0)
        s[i].write(initVal+offset+.1*(360-counter)*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
      else
        s[i].write(initVal+.1*(360-counter)*leftOffset+amplitude*cos(frequency*counter*pi/180+(i-2)*lag));
    }
  }
}

void setup()
{
  stop();
  delay(2000);
  Serial.begin(9600);
  Serial.println("f: forward, b: backward, l: left, r: right, s: stop");
}

void loop()
{
  serVal = serialReadCMD();
  if(serVal == 'f')
    forward();
  else if(serVal == 'b')
    backward();
  else if(serVal == 'r')
    right();
  else if(serVal == 'l')
    left();
  else
    stop();
}
