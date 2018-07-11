#include <ESP8266WiFi.h>
#include "config.h"

#define PINLED 2
#define PINKEY 0

void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  //调用smartconfig功能
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    digitalWrite(PINLED, 0);
    delay(300);
    digitalWrite(PINLED, 1);
    delay(300);
    digitalWrite(PINLED, 0);
    delay(300);
    digitalWrite(PINLED, 1);
    delay(1000);
    //若配置完成打印获取到的ssid
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;
    }
  }
}

/*
 * 启动前5秒检查用户是否按键，如果有按键判断是短按或者长按，分别进入smartconfig模式或者恢复出厂
*/
void waitKey()
{
  pinMode (PINLED, OUTPUT);
  pinMode (PINKEY, INPUT);
  digitalWrite(PINLED, 0);
  Serial.println("long press key: smartconfig mode");
  char keyCnt = 0;
  unsigned long preTick = millis();
  unsigned long preTick2 = millis();
  int num = 0;
  while (1)
  {
    ESP.wdtFeed();
    if (millis() - preTick < 10 ) continue;
    preTick = millis();
    num++;
    if (num % 10 == 0)
    {
      ledState = !ledState;
      digitalWrite(PINLED, ledState);
      Serial.print(".");
    }
    if (keyCnt >= 200)
    { //长按
      keyCnt = 0;
      Serial.println("\r\nLong Press key");
      smartConfig();
    }
    if (digitalRead(PINKEY) == 0) keyCnt++;
    else
      keyCnt = 0;

    if (millis() - preTick2 > 5 * 1000) break;
  }
  digitalWrite(PINLED, 0);
  pinMode (PINKEY, OUTPUT);
}
