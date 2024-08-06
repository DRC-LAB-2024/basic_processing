#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
//#include <HardwareSerial.h>
#include "BluetoothSerial.h"
#include "System_Config.h"

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;


SPIClass *hspi = NULL;
//HardwareSerial SerialPort(2);  // use UART2

// Định nghĩa các chân kết nối


RF24 radio(CE_PIN, CSN_PIN);

// Nơi nhận dữ liệu
//byte dataRec[55] = { 0 };
byte sendData[3] = { 0 };
// long roll = 0;
// long pitch = 0;
// long yaw = 0;
// long Z = 0;
//String data_rec;//, Bluetooth_Name = "ORC_CP_V2_S1";
//long currentTime, previousTime, errorTime;
//float eT;
//int count = 0;
void setup() {
  BT_data_receiving_cnt = 0;
  // Bắt đầu Serial Monitor
  //Serial_2.begin(115200);
  Serial.begin(115200);
  MegaPort.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("UART is started!");
  //SerialPort.begin(115200, SERIAL_8N1, 16, 17);  // 16 RX, 17 TX
  // Bluetooth device name
  SerialBT.begin(Bluetooth_Name, true);
  SerialBT.begin(serviceUUID);
  Serial.println("The device started, now you can pair it with bluetooth!");
  Setup_RF();
  // sendData[1] = 22;
  // sendData[2] = 22;
  pinMode(loa_pin, OUTPUT);
  //digitalWrite(loa_pin, HIGH);
  pinMode(motor_pin, OUTPUT);
  digitalWrite(motor_pin, HIGH);
  //Lay thoi gian hien ta
  currentTime = micros();
  Serial.println("ESP Setup has done!");
}

