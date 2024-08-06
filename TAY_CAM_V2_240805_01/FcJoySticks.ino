float readJoyAnalog(int pinNum)
{
  int temp0 = analogRead(pinNum);
  return ((float)temp0);
 }
void JsErr(){
  int c=0, n=100, temp0 = 0;
  float temp_jlxErr = 0, temp_jlyErr = 0, temp_jrxErr = 0, temp_jryErr = 0, temp1;
  while(c<n){    
    jlxErr += readJoyAnalog(JoyLeftX)/n;
    jlyErr += readJoyAnalog(JoyLeftY)/n; //analogRead(JoyLeftY);
    jrxErr += readJoyAnalog(JoyRightX)/n; //analogRead(JoyRightX);
    jryErr += readJoyAnalog(JoyRightY)/n; //analogRead(JoyRightY);
    c++;
    delay(10);
  }
  //jlxErr/=n;
  jlxErr=142;
  jlyErr/=1;
  jrxErr/=1;
  jryErr/=1;
//  digitalWrite(LedB, LOW);
  Serial.print("Jlx ");
  Serial.print(jlxErr);
  Serial.print(" Jly ");
  Serial.print(jlyErr);
  Serial.print(" Jrx ");
  Serial.print(jrxErr);
  Serial.print(" Jry ");
  Serial.println(jryErr);
  delay(1000);
  
}

int ConvertD(int Val, int MaxVal, int MinVal){
  Val = int(Val/3.72);
  if (Val >= (MaxVal- 5)){
      return MaxVal;
  }
  else if (Val < (MinVal+5)){
    return MinVal;
  }
  else{
    return Val;
  }
}

void ReadJoySticks_Old(){
    int jleft_x = analogRead(JoyLeftX) - 0*jlxErr;
    int jleft_y = analogRead(JoyLeftY) - 0*jlyErr;
    
    int jright_x = analogRead(JoyRightX);// - jrxErr + 30;
    int jright_y = analogRead(JoyRightY);// - jryErr + 30;
    
    // if(abs(jleft_x) < 20) jleft_x =0;
    // if(abs(jleft_y) < 20) jleft_y =0;
    
    // if(abs(jright_x) < 45) jright_x = 0;
    // if(abs(jright_y) < 45) jright_y = 0;

    if (!CheckEmer){
      Dx = jright_x/3.72;
      Dy = -jright_y/3.72;
      
      Dx = constrain(Dx,-138,138);
      Dy = constrain(Dy,-138,138);
     
      zDes  += ((jleft_x)/400)*scale_z;
      //zDes = constrain(zDes,0,z_max*100);
      YawDes += jleft_y/400;
      if(abs(YawDes)>=180){
        YawDes = YawDes - 360*(YawDes/abs(YawDes));
        }

     Serial.print(" Dx: "); Serial.print(Dx);
     Serial.print(" Dy: "); Serial.print(Dy);
     Serial.print(" Yaw: "); Serial.print(YawDes);
     Serial.print(" Zdes: "); Serial.println(zDes);
      
    }

}
int jleft_x_pre = 0;
void ReadJoySticks(){
    float temp1 = 0, temp2, temp3;

    
    float jleft_x = readJoyAnalog(JoyLeftX) - 1*jlxErr;//analogRead(JoyLeftX) - 1*jlxErr;
    float jleft_y = readJoyAnalog(JoyLeftY) - 1*jlyErr;//analogRead(JoyLeftY) - 1*jlyErr;

//    Serial.print(" Zdes0: "); Serial.println(jleft_x);
//    Serial.print(" Yawdes0: "); Serial.println(jleft_y);
    
    float jright_x = readJoyAnalog(JoyRightX) - 1*jrxErr;//analogRead(JoyRightX) - jrxErr;
    float jright_y = readJoyAnalog(JoyRightY) - 1*jryErr;//analogRead(JoyRightY) - jryErr;

    //------ XU LY DX------------------------
      //temp1 = Dx;
      temp1 = jright_y/2;      
      temp1 = constrain(temp1,-138,138);     
      temp2 = (float)Dx; 
      temp2 = 0.5*temp2 + 0.52*scale_roll(temp1, pitchscale_pin);
      Dx = (long)temp2;
    //----- END OF XU LY DX------------------

    //----- XU LY DY-------------------------
     // temp1 = Dy;
      temp1 = jright_x/2;
      temp1 = constrain(temp1,-138,138);
      temp2 = (float)Dy;
      temp2 = 0.5*temp2 + 0.46*scale_pitch(temp1, rollscale_pin);
      Dy = (long)temp2;
     //---- END OF XU LY DY-----------------
//
    //---- XU LY Z-----------------------------------
      //temp1 = zDes;
      temp1 = jleft_x;//*0.5*10;
//       Serial.print(" Zdes1: "); Serial.println(temp1);
//       Serial.print(" Zdes2: "); Serial.println(zDes);
//      temp1 = constrain(temp1,0,1895);
      temp1 = constrain(temp1,0,375);
      temp2 = (float)zDes;
      temp3 = scale_Z(temp1, zscale_pin);
//      Serial.print(" yemp3: "); Serial.println(temp3);
      temp2 = 0.97*temp2 + 0.03*scale_Z(temp1, zscale_pin);
      zDes = (long)temp2;
//      Serial.print(" Zdes3: "); Serial.println(zDes);
    //--- END OF XU LY DZ-------------------------
//
    //--- XU LY YAW----------------------------------------------    
     // temp1 = YawDes;
      temp1 = jleft_y;
//      Serial.print(" Yaw: "); Serial.println(temp1);
      temp1 = constrain(temp1,-179,179);
      temp2 = (float)YawDes;
      temp2 = 0.6*temp2 + 0.4*scale_yaw(temp1, yawscale_pin);
      YawDes = (long)temp2;
    //--- END OF YAW-----------------------------------------------
////      YawDes += jleft_y/100;
////      if(abs(YawDes)>180){
////        YawDes = YawDes - 360*(YawDes/abs(YawDes));
////        }
//
//     Serial.print(" Dx: "); Serial.print(Dx);
//     Serial.print(" Dy: "); Serial.println(Dy);
//     Serial.print(" Yaw: "); Serial.println(YawDes);
//     Serial.print(" Zdes: "); Serial.println(zDes);
////         Serial.print(" Jly ");
////  Serial.println(jlxErr);
//      
//  //  }

}

