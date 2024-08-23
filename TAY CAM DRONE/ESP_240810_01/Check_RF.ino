 bool Check_RF( int expectedIndex, long timeout)
 {
    long dataRec[1];
    long preRFTime_inMilli = millis();
    long curRFTime_inMilli = preRFTime_inMilli;
    long cnt = 0;
    while(cnt < 20){
      while ((!radio.available()) && ((curRFTime_inMilli - preRFTime_inMilli)<timeout))
      {
        curRFTime_inMilli = millis();      
      }
//      if((curRFTime_inMilli - preRFTime_inMilli)>= timeout)
//      {
//        return false; //Nhan du lieu fail vi timeout
//      }else
      if(radio.available())
      {
        radio.read(&dataRec, sizeof(dataRec));  // Đọc dữ liệu vào mảng
        //Xuất dữ liệu ra Serial Monitor
        if(Serial.availableForWrite()>= 63){
        Serial.print("Nhan du lieu tu RF xuat ra May tin: ");
        Serial.print(dataRec[0]);
        }
  //      for (int i = 0; i < sizeof(dataRec); i++) {
  //        Serial.print(dataRec[i]);
  //        Serial.print(" ");
  //      }
        int idRec = (int) dataRec[0];
        if(Serial.availableForWrite()>= 63){
        Serial.print(". id: "); Serial.print(idRec); Serial.print("  ===  eid: "); Serial.println(expectedIndex);}
        if (expectedIndex == idRec)
          return true;
        else 
          cnt++;    
      }else cnt++; 
    }
    return false;
 }

 void Read_RF_test()
 {
    long dataRec[1];
    if(radio.available())
    {
      radio.read(&dataRec, sizeof(dataRec));  // Đọc dữ liệu vào mảng
      //Xuất dữ liệu ra Serial Monitor
      if(Serial.availableForWrite()>= 63)
      Serial.print("Nhan du lieu tu RF:.. ");
      for (int i = 0; i < sizeof(dataRec); i++) {
        if(Serial.availableForWrite()>= 63){
        Serial.print(dataRec[i]);Serial.print("....");Serial.print(i);
        Serial.println(" ");}
      }
      if(Serial.availableForWrite()>= 63)
      Serial.println("Done.");
    }    
 }
 
