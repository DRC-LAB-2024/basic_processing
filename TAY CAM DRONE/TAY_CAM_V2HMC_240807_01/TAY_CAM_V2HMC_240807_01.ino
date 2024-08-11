#include <SPI.h>
#include <RF24.h>
#include <printf.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "System_Config.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SimpleKalmanFilter.h>



// Create display oled
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  
// khởi tạo chân CE và CSN cho nrf24l01
RF24 radio(34, 49); // CE, CSN

//khởi tạo cảm biến la bàn
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
//khởi tạo bộ lọc kalman
SimpleKalmanFilter bo_loc(0.01, 0.01, 0.001);
float heading = 0;

const int z_max = 100 ; // m 
 int scale_z = 2;
const byte addresses[][8] = {"abcd123","qwer234"}; 

bool LedControl = false, LedStop = LOW, LedMode = LOW, ConMode = false, CheckEmer = false, CheckVol = false;
float jlxErr=0, jlyErr=0, jrxErr=0, jryErr=0;
//int32_t jlxErr=0, jlyErr=0, jrxErr=0, jryErr=0;

float eT, currentTime, previousTime;
const float EslapTime = 0.01; //s
long Dx, Dy, YawDes=0, zDes =0, CheckSum = 0;
long rollsc, pitchsc, yawsc, zsc;

long CurrentTimeLed = 0, CurrentTimeMode = 0;
bool report = true;
uint8_t  PreStaEmer = 1 ;

//================ BIEN CHE DO DIEU KHIEN===================
bool PreBtnStart = HIGH, PreBtnStop = HIGH, PreBtnEmer = HIGH, MenuMode = false;
uint8_t PreStaMode = 2;
byte BtState = 8, preBtState = 8;
int count = 0;
//=============== END OF BIEN CHE DO DIEU KHIEN ============

byte BtMode = 4, ByteStart = '*', ByteMid = 0, ByteID = 0;
byte ByteFlow, BytePumpState,ByteLen,ByteStt; 

String DataRecApp=" ";
bool CheckD = false;
long DataESPRec[POINT_MEMORY_INDEX_LEN_ALL];
long RFSendDefault[POINT_MEMORY_INDEX_LEN_ALL];
uint8_t CountIndex = 0;
byte Send[35] = {0};
int DataRecDrone = 0, display_cnt = 0;

///////////////////
bool led_in_temp1 = false; //Gia tri khoi tao chop led o che do Emergency
bool led_MODE_temp1 = false; //Gia tri khoi tao chop led o che do Emergency
uint8_t mode_control_temp1 = 1;

//==============biến trở scale Joystick===================
//int rollscale = A12;
//int pitchscale = A13;
//int zscale = A15;
//int yawscale = A14;
//==============end of biến trở scale Joystick============

//==============biến Uart=================
int bytesRead = 0;
byte mang[3];
//============end of biến Uart============

