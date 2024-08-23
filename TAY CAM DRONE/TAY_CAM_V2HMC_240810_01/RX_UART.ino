void RX_UART()
{
  String dataRec;
  if(Serial1.available()){//Kiem tra du co du lieu tu ESP32 goi qua hay ko
    dataRec = Serial1.readStringUntil('\n');
    Serial.println(dataRec);
    
  }
  
//    if(Serial1.available())
//  {
//    for (int i = 0; i < 3; i++) {
//      Serial1.readBytes(&mang[i], sizeof(mang[i]));
//       }
//  }
//  if(mang[0] == 0)
//  {
//    digitalWrite(LedRF_ESP, HIGH);
//  }
//  else digitalWrite(LedRF_ESP, LOW);
//  Serial.print("mang: ");
//  for(int i = 0; i < 3; i++)
//  {
//    Serial.print(mang[i]);
//    Serial.print(" ");
//  }
//  Serial.println();
}
