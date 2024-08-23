/*-------------------------------------------
HAM DOI LINH HOAT
--------------------------------------------*/
long softDelay(long preTime, float Ts)
{
  //10m he thong se xu ly mot lan
//  long previousTime = currentTime;
//  previousTime = currentTime;
  long eTime = 0;
  float eT = 0;
  long cTime = (long) (micros()); 
  eTime = cTime - preTime;
  eT = ((float)eTime)/1000;
  while(eT < Ts){
    cTime = (long) (micros()); 
    eTime = cTime - preTime;
    eT = ((float)eTime)/1000;
  }
  return cTime;
}
/*----------------------------------------------
TRICH DU LIEU TU BLUETOOTH VA LUU VAO BO NHO
------------------------------------------------*/
bool extractAPPDataandSavetoMemory(String in, long (*dataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long *preIndex, long *modeRead) 
{
  //inString format: "*3,lat, long, z, time, len, index#"
  //dataMemory: kieubay, lat, long,z,time, len, index;
  String inString = in, data_sub = "";
  uint16_t dataCnt = 0;
  unsigned int len = 0;
  int pos = 0;
  long Rec_in[POINT_MEMORY_INDEX_LEN_ALL]; 
  long index_data = 0;
  long pIndex = *preIndex;
  long checksumTemp1 = 0;
  char delinote = ',';
  
  
  inString.trim(); //Bo cac ky tu ko can thiet truoc va sau chuoi
  len = inString.length();
  if(CheckValidAPPRecData(inString, APP_DATA_HEADER, APP_DATA_FOOTER)){ //Kiem tra header va footer
    data_sub = inString.substring(1,len-1); //Trich du lieu chinh
    dataCnt = SplitData2Long(data_sub, Rec_in, POINT_MEMORY_INDEX_LEN_ALL, delinote);//SplitData2Long(data_sub, Rec_in, POINT_MEMORY_INDEX_LEN_ALL,','); //Tach chuoi du lieu va chuyen thanh kieu LONG
    if((dataCnt <= POINT_MEMORY_INDEX_LEN_ALL) && (dataCnt > 0)) //So du lieu nhan phu hop
    {
      checksumTemp1 = ChecksumOfLongArray(Rec_in, dataCnt-1);
      if(Serial.availableForWrite()>= 63){
        Serial.print("Check sum old:"); Serial.println(Rec_in[POINT_MEMORY_INDEX_CHECKSUM]);
        Serial.print("Check sum new:"); Serial.println(checksumTemp1);
      }
      if((checksumTemp1 == Rec_in[POINT_MEMORY_INDEX_CHECKSUM])&& (checksumTemp1 > 0)){ //Kiem tra CHECKSUM
        index_data = Rec_in[POINT_MEMORY_INDEX_INDEX];
        if((index_data == 0)||((index_data >0)&&(index_data < Rec_in[POINT_MEMORY_INDEX_LEN])&&(index_data == (pIndex+1))&&(index_data < POINT_MEMORY_INDEX_LONG_ALL))) //Kiem tra index thoa man
        {
          for(pos = 0; pos < POINT_MEMORY_INDEX_LEN_ALL; pos++){
            dataMemory[index_data][pos] = Rec_in[pos];
          }
          *modeRead = Rec_in[POINT_MEMORY_INDEX_MODE];
          *preIndex = index_data;
          if(Serial.availableForWrite()>= 63){
            Serial.print("Mode:"); Serial.println(Rec_in[POINT_MEMORY_INDEX_MODE]);
            Serial.print("current Index:"); Serial.println(index_data);
          }
          return true;
         }else //Loi goi sai thu tu du lieu
         return false;
      }else//Loi CHECKSUM
        return false;
    }else //Loi du phan tach du lieu
      return false;

  }else //Loi header hoac/va footer
    return false; 

}
/*----------------------------------------------
TRICH DU LIEU TU BLUETOOTH VA LUU VAO BO NHO
------------------------------------------------*/
bool extractDataandSavetoMemory(String in, long (*dataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long *preIndex, long *modeRead) 
{
  //inString format: "*3,lat, long, z, time, len, index#"
  //dataMemory: kieubay, lat, long,z,time, len, index;
  String inString = in, data_sub = "";
  unsigned int len = 0;
  int pos = 0;
  long temp_num = 0, lat_data, lon_data, mode_data, z_data, time_data, len_data, index_data, nextIndex = 0;
  long pIndex = *preIndex;
  long checksumTemp1 = 0, dataTemp[POINT_MEMORY_INDEX_LEN_ALL];
  
  inString.trim(); //Bo cac ky tu ko can thiet truoc va sau chuoi
  len = inString.length();
  
  //---------Kiem tra va trich du lieu-------------------------
  if(len > 3) //Do dai chuoi hop le
  {
    if((inString[0] == '*') && (inString[len-1] == '#')) //Kiem tra header and footer: * and #
    {
      inString.remove(0, 1); //xoa ky tu '*'
      if(inString[0] == '3') //kieu du lieu bay da diem
      {
        mode_data = 3;
      } else if(inString[0] == '2') //du lieu bay 1 diem
      {
        mode_data = 2;       
      } else //Loi du lieu khac
      return false;
      inString.remove(0, 2); //xoa ky tu '2(3),'
      //-----DOC DU LIEU LAT
      pos=inString.indexOf(',');//Tim dau phay
      if(pos > 0) //Neu co thay dau phay
      {
        data_sub = inString.substring(0,pos); //Lay du lieu lat dang string
        lat_data = convertString2Long(data_sub);//Chuyen tu string sang long
        inString.remove(0, pos+1); //xoa ky tu 'lat,'
        //-----DOC DU LIEU LON
        pos=inString.indexOf(',');//Tim dau phay
        if(pos > 0) //Neu co thay dau phay
        {
          data_sub = "";
          data_sub = inString.substring(0,pos); //Lay du lieu lon dang string
          lon_data = convertString2Long(data_sub);//Chuyen tu string sang long
          inString.remove(0, pos+1); //xoa ky tu 'lon,'
          //-----DOC DU LIEU z
          pos=inString.indexOf(',');//Tim dau phay
          if(pos > 0) //Neu co thay dau phay
          {
            data_sub = "";
            data_sub = inString.substring(0,pos); //Lay du lieu Z dang string
            z_data = convertString2Long(data_sub);//Chuyen tu string sang long
            inString.remove(0, pos+1); //xoa ky tu 'z,'
            //-----DOC DU LIEU time
            pos=inString.indexOf(',');//Tim dau phay
            if(pos > 0) //Neu co thay dau phay
            {
              data_sub = "";
              data_sub = inString.substring(0,pos); //Lay du lieu Z dang string
              time_data = convertString2Long(data_sub);//Chuyen tu string sang long
              inString.remove(0, pos+1); //xoa ky tu 'time,'
              //-----DOC DU LIEU len
              pos=inString.indexOf(',');//Tim dau phay
              if(pos > 0) //Neu co thay dau phay
              {
                data_sub = "";
                data_sub = inString.substring(0,pos); //Lay du lieu Z dang string
                len_data = convertString2Long(data_sub);//Chuyen tu string sang long
                inString.remove(0, pos+1); //xoa ky tu 'len,'
                //-----DOC DU LIEU index
                pos=inString.indexOf('#');//Tim dau #
                if((pos > 0)&&(len_data >= 0)&&(len_data < POINT_MEMORY_INDEX_LONG_ALL)) //Neu co thay dau phay
                {
                  data_sub = "";
                  data_sub = inString.substring(0,pos); //Lay du lieu Z dang string
                  index_data = convertString2Long(data_sub);//Chuyen tu string sang long
                  //Kiem tra loi index
                  
                  if(checkValidIndexData(index_data,len_data, pIndex,  mode_data, &nextIndex)){
                    //------ GAN CAC DU LIEU VAO BO NHO------------
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_MODE] = mode_data; //Luu mode
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_LAT] = lat_data; //Luu lat
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_LONG] = lon_data; //Luu lon
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_Z] = z_data; //Luu z
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_TIME] = time_data; //Luu time
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_LEN] = len_data; //Luu len
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_INDEX] = index_data; //Luu index
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_CHECKSUM] = 0; //Luu index

                    for(int j = 0; j < POINT_MEMORY_INDEX_LEN_ALL; j++)
                    {
                      dataTemp[j] = dataMemory[nextIndex][j];
                    }
                    checksumTemp1 = ChecksumOfLongArray(dataTemp, POINT_MEMORY_INDEX_LEN_ALL);
                    dataMemory[nextIndex][POINT_MEMORY_INDEX_CHECKSUM] = checksumTemp1; //Luu index
                    //--- luu du lieu
                    *modeRead = mode_data;
                    *preIndex = nextIndex;
                    return true;
                  }                  
                  else     //Loi du lieu index
                  return false;
                
                }else    //Loi khong thay dau '#' sau du lieu index
                return false;
                
              }else    //Loi khong thay dau phay sau du lieu len
              return false;
              
            }else    //Loi khong thay dau phay sau du lieu time
            return false;
            
            
          }else    //Loi khong thay dau phay sau du lieu Z
          return false;
          
        }else    //Loi khong thay dau phay sau du lieu LON
        return false;
      
        
      }else    //Loi khong thay dau phay sau du lieu lat
      return false;
      
    } else //Header footer khong hop le
    return false;
    
  }else  //Do dai chuoi khong hop le
  return false; 
  
}

