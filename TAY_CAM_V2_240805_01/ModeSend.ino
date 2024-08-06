
void SendManualData2Drone(byte BtMode_in, byte preBtState_in,byte BtState_in, byte ByteStart_in, byte ByteID_in, long Dx_in,long Dy_in,long YawDes_in, long zDes_in,byte ByteLen_in, byte ByteStt_in, byte ByteFlow_in, byte BytePumpState_in,byte *Send_in){
  long ConvertByteSta = 0;
  byte BtState_in_temp = BtState_in;
  if((BtState_in == DKBTN_VALUE_START) && (preBtState_in==BtState_in)) //Chi cho phep goi start 1 lan
    BtState_in_temp = DKBTN_VALUE_DONOTHING;
  ConvertByteSta = ConvertByteStart(BtMode_in,BtState_in_temp, ByteStart_in,ByteID_in); //GHEP 4 BYTE DK DAU THANH DANG LONG
  ConvertLongToByte(0,3,Send_in,ConvertByteSta); //TACH LONG DK THANH 4 BYTE TRUYEN DAU TIEN
  ConvertLongToByte(4,7,Send_in,Dx_in);
  ConvertLongToByte(8,11,Send_in,Dy_in);
  ConvertLongToByte(12,15,Send_in,YawDes_in);
  ConvertLongToByte(16,19,Send_in,zDes_in);
  long ConvertByteP = ConvertBytePump(ByteLen_in, ByteStt_in, ByteFlow_in,BytePumpState_in);
  Send_in[20] = ByteLen_in;
  Send_in[21] = ByteStt_in;
  Send_in[22] = ByteFlow_in;
  Send_in[23] = BytePumpState_in;
  long CheckSum_in = CheckSumByte(Send_in);
  
  ConvertLongToByte(24,27,Send_in,CheckSum_in);
}

void SendAuto1PData2Drone(byte BtMode_in, byte preBtState_in,byte BtState_in, byte ByteStart_in, byte ByteID_in,long *Rec_in,byte ByteLen_in, byte ByteStt_in, byte ByteFlow_in, byte BytePumpState_in,long *Recin, byte *Send_in){
  long ConvertByteSta = ConvertByteStart(BtMode_in,BtState_in, ByteStart_in,ByteID_in); //GHEP 4 BYTE DK DAU THANH DANG LONG
  ConvertLongToByte(0,3,Send_in,ConvertByteSta); //TACH LONG DK THANH 4 BYTE TRUYEN DAU TIEN
  ConvertLongToByte(4,7,Send_in,Rec_in[1]); //lat
  ConvertLongToByte(8,11,Send_in,Rec_in[2]); //long
  ConvertLongToByte(12,15,Send_in,Rec_in[3]); //z
  ConvertLongToByte(16,19,Send_in,Rec_in[4]); //time 
  long ConvertByteP = ConvertBytePump(ByteLen_in, ByteStt_in, ByteFlow_in,BytePumpState_in);
  Send_in[20] = ByteLen_in;
  Send_in[21] = ByteStt_in;
  Send_in[22] = ByteFlow_in;
  Send_in[23] = BytePumpState_in;
  long CheckSum_in = CheckSumByte(Send_in);
  
  ConvertLongToByte(24,27,Send_in,CheckSum_in);
}


void SendAutoMutiPData2Drone(byte BtMode_in, byte preBtState_in, byte BtState_in, byte ByteStart_in, byte ByteID_in,long *Rec_in, byte ByteFlow_in, byte BytePumpState_in, byte *Send_in){
  long ConvertByteSta = 0;
  byte BtState_in_temp = BtState_in;
  if((BtState_in == DKBTN_VALUE_START) && (preBtState_in==BtState_in)) //Chi cho phep goi start 1 lan
    BtState_in_temp = DKBTN_VALUE_DONOTHING;
  ConvertByteSta = ConvertByteStart(BtMode_in,BtState_in_temp, ByteStart_in,ByteID_in); //GHEP 4 BYTE DK DAU THANH DANG LONG
  ConvertLongToByte(0,3,Send_in,ConvertByteSta); //TACH LONG DK THANH 4 BYTE TRUYEN DAU TIEN
  ConvertLongToByte(4,7,Send_in,Rec_in[POINT_MEMORY_INDEX_LAT]); //lat
  ConvertLongToByte(8,11,Send_in,Rec_in[POINT_MEMORY_INDEX_LONG]); //long
  ConvertLongToByte(12,15,Send_in,Rec_in[POINT_MEMORY_INDEX_Z]); //z
  ConvertLongToByte(16,19,Send_in,Rec_in[POINT_MEMORY_INDEX_TIME]); //time 
//  long ConvertByteP = ConvertBytePump(ByteLen_in, ByteStt_in, ByteFlow_in,BytePumpState_in);
  Send_in[20] = byte(Rec_in[POINT_MEMORY_INDEX_LEN]);   //len
  Send_in[21] = byte(Rec_in[POINT_MEMORY_INDEX_INDEX]);  //stt
  Send_in[22] = ByteFlow_in; 
  Send_in[23] = BytePumpState_in;
//  Serial.print(" Send_in[20]: "); Serial.println(Send_in[20]);
//  Serial.print(" Send_in[20]: "); Serial.println(Send_in[21]);
  //long CheckSum_in = CheckSumByte(Send_in);
  //ConvertLongToByte(24,27,Send_in,CheckSum_in);
  ConvertLongToByte(24,27,Send_in,Rec_in[POINT_MEMORY_INDEX_CHECKSUM]);
}
void SendManual(byte BtMode_in, byte BtState_in, byte ByteStart_in, byte ByteID_in, long Dx_in,long Dy_in,long YawDes_in, long zDes_in,byte ByteLen_in, byte ByteStt_in, byte ByteFlow_in, byte BytePumpState_in,byte *Send_in)
{
  long ConvertByteSta = ConvertByteStart(BtMode_in,BtState_in, ByteStart_in,ByteID_in); //GHEP 4 BYTE DK DAU THANH DANG LONG
  ConvertLongToByte(0,3,Send_in,ConvertByteSta); //TACH LONG DK THANH 4 BYTE TRUYEN DAU TIEN
  ConvertLongToByte(4,7,Send_in,Dx_in);
  ConvertLongToByte(8,11,Send_in,Dy_in);
  ConvertLongToByte(12,15,Send_in,YawDes_in);
  ConvertLongToByte(16,19,Send_in,zDes_in);
  long ConvertByteP = ConvertBytePump(ByteLen_in, ByteStt_in, ByteFlow_in,BytePumpState_in);
  Send_in[20] = ByteLen_in;
  Send_in[21] = ByteStt_in;
  Send_in[22] = ByteFlow_in;
  Send_in[23] = BytePumpState_in;
  long CheckSum_in = CheckSumByte(Send_in);
  
  ConvertLongToByte(24,27,Send_in,CheckSum_in);
}
