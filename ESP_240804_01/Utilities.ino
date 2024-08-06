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
