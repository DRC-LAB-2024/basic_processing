 bool Check_RF( int expectedIndex, long timeout)
 {
    byte dataRec[55];
    long preRFTime_inMilli = millis();
    long curRFTime_inMilli = preRFTime_inMilli;
    while ((!radio.available()) && ((curRFTime_inMilli - preRFTime_inMilli)<timeout))
    {
      curRFTime_inMilli = millis();      
    }
    if((curRFTime_inMilli - preRFTime_inMilli)>= timeout)
    {
      return false; //Nhan du lieu fail vi timeout
    }else{
      radio.read(&dataRec, sizeof(dataRec));  // Đọc dữ liệu vào mảng
      //Xuất dữ liệu ra Serial Monitor
      Serial.print(F("Nhan du lieu tu RF xuat ra May tin: "));
      for (int i = 0; i < sizeof(dataRec); i++) {
        Serial.print(dataRec[i]);
        Serial.print(" ");
      }
      int idRec = (int) dataRec[0];
      return (expectedIndex == idRec);      
    }
 }
 
