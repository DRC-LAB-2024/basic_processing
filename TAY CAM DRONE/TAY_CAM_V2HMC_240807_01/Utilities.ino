//float read_roll_scale()
//{
//  long scale = analogRead(rollscale_pin);
//  //*rscale = scale;
//  long scalevalue = map(scale, 0, 1023, rollscale_MIN, rollscale_MAX);
//  return ((float)scalevalue);  
//}
//
//
//float read_pitch_scale()
//{
//  long scale = analogRead(pitchscale_pin);
//  //*rscale = scale;
//  long scalevalue = map(scale, 0, 1023, pitchscale_MIN, pitchscale_MAX);
//  return ((float)scalevalue);  
//}

long read_scale_value(unsigned int pin_name, long val_min, long val_max)
{
  long scale = analogRead(pin_name);  
  return (map(scale, 0, 1023, val_min, val_max));   
}

/*---------------------------------------------------
 * READ DATA WITH PREFIX: String result and bool result
-----------------------------------------------------*/
String extractDataFromPrefix(String in, String prefix, bool *result)
{
  String inString = in, outString;
  unsigned int len_prefix = 0, len_inString = 0;
  //check header -- footer
  if(CheckValidUARTRecData(inString, UART_DATA_HEADER, UART_DATA_FOOTER))
  {
    if(inString.startsWith(prefix)) {
      //Remove Prefix and footer
      len_prefix = prefix.length();
      len_inString = inString.length();
      if(len_inString > (len_prefix+1)){ //Ton tai du lieu
        *result = true;
        outString = inString.substring(len_prefix+1, (len_inString -1));
        
      }
      else{ //Khong ton tai du lieu
        *result = false;
      } 
    }else{ //Prefix khong dung
      *result = false;
    } 
  }else{ //Dinh dang du lieu khong dung
    *result = false;
  } 
  return outString; 
}
/*---------------------------------------------------
 * CHECK VALID DATA OR NOT
-----------------------------------------------------*/
bool CheckValidUARTRecData(String data_in, char fchar, char echar){
  String inString = data_in;
  unsigned int len = 0;
  len = inString.length();
  if(len > 2){ //Do dai phu hop
    return ((inString[0] == fchar) && (inString[len-1] == echar));
  }else return false; //Do dai khong phu hop  
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
