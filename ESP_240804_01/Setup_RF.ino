void Setup_RF()
{
  hspi = new SPIClass(HSPI);
  hspi->begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);
  
  // Khởi động radio và kiểm tra nếu không thành công thì in lỗi ra Serial Monitor
  if (!radio.begin(hspi)) {          
    while (!radio.begin(hspi)) {
      Serial.println(F("khởi động radio thất bại"));
      sendData[0] = 1;
      for (int i = 0; i < 3; i++) {
      MegaPort.write((uint8_t*)&sendData[i], sizeof(sendData[i]));
      }
      
      Serial.print("Tín hiệu truyền: ");
      for (int i = 0; i < 3; i++) {
      Serial.print(sendData[i]);
      Serial.print(" ");
      }
      Serial.println();
    } // Lặp vô hạn khi có lỗi
  }

  // Đặt địa chỉ pipe
  radio.openReadingPipe(1, 0xe8e8f0f0e1);
  // radio.openReadingPipe(1,0xf1f1f0f0e0);
  radio.setChannel(0x77); 
  radio.setPALevel(RF24_PA_MIN); // Đặt mức năng lượng phát sóng
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();  
}
