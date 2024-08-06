/*
hàm bool CheckData
Kiểm tra tín hiệu gửi về App có chuẩn ký tự đầu ra không ví dụ:
*3,10155555,1071060606,100,10,5,0# -> gồm ký tự bắt đầu và ký tự kết thúc, ký tự có lỗi hay không nếu không return True
'⸮bb⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮b⸮⸮⸮R' -> chuỗi lỗi return false


*/
bool CheckData(String data, char fchar=' ', char echar = ' '){
  bool check = false;
  if ( (data.charAt(0) == fchar) && (data.charAt(data.length()-1) == echar)  && (data.length() > 0) ){
      check = true;
      for (int i=0; i< data.length(); i++){
        char c = data.charAt(i);
        if(!isPrintable(c)){
            check = false;
            break;
        }
      }    
   }
   else{
      check = false;
   }
  return check;
}
/*

hàm SplitData:
*3,10155555,1071060606,100,10,5,0#  tiến hành tách từng dữ liệu sau dấu phẩy lưu vào 1 mảng:
Btmode,Lat,long,Z,t,len_p,stt -> trong đó len_p: số lượng điểm, stt: số thự tự của điểm ví dự điểm thứ nhất, điểm thứ hai


*/
uint8_t SplitData(String data, long Arr[], char delimiter = ' '){
    char *_data = data.c_str();
    char *token;
    uint8_t i = 0;
    token = strtok(_data,&delimiter);
    while(token!=NULL){
      Arr[i] = atol(token);
      token = strtok(NULL,&delimiter);
      i++;
    } 
    return i;   
}
/*
 hàm ReceivedData
 Nhận dữ liệu từ App gửi về và tiến hành xử lý nếu dữ liệu lỗi thì gửi lại dữ liệu thông báo cho App
 biết xử lý.

*/
bool ReceivedDataFromESP32(uint8_t *BtMode_in, long *Rec_in){
  String DataRecApp_in, readStringOut;
  uint8_t dataCnt = 0;
  bool readDataResult = false;
  long checksumTemp = 0;
  if(SerEsp.available()){    
    //    SerEsp.println(CheckD_in);
    if((BtMode_in == MANUALMODE_POS_SEND)||(BtMode_in==MANUALMODE_ANGLE_SEND))//Khong xu ly du lieu tu ESP32 trong mode MANUAL
    {
      return false;
    }else{
      DataRecApp_in = SerEsp.readStringUntil('\n');//DINH DANG CHUOI NHAN: {*btmode,lat,long,z,time,length, index, checksum#}
      DataRecApp_in.trim();
      if(Serial.availableForWrite()>=63){
        Serial.print("Data Received: ");
        Serial.println(DataRecApp_in);
      }
      if(CheckValidUARTRecData(DataRecApp_in, UART_DATA_HEADER, UART_DATA_FOOTER)){ //Du lieu bay
        if((DataRecApp_in.startsWith("*2"))||(DataRecApp_in.startsWith("*3"))){
          String DataRecApp_in1 = DataRecApp_in.substring(1,DataRecApp_in.length()-1);
          if(Serial.availableForWrite()>=63){
          Serial.print("MAIN DATA: ");
          Serial.println(DataRecApp_in1);
          }
          dataCnt = SplitData(DataRecApp_in1, Rec_in, ',');
//          Serial.println(dataCnt);
//          for (int i = 0; i < POINT_MEMORY_INDEX_LEN_ALL; i++)
//            Serial.println(Rec_in[i]);
          if((dataCnt <= POINT_MEMORY_INDEX_LEN_ALL) && (dataCnt > 0)) //So du lieu nhan phu hop
          {
            checksumTemp = ChecksumOfLongArray(Rec_in, dataCnt-1);
            if(checksumTemp == Rec_in[POINT_MEMORY_INDEX_CHECKSUM]) //Du lieu nhan Ok
            {
                //goi qua drone
                
            }           
//            Serial.print("Checksum: ");
//            Serial.println(checksumTemp);
          }
          *BtMode_in = Rec_in[0]; //Bt Mode
          return true;
        }else{
          //Doc du lieu Bluetooth ID
          readStringOut = extractDataFromPrefix(DataRecApp_in, (String("*BTID")), &readDataResult);
          if(readDataResult)
          {
            ESP32_BLUETOOTHID = readStringOut;
            Serial.print("ID: ");
            Serial.println(ESP32_BLUETOOTHID);
          }
          return false;
        }
      }
      return false;
    }
  }else return false; //Tra ve mode cu
}
  
