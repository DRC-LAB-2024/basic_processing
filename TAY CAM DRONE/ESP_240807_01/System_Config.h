#define CE_PIN 26
#define CSN_PIN 27
#define MOSI_PIN 13
#define MISO_PIN 12
#define SCK_PIN 14
#define loa_pin 33
#define motor_pin 25
//-------------------- DINH THI VONG LAP CHINH------------------------------
#define TS_MAIN 10  //(ms)
//---------------------Serial 2-----------------------------------
#define MegaPort Serial2
//----------------------DINH NGHI CAU TRUC BO NHO CHO CAC DIEM DU LIEU BAY-----------------------
#define APP_DATA_SEND_RF_MAX_TIMOUT 5000 //2s

#define APP_DATA_SEND_UNSUCCESS_FROM_DRONE 5
#define APP_DATA_RECEIVED_SUCCESS 0
#define APP_DATA_RECEIVED_UNSUCCESS 1
#define APP_DATA_RECEIVED_NONE 2
#define APP_DATA_RECEIVING 3
#define APP_DATA_RECEIVED_UNKNOWDATA 4
#define APP_DATA_RECEIVING_WAIT_MAX 100


#define FLY_MODE_SINGLE_POINT 2
#define FLY_MODE_SMULTIPLE_POINT 3
#define POINT_MEMORY_INDEX_LONG_ALL 60
#define POINT_MEMORY_INDEX_LEN_ALL 8
#define POINT_MEMORY_INDEX_MODE 0
#define POINT_MEMORY_INDEX_LAT 1
#define POINT_MEMORY_INDEX_LONG 2
#define POINT_MEMORY_INDEX_Z 3
#define POINT_MEMORY_INDEX_TIME 4
#define POINT_MEMORY_INDEX_LEN 5
#define POINT_MEMORY_INDEX_INDEX 6
#define POINT_MEMORY_INDEX_CHECKSUM 7
//--------------HEADER & FOOT OF APP DATA
#define APP_DATA_HEADER '*'
#define APP_DATA_FOOTER '#'

long POINT_MEMORY_VAR[POINT_MEMORY_INDEX_LONG_ALL][POINT_MEMORY_INDEX_LEN_ALL];

String Bluetooth_Name = "ESP32";//"ORC_CP_V2_S1";  //Bluetooth name
const char *serviceUUID = "00001101-0000-1000-8000-00805F9B34FB"; 
long Fly_Mode_Data = 0, Current_Data_Index = -1, lenTempInt;
int BT_data_receiving_cnt = 0, intTemp;
byte preBTProcess = APP_DATA_RECEIVED_SUCCESS;
char dataBTConfirm[50];
bool tempBool;
byte megasendCnt = 0;
long currentTime;
//const float EslapTime = 0.01;
