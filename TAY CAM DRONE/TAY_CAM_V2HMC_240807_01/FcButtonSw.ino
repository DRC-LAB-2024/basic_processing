byte ReadButton(byte in, bool *preStart, bool *preStop, bool *preEmer){
  //int StaButton = digitalRead(SwiRi); //DOC GIA TRI SWITCH EMER
  bool readLSWEmer = digitalRead(ButtonEmer_PIN), btnStart_temp, btnStop_temp = 0;
  byte BtState_in = in;
  if (readLSWEmer == HIGH) //EMER CASE 
  {
    BtState_in = DKBTN_VALUE_EMER ;
  }else
  {
    if(readLSWEmer != (*preEmer)) //EMER O MUC THAP VA CO CHUYEN TRANG THAI ==> STOP
    {
      BtState_in = DKBTN_VALUE_STOP;
    }else{
      btnStop_temp = digitalRead(ButtonStop_PIN); //DOC GIA TRI SWITCH STOP
      if(btnStop_temp != (*preStop)) //STOP CASE
      {
        BtState_in = DKBTN_VALUE_STOP;
      }else
      {
        btnStart_temp = digitalRead(ButtonStart_PIN); //DOC GIA TRI SWITCH START
        if(btnStart_temp != (*preStart)) //START CASE
        {
          BtState_in = DKBTN_VALUE_START;
        }
        *preStart = btnStart_temp; //LUU LAI TRANG THAI NUT START 
       }
       *preStop = btnStop_temp; //LUU LAI TRANG THAI NUT STOP         
    }  
  }
  *preEmer = readLSWEmer;  //LUU LAI BIEN EMER
  
  return BtState_in;

}
 
/*------------------------------------------
 * DOC SWITCH MANUAL MODE
-------------------------------------------*/
uint8_t ReadSwManualMode(uint8_t PreStateBtn){
  bool SwStaMode = digitalRead(AUTOMANUAL_MODE_PIN); //DOC TRANG THAI NUT NHAT MANUAL AUTO MODE
  uint8_t out = PreStateBtn;
// Serial.print("BtMode_in: "); 
 //Serial.println(SwStaMode);
  if(SwStaMode == HIGH) //AUTO MODE
  {
      out = ((PreStateBtn == MANUALMODE_POS_SEND)||(PreStateBtn==MANUALMODE_ANGLE_SEND))?AUTOMODE_MULTIPLEPOINT_SEND:PreStateBtn; //TRANG THAI CAO AUTO DA DIEM 
  }else
  {
    SwStaMode = digitalRead(MANUALPOS_ANL_MODE_PIN); //DOC TRANG THAI NUT NHAT MANUAL AUTO MODE
    out = SwStaMode?MANUALMODE_POS_SEND:MANUALMODE_ANGLE_SEND;
  }
  return out;
  //  if(SwStaMode) //AUTO MODE
  // {
    
  //   out = (SwStaMode == 1)?1:4; //TRANG THAI CAO --> MANUAL POSITION; THAP ---> MANUAL ANGLE    
  // }
  // return SwStaMode; //TRA VE TRANG THAI NUT NHAN MOI NHAT
}


bool LedControlStatus(int btnControlState, bool led_in){
  /*
    BtState = 1 -> Start
    BtState = 2 -> Stop
    BtState = 8 -> Emer
  */
  bool led_out = led_in;
  if (millis() - CurrentTimeLed >= 100)
  {
  if(btnControlState == 1){

    digitalWrite(LedDKC, HIGH);

  }
  else if (btnControlState == 2){
    digitalWrite(LedDKC, LOW);

  }
  else if(btnControlState == 8){
    led_out = ! led_in ;
    digitalWrite(LedDKC, led_out);

  }
  CurrentTimeLed = millis();
  }
  return led_out;
}

bool LedModeStatus(uint8_t BtMode_in, bool ledMode_in, int Led_IN){
  /*
    BtMode = 1 : Manual vi tri
    BtMode = 4: Manual goc
    BtMode = 2 | BtMode = 3 : Auto
  */
  bool ledMode_out = ledMode_in;
  if (millis() - CurrentTimeMode >= 10)
  {
    if(BtMode_in == MANUALMODE_POS_SEND){
  
      digitalWrite(Led_IN, HIGH);
      //analogWrite(Led_IN, 254); //LedO
  
    }
    else if (BtMode_in == MANUALMODE_ANGLE_SEND){
      digitalWrite(Led_IN, LOW);
      //analogWrite(Led_IN, 0); //LedO
  
    }
    else if((BtMode_in==AUTOMODE_SINGLEPOINT_SEND)||(BtMode_in == AUTOMODE_MULTIPLEPOINT_SEND)){
      ledMode_out = ! ledMode_in ;
      digitalWrite(Led_IN, ledMode_out); //LedO
      //analogWrite(Led_IN, 128); //LedO
  
    }
    CurrentTimeMode = millis();
   // Serial.print("BtMode_in: "); Serial.println(BtMode_in);
  }
  
  return ledMode_out;
}

bool Read_BtMenu (uint8_t BtMode_in, bool preMenu)
{
  bool Menustate = digitalRead(ButtonMenu_PIN);
  if((Menustate == false)&&(Menustate != preMenu))
  {
      count++; 
    if(count>3) 
      count = 0;
  }
  switch(count)
  {
    case 0: //Hien thi cac du lieu can thiet
      if(BtMode_in == MANUALMODE_ANGLE_SEND) //Che do manual angle
        Menu_oled_manual_ang( -Dy, Dx, YawDes, zDes); //roll, pitch, yaw, z
      else
        if(BtMode_in == MANUALMODE_POS_SEND) //Che do manual pos
          Menu_oled_manual_pos(Dx, Dy, YawDes, zDes);
        else  //Che do auto
          Menu_oled_automode(Dx, Dy, YawDes, zDes, BtMode_in, heading);
      break;
    case 1: //Hien thi cac thong so pump
      Menu_oled1(ManualPumpStatus, ManualPumpFlow);
      break; 
    case 2: //Hien thi du lieu pin va ten cong ty
      Menu_oled_About();
      break;
    default: //Man hinh scale
      Menu_oled2(rollscale_pin, pitchscale_pin, zscale_pin, yawscale_pin, BtMode_in);    
    break;
  }
  return Menustate;
}