/*--------------------------------------------------------------
 * HAM KIEM TRA LOI INDEX ---> RETURN TRUE/FALSE
----------------------------------------------------------------*/
bool checkValidIndexData(long inIndex,long len, long preIndex,  long modeData, long *nextIndex)
{
  //--Index trong tam du lieu
  if((inIndex >= 0) && (inIndex<len)){
    if(modeData == 2){ //Mode 1 diem ---> index phai bang 0
      if(inIndex == 0){
        *nextIndex = 0;
        return true;
      } else
      return false;
    }else if(modeData == 3) //Mode da diem ---> index == 0 hoac index == (preindex + 1)
    {
      if(inIndex == 0){
        *nextIndex = 0;
        return true;
      } else{
        if((inIndex - preIndex)==1) //du lieu ok
        {
          *nextIndex = inIndex;
          return true;
          
        }else//loi sai thu tu
        return false;         
      }
      
    }else //chua ho tro cac mode khac
    return false; 
    
    
  }else //loi ngoai tam index
  return false;
}

/*-----------------------------------------------------------
 * ------CONVERT STRING TO LONG: "123678.677" ->123678
---------------------------------------------------------------*/
long convertString2Long(String in)
{
  String string_in = in, data_temp = "";
  int len = string_in.length();
  
  long out = 0, temp2 = 0;
  bool result = true;
  char temp_Data;
  byte temp_byte = 0;
  string_in.trim();
  while(len > 0)
  {
    temp_Data = string_in[0];//Trich ky tu dau tien
    result = convertChar2Byte(temp_Data, &temp_byte);//chuyen sang so kieu byte
    if(result)
    {
      temp2 = (long)temp_byte; //chuyen kieu
      out = out*10 + temp2; //tong hop du lieu
      string_in.remove(0, 1); //xoa ky tu dau tien
      len = string_in.length(); //tinh lai chieu dai chuoi
    }else
      break;
    
//    data_temp = string_in.substring(0,1);
//    temp1 = data_temp.toInt();
//    temp2 = (long)temp1;
//    out = out*10 + temp2;

//    Serial.println(data);
//    Serial.println(temp1);
//    Serial.println(temp2);
//    Serial.println(len);
//    Serial.println(out);
//    Serial.println(".......");

  }  
  return out;
}