void loop() {
  //10m he thong se xu ly mot lan 
  currentTime = softDelay(currentTime, TS_MAIN);
  
//  Serial.println("Loop.");
  digitalWrite(motor_pin, HIGH);
  //Xu ly du lieu nhan duoc tu Bluetooth
  preBTProcess = ProcessDataFromBluetooth(POINT_MEMORY_VAR, &Fly_Mode_Data, &Current_Data_Index, preBTProcess, &BT_data_receiving_cnt);
  //----Xu ly he thong khi nhan du du lieu----------
  if(preBTProcess == APP_DATA_RECEIVED_SUCCESS){
    //B1: GOI TIN HIEU TOI MEGAPRO---
    //B2: DOI NHAN DUOC TIN HIEU PHAN HOI TU DRONE
    //--NEU SAU 2s KHONG NHAN DUOC TIN HIEU TU DRONE ==> GOI TIN HIEU LOI LEN APP (APP_DATA_SEND_UNSUCCESS_FROM_DRONE) ==> THOAT VONG LAP
    //--NEU NHAN DUOC TIN HIEU PHAN HOI DUNG TU DRONE 
    //      ==> NEU DU LIEU TRUYEN DA HET, GOI TIN HIEU THANH CONG (APP_DATA_SEND_SUCCESS) ==> THOAT VONG LAP
    //      ==> NEU DU LIEU TRUYEN DA HET TRO VE BUOC 1
    //------------------------------------------------------------
    lenTempInt = POINT_MEMORY_VAR[0][POINT_MEMORY_INDEX_LEN];
    Serial.println("Bat dau goi du lieu qua MEGA PRO:");
    for (int indTemp = 0; indTemp < ((int)lenTempInt); indTemp++) 
    {  
      //B1:
      sendDatabySerial2(POINT_MEMORY_VAR, indTemp);
      delay(1);
      tempBool = true; //Check_RF(indTemp, APP_DATA_SEND_RF_MAX_TIMOUT);======================> for testing
      if(!tempBool)
      {
        //Goi tin hieu loi len App va bat loa bao hieu
        sprintf(dataBTConfirm,"*wd,%d,#",APP_DATA_SEND_UNSUCCESS_FROM_DRONE);
        SerialBT.write((uint8_t*)dataBTConfirm, strlen(dataBTConfirm));
        digitalWrite(loa_pin, HIGH); 
        delay(10);
        break;
      }else if (indTemp >= (((int)lenTempInt)-1))
      {
        //Goi tin hieu thanh cong len App va bat loa bao hieu
        sprintf(dataBTConfirm,"*wd,%d,#",APP_DATA_RECEIVED_SUCCESS);
        SerialBT.write((uint8_t*)dataBTConfirm, strlen(dataBTConfirm));
        digitalWrite(loa_pin, HIGH); 
        delay(10);
        digitalWrite(loa_pin, LOW); 
        delay(10);
        break;
      }      
    }      
  }else if (preBTProcess == APP_DATA_RECEIVED_UNSUCCESS){
    // GOI TIN HIEU LOI LEN APP (APP_DATA_SEND_UNSUCCESS_FROM_APP) (3 LAN) ==> THOAT VONG LAP    
    sprintf(dataBTConfirm,"*wd,%d,#",preBTProcess);
    SerialBT.write((uint8_t*)dataBTConfirm, strlen(dataBTConfirm));
    delay(10);
  }

//  
//  if (!radio.available()) {
//    Serial.println(F("đợi dữ liệu"));
//    sendData[0] = 1;
//    for (int i = 0; i < 3; i++) {
//    SerialPort.write((uint8_t*)&sendData[i], sizeof(sendData[i]));
//    }
//    Serial.print("Tín hiệu truyền: ");
//    for (int i = 0; i < 3; i++) {
//      Serial.print(sendData[i]);
//      Serial.print(" ");
//    }
//    Serial.println();
//    Reset_RF();
//  } else {
//    sendData[0] = 0;
//    radio.read(&dataRec, sizeof(dataRec));  // Đọc dữ liệu vào mảng
//    //gửi dữ liệu nhận được sang mega bằng uart
//    //TX_UART();
//    /*//tách các thông số cần thiết và in ra Serial
//    roll = convertBytesToLong(dataRec[7], dataRec[6], dataRec[5], dataRec[4]);
//    pitch = convertBytesToLong(dataRec[11], dataRec[10], dataRec[9], dataRec[8]);
//    yaw = convertBytesToLong(dataRec[15], dataRec[14], dataRec[13], dataRec[12]);
//    Z = convertBytesToLong(dataRec[19], dataRec[18], dataRec[17], dataRec[16]);*/
//    /*Serial.print(F("roll: "));
//    Serial.print(roll, DEC);
//    Serial.print("    ");
//    Serial.print(F("pitch: "));
//    Serial.print(pitch, DEC);
//    Serial.print("    ");
//    Serial.print(F("yaw: "));
//    Serial.print(yaw, DEC);
//    Serial.print("    ");
//    Serial.print(F("Z: "));
//    Serial.println(Z, DEC);*/
//
//    //Xuất dữ liệu ra Serial Monitor
//    Serial.print(F("Nhận được dữ liệu: "));
//    for (int i = 0; i < sizeof(dataRec); i++) {
//      Serial.print(dataRec[i]);
//      Serial.print(" ");
//    }
//    Serial.println();
//    //gửi tín hiệu đã nhận được dữ liệu qua uart
//    for (int i = 0; i < 3; i++) {
//      SerialPort.write((uint8_t *)&sendData[i], sizeof(sendData[i]));
//    }
//    Serial.print("Tín hiệu truyền: ");
//    for (int i = 0; i < 3; i++) {
//      Serial.print(sendData[i]);
//      Serial.print(" ");
//    }
//    Serial.println();
//  }
 // Check_RF();
  // Đợi 1 giây
 // delay(100);
 //----------------GOI TIN HIEU ID BLUETOOTH QUA MEGA------------------------
 megasendCnt++;
 if(megasendCnt >= 200){
  megasendCnt = 0;
  sprintf(dataBTConfirm,"*BTID,%s#",Bluetooth_Name);
  if(Serial.availableForWrite()>= 63){
    Serial.print("Dang goi du lieu qua Mega Pro:");
    Serial.println(String(dataBTConfirm));  
  }
  sendStringDatatoMega(String(dataBTConfirm));
 }
}
