#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include "BLEConfig.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ModbusMaster.h>

#define RS232_LED 32  
#define RS485_LED 19 
#define ANALOG01_LED 18 
#define ANALOG02_LED 5 
#define DIGITAL01_LED 33
#define DIGITAL02_LED 25

#define ANALOG01_PIN 34
#define ANALOG02_PIN 35
#define DIGITAL01_PIN 26
#define DIGITAL02_PIN 27

#define BUTTON 23

#define MAX485_RE_NEG  4
#define MAX485_DE      4

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

#define SDA_PIN 21
#define SCL_PIN 22 

#define TX1 2
#define RX1 15

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ModbusMaster node;

// HardwareSerial Serial2(2);

IPAddress server_addr(103, 200, 23, 149);
char user[] = "optimaro_hoang_dev_esp32";
char password[] = "Hoang2468@";
char db[] = "optimaro_hoang_dev";

char* ssid = "";
char* pass = "";

char* device_id = "MONITOR01";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

/*------------------------------*/
// Procedure time:
unsigned long read_data_timer = 0;
unsigned long send_data_timer = 0;
unsigned long show_data_timer = 0;
unsigned long rs485_timer = 0;
/*------------------------------*/

/*------------------------------*/
// Data
float temp = 0;
float humid = 0;
int a1 = 0;
int a2 = 0;
int d1 = 0;
int d2 = 0;
/*------------------------------*/