/*-----------------------------------------------------------
 * ------CONVERT STRING TO LONG: "123678.677" ->123678
---------------------------------------------------------------*/
bool convertChar2Byte(char in_char, byte *out_num)
{
  if((in_char >= 48) && (in_char <= 57)) //48 -->0; 57--->9
  {
    *out_num = in_char - 48;
    return true;    
  }else return false;
}

/*-----------------------------------------
 * print String
--------------------------------------------*/
String printString(String header, int num_in, String tail)
{
  return (header+String(num_in)+tail);
}

/*---------------------------------------------------
 * CHECKSUM OF LONG VARIALBE
-----------------------------------------------------*/
long ChecksumOfLong(const long data){
  byte byteData[4] = {0,0,0,0};
  byteData[0] = (data >> 24) & 0xFF;
  byteData[1] = (data >> 16) & 0xFF;
  byteData[2] = (data >> 8) & 0xFF;
  byteData[3] = data & 0xFF;
  return ((long)byteData[0] + (long)byteData[1] + (long)byteData[2] + (long)byteData[3]);
}
/*---------------------------------------------------
 * CHECKSUM OF LONG ARRAY
-----------------------------------------------------*/
long ChecksumOfLongArray(const long *data_in, int len){
  long out = 0, temp1;
  for (int i = 0; i < len;i++)
  {
    temp1 = data_in[i];
    out += ChecksumOfLong(temp1);
  }
  return out;
}
/*---------------------------------------------------
 * CHECK VALID DATA OR NOT
-----------------------------------------------------*/
bool CheckValidAPPRecData(String data_in, char fchar, char echar){
  String inString = data_in;
  unsigned int len = 0;
  len = inString.length();
  if(len > 2){ //Do dai phu hop
    return ((inString[0] == fchar) && (inString[len-1] == echar));
  }else return false; //Do dai khong phu hop  
}
/*

hàm SplitData:
*3,10155555,1071060606,100,10,5,0#  tiến hành tách từng dữ liệu sau dấu phẩy lưu vào 1 mảng:
Btmode,Lat,long,Z,t,len_p,stt -> trong đó len_p: số lượng điểm, stt: số thự tự của điểm ví dự điểm thứ nhất, điểm thứ hai


*/
uint16_t SplitData2Long(String data_in, long Arr[], uint16_t arrLen, char delimiter){
    char in[data_in.length()];
    char *_data;
    char *token;
    uint16_t i = 0;
    uint16_t len = data_in.length();
    for (int j = 0; j < len; j++)
      in[j] = data_in[j];
    _data = in; //data_in.c_str();
    token = strtok(_data,&delimiter);
    while(token!=NULL){
      //Arr[i] = atol(token);
      if(i < arrLen){
        Arr[i] = atol(token);
//        Arr[i] = convertString2Long(String(token));
        token = strtok(NULL,&delimiter);
        i++;
      }else break;
    } 
    return i;   
}
/*--------------------------------------------
 * PHAT AM THANH RA LOA
----------------------------------------------*/
void panelReact(uint16_t pin_name, int in_num)
{
  if(in_num > 0){
    for (int i = 0; i < in_num; i++){
      digitalWrite(pin_name, HIGH); 
      delay(100);
      digitalWrite(pin_name, LOW); 
      delay(100);
    }      
  }
}

void LedReact(uint16_t pin_name, int in_num)
{
  if(in_num > 0){
    for (int i = 0; i < in_num; i++){
      digitalWrite(pin_name, HIGH); 
      delay(200);
      digitalWrite(pin_name, LOW); 
      delay(200);
    }      
  }
}
