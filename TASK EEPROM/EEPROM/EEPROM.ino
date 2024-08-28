#include <EEPROM.h>

#define EEPROM_SIZE 40

void WriteStringToEEPROM(const char* str, int len, int addr) {
  EEPROM.begin(EEPROM_SIZE); //lệnh này chỉ dùng cho esp32 để giả lập ROM vì esp32 không có ROM, nếu dùng cho arduino mega thì không cần
  // Serial.print("Writing string: ");
  // Serial.print(str);
  // Serial.print(" at address: ");
  // Serial.println(addr);

  // Ghi dữ liệu vào EEPROM
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + i, str[i]);
  }

  // Tính và ghi checksum
  byte checksum = 0;
  for (int i = 0; i < len; i++) {
    checksum ^= str[i];
  }
  EEPROM.write(addr + len, checksum); // Lưu checksum sau chuỗi

  EEPROM.commit(); //lệnh này chỉ dùng cho esp32 để giả lập ROM vì esp32 không có ROM, nếu dùng cho arduino mega thì không cần
}

bool ReadStringFromEEPROM(char* data, int len, int addr) {
  EEPROM.begin(EEPROM_SIZE); //lệnh này chỉ dùng cho esp32 để giả lập ROM vì esp32 không có ROM, nếu dùng cho arduino mega thì không cần
  // Đọc dữ liệu từ EEPROM
  for (int i = 0; i < len; i++) {
    data[i] = EEPROM.read(addr + i);
  }
  data[len] = '\0'; 

  // Đọc và kiểm tra checksum
  byte checksum_old = EEPROM.read(addr + len);
  byte checksum_new = 0;
  for (int i = 0; i < len; i++){
    checksum_new ^= data[i];
  }
  
  return (checksum_old == checksum_new);
}

const char chuoi[] = "chay ngay di!!!!!";
int len = sizeof(chuoi) - 1;
int addr = 0;
char data[sizeof(chuoi)];

void setup() {
  Serial.begin(115200);
  WriteStringToEEPROM(chuoi, len, addr);
}

void loop() {
  if(ReadStringFromEEPROM(data, len, addr)) {
    Serial.print("Read string: ");
    Serial.println(data);
  }
  else {
    Serial.println("Read failed");
  }
}