void setup() {
  ByteID = 0;
  BytePumpState = 0;
  ByteFlow = 6;
  ByteLen = 0;
  ByteStt = 0;
  CheckSum = 0;
  // Gia tri mac dinh goi qua drone
  for (int k = 0; k<POINT_MEMORY_INDEX_LEN_ALL; k++)
    RFSendDefault[k] = 0;
  //Cau hinh UART 0
  Serial.begin(115200);
  //Serial1.begin(115200);
  Serial.println("--------------Done SetUp RF!------------------------------");
  SerEsp.begin(9600);
  SetUpIO();
  setup_oled();
  setup_laban();
  digitalWrite(LedDKC, HIGH);
  
  while(!radio.begin()){
    digitalWrite(LedRFNhan, LOW);
  }
  SetupRF();
  Serial.println("--------------Done SetUp RF11!------------------------------");
  
  JsErr();
  
  CurrentTimeLed = millis();
  CurrentTimeMode = millis();
  InitializePre();
  digitalWrite(LedCDBay, HIGH);
  delay(1000);  
  digitalWrite(LedRFNhan, HIGH);
  Serial.println("------------------------------Done Setup!-------------------------------------");

}
void loop() {
  // thời gian lấy mẫu EslapTime
  previousTime = currentTime;
  currentTime = micros(); 
  eT = (currentTime - previousTime)/1000000; 
  while(eT < EslapTime){
   currentTime = micros(); 
  eT = (currentTime - previousTime)/1000000; 
  }
  // đọc cảm biến la bàn
  heading = la_ban();
  // Doc gia tri pump
  ManualPumpStatus = digitalRead(PUMP_MODE_PIN);
  
//   // đọc tín hiệu joysticks
   ReadJoySticks();
   BtState = ReadButton(BtState, &PreBtnStart, &PreBtnStop, &PreBtnEmer); //DOC TRANG THAI NUT NHAN: START, STOP, EMER
  PreStaMode = ReadSwManualMode(PreStaMode); //DOC TRANG THAI MANUAL
  display_cnt++;
  if(display_cnt >= 5)
  {
    display_cnt = 0;
    MenuMode = Read_BtMenu(PreStaMode, MenuMode); // đọc trạng thái menu
  }  
  //-----GOI DU LIEU TOI DRONE-----------------------
  if((PreStaMode == MANUALMODE_ANGLE_SEND)||(PreStaMode == MANUALMODE_POS_SEND)) //Manual Mode
  {
    SendManualData2Drone(PreStaMode,preBtState, BtState,ByteStart,ByteID,Dx,Dy,YawDes,zDes,ByteLen,ByteStt, ByteFlow, BytePumpState, Send);
    report = radio.write(&Send, sizeof(Send));
  }else if((PreStaMode == AUTOMODE_SINGLEPOINT_SEND)||(PreStaMode == AUTOMODE_MULTIPLEPOINT_SEND)) //Auto mode
  {
    if(ReceivedDataFromESP32(&PreStaMode,DataESPRec))
    {
      SendAutoMutiPData2Drone(PreStaMode,preBtState, BtState,ByteStart,ByteID,DataESPRec,ByteFlow, BytePumpState, Send);
      //Goi du lieu den khi thanh cong trong vong 100ms
      ctimeRF = millis();
      ptimeRF = ctimeRF;
      report = false;
      while((!report) && (abs(ctimeRF - ptimeRF) < 100)){
        report = radio.write(&Send, sizeof(Send));
        delay(10);
        ctimeRF = millis();
      }
    }else{
      SendAutoMutiPData2Drone(PreStaMode,preBtState, BtState,ByteStart,ByteID,RFSendDefault,ByteFlow, BytePumpState, Send);      
      report = radio.write(&Send, sizeof(Send));
    }
  }else
    report = false;
    //  report = true;
//   // tín hiệu RF
   digitalWrite(LedRFNhan, report);
//   Serial.println(BtState);
//   Serial.println(BtMode);
/*   if((BtMode == 1)||(BtMode == 4)) //GOI DU LIEU O CHE DO MANUAL CHO DRONE
  {
     // SendManualData2Drone(BtMode,BtState*0 + 1,ByteStart,ByteID,Dx*0 + 10,Dy*0 - 10,YawDes,zDes,ByteLen,ByteStt, ByteFlow, BytePumpState, Send);
       SendManualData2Drone(BtMode,BtState,ByteStart,ByteID,Dx,Dy,YawDes,zDes,ByteLen,ByteStt, ByteFlow, BytePumpState, Send);
   }*/
   //else
//   {
//    if(BtMode == 2) //GOI DU LIEU O CHE DO BAY 1 DIEM CHO DRONE
//    {
//     SendAuto1PData2Drone(BtMode,BtState,ByteStart,ByteID,Rec,ByteLen,ByteStt, ByteFlow, BytePumpState, Send);
//     }
//     else if(BtMode == 3){
//      SendAutoMutiPData2Drone(BtMode,BtState,ByteStart,ByteID,Rec,ByteFlow, BytePumpState, Send);

//     // nhận tín hiệu từ Drone
//       if (RF_Read_Response(&DataRecDrone)){
//       SerEsp.println(DataRecDrone);   
//     }
    
//     }
//    }
  
  
 preBtState = BtState;
 //////////  RF_Read_Response(&DataRecDrone);
 //   radio.write(&Send, sizeof(Send));

//   // tín hiệu điều khiển
    led_in_temp1 = LedControlStatus(BtState, led_in_temp1);


// //   //tín hiệu mode
    led_MODE_temp1 = LedModeStatus(PreStaMode, led_MODE_temp1, LedCDBay);
//   Serial.print("report: ");
//   Serial.println(report);


//  if (BtMode == 3){
//    if (RF_Read_Response(&DataRecDrone)){
//        SerEsp.println(DataRecDrone);   
//    }
//  }
//RX_UART();
// ShowData();
Serial.print("Pump:");
Serial.println(digitalRead(PUMP_MODE_PIN));

}
