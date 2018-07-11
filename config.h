#ifndef _MAIN_H__
#define _MAIN_H__


void smartConfig();
void waitKey();

// 设备配置参数
const char* mqtt_server = "192.168.1.47";
const uint16_t mqtt_port = 8886;
const char* GUUID = "device_4867bf13-7d60-41f8-8252-3597267ad3f2";
const char* BLUE_UUID = "03515869-eb59-4478-95f0-b17fadee1000";
const char* GREEN_UUID = "a7596272-6261-4aeb-bed3-e47ae73cd88e";
const char* USER = "6ba7b810-9dad-11d1-80b4-00c04fd430c8";

bool ledState = 0;

#endif
