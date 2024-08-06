void Menu_oled_manual_ang(long roll, long pitch, long yaw, long Z )
{
  char roll_string[10], pitch_string[10], yaw_string[10], Z_string[10];
  //in ra oled
  //chuyển long thành string
  sprintf(roll_string, "%ld", roll);
  sprintf(pitch_string, "%ld", pitch);
  sprintf(yaw_string, "%ld", yaw);
  sprintf(Z_string, "%ld", Z);
  display.clearDisplay(); 
  display.setTextSize(1);
  //gửi dữ liệu lên oled
  //gửi roll
  display.setCursor(0, 0);  // (x,y)
  display.print("roll:");
  display.setCursor(36, 0);
  display.print(roll_string);
  //gửi pitch
  display.setCursor(0, 16);
  display.print("pitch:");
  display.setCursor(36, 16);
  display.print(pitch_string);
  //gửi yaw
  display.setCursor(0, 32);
  display.print("yaw:");
  display.setCursor(36, 32);
  display.print(yaw_string);
  //gửi Z
  display.setCursor(0, 48);
  display.print("Z:");
  display.setCursor(36, 48);
  display.print(Z_string);
  display.display();
}

void Menu_oled2(unsigned int rollscale_pin,unsigned int pitchscale_pin,unsigned int zscale_pin,unsigned int yawscale_pin, uint8_t BtMode_in)
{
//  long roll = analogRead(rollscale);
//  long roll_sc = map(roll, 0, 1023, 50, 138);
  long roll_sc = read_scale_value(rollscale_pin, rollscale_MIN, rollscale_MAX);
//  long pitch = analogRead(pitchscale);
//  long pitch_sc = map(pitch, 0, 1023, 50, 138);
  long pitch_sc = read_scale_value(pitchscale_pin, pitchscale_MIN, pitchscale_MAX);
//  long z = analogRead(zscale);
//  long z_sc = map(z, 0, 1023, 200, 5000);
  long z_sc = read_scale_value(zscale_pin, zscale_MIN, zscale_MAX);
//  long yaw = analogRead(yawscale);
//  long yaw_sc = map(yaw, 0, 1023, 40, 180);
  long yaw_sc = read_scale_value(yawscale_pin, yawscale_MIN, yawscale_MAX);
  char X_1[10], Y_1[10], X_2[10], Y_2[10];
  //chuyển long thành string
  sprintf(X_1, "%ld", roll_sc);
  sprintf(Y_1, "%ld", pitch_sc);
  sprintf(X_2, "%ld", z_sc);
  sprintf(Y_2, "%ld", yaw_sc);
  display.clearDisplay(); 
  display.setTextSize(1);
  //gửi dữ liệu lên oled
  //gửi X_1
  display.setCursor(0, 0);  // (x,y)
  if(BtMode_in == MANUALMODE_ANGLE_SEND)
    display.print("ROS:");
  else
    display.print("XSC:");
  display.setCursor(36, 0);
  display.print(X_1);
  //gửi Y_1
  display.setCursor(0, 16);
  if(BtMode_in == MANUALMODE_ANGLE_SEND)
    display.print("PIS:");
  else
    display.print("YSC:");
  display.setCursor(36, 16);
  display.print(Y_1);
  //gửi X_2
  display.setCursor(0, 32);
  display.print("ZDS:");
  display.setCursor(36, 32);
  display.print(X_2);
  //gửi Y_2
  display.setCursor(0, 48);
  display.print("YAS:");
  display.setCursor(36, 48);
  display.print(Y_2);
  display.display();
}

void Menu_oled_manual_pos(long dx, long dy, long yaw_in, long z_in )
{
  char dx_string[10], dy_string[10], yaw_string[10], z_string[10];
  //in ra oled
  //chuyển long thành string
  sprintf(dx_string, "%ld", dx);
  sprintf(dy_string, "%ld", dy);
  sprintf(yaw_string, "%ld", yaw_in);
  sprintf(z_string, "%ld", z_in);
  display.clearDisplay(); 
  display.setTextSize(1);
  //gửi dữ liệu lên oled
  //gửi roll
  display.setCursor(0, 0);  // (x,y)
  display.print("Dx:");
  display.setCursor(36, 0);
  display.print(dx_string);
  //gửi pitch
  display.setCursor(0, 16);
  display.print("Dy:");
  display.setCursor(36, 16);
  display.print(dy_string);
  //gửi yaw
  display.setCursor(0, 32);
  display.print("Yaw:");
  display.setCursor(36, 32);
  display.print(yaw_string);
  //gửi Z
  display.setCursor(0, 48);
  display.print("Z:");
  display.setCursor(36, 48);
  display.print(z_string);
  display.display();
}

void Menu_oled_automode(long dx, long dy, long yaw_in, long z_in, uint8_t BtMode_in, float heading)
{
  char dx_string[10], dy_string[10], yaw_string[10], z_string[10], heading_string[10];
  //in ra oled
  //chuyển long thành string
  sprintf(dx_string, "%ld", dx);
  sprintf(dy_string, "%ld", dy);
  sprintf(yaw_string, "%ld", yaw_in);
  sprintf(z_string, "%ld", z_in);
  sprintf(heading_string, "%d", heading);
  display.clearDisplay(); 
  display.setTextSize(1);
  //gửi dữ liệu lên oled
  //gửi roll
  display.setCursor(0, 0);  // (x,y)
  display.print("Mod:");
  display.setCursor(36, 0);
  if(BtMode_in == AUTOMODE_SINGLEPOINT_SEND)
    display.print("S POINT");
  else if(BtMode_in == AUTOMODE_MULTIPLEPOINT_SEND)
    display.print("M POINT");
  else display.print("UNKNOWN");
  //gửi pitch
  display.setCursor(0, 16);
  display.print("BTID:");
  display.setCursor(36, 16);
  display.print(ESP32_BLUETOOTHID);
  
  //gửi yaw
  display.setCursor(0, 32);
  display.print("Heading:");
  display.setCursor(52, 32);
  display.print(heading);
  //gửi Z
  display.setCursor(0, 48);
  display.print("Z:");
  display.setCursor(36, 48);
  display.print(z_string);
  display.display();
}
