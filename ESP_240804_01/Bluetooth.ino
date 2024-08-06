/*---------------------------------------------------
 * NHAN DU LIEU BAY TU APP:
 * NEU MODE 2: (BAY DON DIEM) --> Neu du lieu tot thi Tra ve: true, fly_mode = 2, currentIndex = 0. else: False.
 * NEU MODE 3: (BAY DA DIEM) --> Neu du lieu tot thi Tra ve: true, fly_mode = 3, currentIndex = index. else: False.
------------------------------------------------------*/
bool ReceivedDatafromBluetooth(long (*DataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long *fly_mode, long *currentIndex) {
//  float previousTime, currentTime, eT;
//  float EslapTime = 2.0; 
//  bool check = false;
  String data_rec = "";
  if (SerialBT.available()) {
    data_rec = SerialBT.readStringUntil('\n'); //Doc du lieu tu Bluetooth
    Serial.println(data_rec);
    return (extractDataandSavetoMemory(data_rec, DataMemory, currentIndex, fly_mode)); //tach du lieu va dua vao bo nho
  }else  return false; //Khong co du lieu
}

/*---------------------------------------------------
 * XU LY DU LIEU THU BLUETOOTH:
 * KHI NHAN DAY DU DU LIEU TRA VE: APP_DATA_RECEIVED_SUCCESS
 * KHI NHAN DU LIEU KHONG DAY DU SAU APP_DATA_RECEIVING_WAIT_MAX CHU KY QUET: APP_DATA_RECEIVED_UNSUCCESS
 * KHI KHONG NHAN DU LIEU TRA VE: APP_DATA_RECEIVED_NONE
 * KHI KHONG NHAN DU LIEU TRA VE: APP_DATA_RECEIVING
 * KHI DU LIEU RAC TRA VE: APP_DATA_RECEIVED_UNKNOWDATA
-----------------------------------------------------*/
byte ProcessDataFromBluetooth(long (*DataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long *fly_mode, long *currentIndex, byte preProcess, int *wait_cnt)
{
  int cnt = *wait_cnt;
  long temp1 = 0, temp2 = 0;
  if(ReceivedDatafromBluetooth(DataMemory, fly_mode, currentIndex)) //Co nhan du lieu
  {
    if((*fly_mode) == FLY_MODE_SINGLE_POINT)
    {
      //*wait_cnt = 0;
      return APP_DATA_RECEIVED_SUCCESS;
    }else if((*fly_mode) == FLY_MODE_SMULTIPLE_POINT)
    {
      temp1 = *currentIndex;
      if((temp1 >= 0)&&(temp1 < POINT_MEMORY_INDEX_LONG_ALL))
      {
        temp2 = DataMemory[temp1][POINT_MEMORY_INDEX_LEN];
        if(temp1 < (temp2 -1)) //dang cho nhan du lieu tiep
          return APP_DATA_RECEIVING;
        else if (temp1 == (temp2 -1)) //da nhan xong du lieu
        {
          //*wait_cnt = 0;
          return APP_DATA_RECEIVED_SUCCESS;
         }
        else return APP_DATA_RECEIVED_UNKNOWDATA; //Loi du lieu khong biet          
      }else return APP_DATA_RECEIVED_UNKNOWDATA; //Loi du lieu khong biet
    } else return APP_DATA_RECEIVED_UNKNOWDATA; //Loi du lieu khong biet
    
  }else //Khong nhan du lieu
  {
    if(preProcess == APP_DATA_RECEIVING)//Dang trong qua trinh nhan du lieu ---> tang xung dem len 1, neu vuot qua gioi han cho tra ve APP_DATA_RECEIVED_UNSUCCESS
    {
      cnt++;
      if(cnt >= APP_DATA_RECEIVING_WAIT_MAX)
      {
        cnt = APP_DATA_RECEIVING_WAIT_MAX;
        return APP_DATA_RECEIVED_UNSUCCESS;
      }else
      {
       *wait_cnt = cnt;
       return APP_DATA_RECEIVING;
      }
    }else // Tra ve APP_DATA_RECEIVED_NONE
    {
      return APP_DATA_RECEIVED_NONE;
    }    
  }
}


//bool ReceivedData(long *fly_mode, long *currentIndex) {
//  float previousTime, currentTime, eT;
//  float EslapTime = 2.0; 
//  bool check = false;
//  if (SerialBT.available()) {
//    data_rec = SerialBT.readStringUntil('\n');
//    Serial.print("Bluetooth_'0': ");
//    Serial.println(data_rec);
//
//    // Đợi trong vòng 2 giây
//    previousTime = micros();
//    currentTime = micros(); 
//    eT = (currentTime - previousTime) / 1000000.0; 
//    while (eT < EslapTime) {
//      currentTime = micros(); 
//      eT = (currentTime - previousTime) / 1000000.0;
//      if (SerialBT.available()) {
//        data_rec = SerialBT.readStringUntil('\n');
//        Serial.print("Bluetooth_'1': ");
//        Serial.println(data_rec);
//        check = true;
//        break;
//      }
//    }
//    if(!check)
//    {
//      char AA1[] = "*wd,1,#";
//    SerialBT.write((uint8_t*)AA1, strlen(AA1));
//    }
//
//    check = false;
//    // Đợi trong vòng 2 giây
//    previousTime = micros();
//    currentTime = micros(); 
//    eT = (currentTime - previousTime) / 1000000.0; 
//    while (eT < EslapTime) {
//      currentTime = micros(); 
//      eT = (currentTime - previousTime) / 1000000.0;
//      if (SerialBT.available()) {
//        data_rec = SerialBT.readStringUntil('\n');
//        Serial.print("Bluetooth_'2': ");
//        Serial.println(data_rec);
//        check = true;
//        break;
//      }
//    }
//    if(!check)
//    {
//      char AA2[] = "*wd,1,#";
//    SerialBT.write((uint8_t*)AA2, strlen(AA2));
//    }
//
//    char AA3[] = "*wd,0,#";
//    SerialBT.write((uint8_t*)AA3, strlen(AA3));
//
//  } else {
//    delay(20);
//  }
//}

void ReceivedData_old ()
{
  if (SerialBT.available()){
  String data_rec_0 = SerialBT.readStringUntil('\n');
  Serial.print("Bluetooth_0: ");
  Serial.println(data_rec_0);
  String data_rec_1 = SerialBT.readStringUntil('\n');
  Serial.print("Bluetooth_1: ");
  Serial.println(data_rec_1);
  String data_rec_2 = SerialBT.readStringUntil('\n');
  Serial.print("Bluetooth_2: ");
  Serial.println(data_rec_2);
  char AA[] ="*wd,0,#";
  SerialBT.write((uint8_t*)AA, strlen(AA));
  }
  else
  {
    char AA[] ="*wd,1,#";
    SerialBT.write((uint8_t*)AA, strlen(AA));
  }
}

bool CheckData(String data, char i){
  bool check = false;
    if ((data.charAt(data.length()-2) == i)){
      check = true;
   }
   else{
      check = false;
   }
  return check;
}
