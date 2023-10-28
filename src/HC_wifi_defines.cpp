#include "HC_wifi_defines.hpp"

IPAddress local_IP(192, 168, 0, 220);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 90);
String hostname = "ESP32_Home_Controller";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

WiFiUDP UDP;
WakeOnLan WOL(UDP);

shutterMsg sMsg_tx;
QueueHandle_t xQueueShutter_tx;