float scale_roll(float temp0, int scale_pin)
{
  long temp1 = (long)temp0;
  long scale = analogRead(scale_pin);
  //*rscale = scale;
  long scalevalue = map(scale, 0, 1023, 50, 138);
  long scalevalue_1 = map(temp1, 0, 54, 0, scalevalue);
  long scalevalue_2 = map(temp1, -56, 0, -scalevalue, 0);
  if(temp1 >= 0) return ((float)scalevalue_1);
  if(temp1 < 0) return ((float)scalevalue_2);
  else return 0;
}

float scale_pitch(float temp0, int scale_pin)
{
  long temp1 = (long)temp0;
  long scale = analogRead(scale_pin);
  //*pscale = scale;
//  Serial.print(" pitch211: "); Serial.println(scale);
  long scalevalue = map(scale, 0, 1023, 50, 138);
//  Serial.print(" pitch212: "); Serial.println(scalevalue);
  long scalevalue_1 = map(temp1, 0, 50, 0, scalevalue);
//  Serial.print(" pitch212: "); Serial.println(scalevalue);
  long scalevalue_2 = map(temp1, -49, 0, -scalevalue, 0);
  if(temp1 >= 0) return ((float)scalevalue_1);
  if(temp1 < 0) return ((float)scalevalue_2);
  else return 0;
}

float scale_yaw(float temp0, int scale_pin)
{
  long temp1 = (long)temp0;
  long scale = analogRead(scale_pin);
  //*yscale = scale;
  long scalevalue = map(scale, 0, 1023, 40, 180);
  long scalevalue_1 = map(temp1, 0, 99, 0, scalevalue);
  long scalevalue_2 = map(temp1, -105, 0, -scalevalue, 0);
  if(temp1 >= 0) return ((float)scalevalue_1);
  if(temp1 < 0) return ((float)scalevalue_2);
  else return 0;
}

float scale_Z(float temp0, int scale_pin)
{
  long temp1 = (long)temp0;
  long scale = analogRead(scale_pin);
  //*zscale = scale;
  long scalevalue = map(scale, 0, 1023, 100, 5000);
  long scalevalue_1 = map(temp1, 0, 210, 0, scalevalue);
  return ((float)scalevalue_1);
}
