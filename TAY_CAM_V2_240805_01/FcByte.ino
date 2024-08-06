
// Covert 1 byte thành 1 long byte cho trường hợp 'long' đầu
long ConvertByteStart( byte bt_mode,byte bt_state, byte byte_start, byte byte_mid){
  uint16_t state = bt_mode*10 + bt_state ;
  long Value = ((long)state << 24) | ((long)byte_mid <<8) | (long)byte_start ;
  return Value;
}


// Convert 4 Byte thành 1 long cho trường hợp chuỗi dữ liệu của pump
long ConvertBytePump(byte byte_len, byte byte_stt, byte byte_flow, byte byte_pump_state){

  long Value = ((long) byte_pump_state << 24) | ((long) byte_flow << 16) | ((long) byte_stt <<8) | ((long) byte_len) ;
  return Value ;
  
}

// Convert giá trị long sang 4 Byte để tiến hành gửi sang Drone
void ConvertLongToByte(byte ByteStart, byte ByteStop, byte Data[], long DataCon){
  for (int i = ByteStart; i < (ByteStop+1); i++){
    Data[i] = (byte)(DataCon >> ((i- ByteStart))*8);
  }
}

// Check tổng dữ liệu gửi đii
long CheckSumByte(byte Data[]){
  long sum = 0;
  for(int i=0; i < 24; i++){
      sum+= Data[i];
  }
  return sum ;
}
