void SetUpIO(){
  pinMode(ButtonStart_PIN, INPUT_PULLUP);
  pinMode(ButtonStop_PIN, INPUT_PULLUP);
  pinMode(ButtonEmer_PIN, INPUT_PULLUP);
  pinMode(ButtonMenu_PIN, INPUT_PULLUP);

  pinMode(LedRFNhan, OUTPUT);
  pinMode(LedDKC, OUTPUT);
  pinMode(LedCDBay, OUTPUT);
  pinMode(LedRF_ESP, OUTPUT);

  pinMode(AUTOMANUAL_MODE_PIN, INPUT_PULLUP);
  pinMode(MANUALPOS_ANL_MODE_PIN, INPUT_PULLUP);
  pinMode(PUMP_MODE_PIN, INPUT_PULLUP);

  pinMode(rollscale_pin, INPUT);
  pinMode(pitchscale_pin, INPUT);
  pinMode(zscale_pin, INPUT);
  pinMode(yawscale_pin, INPUT);
}

void InitializePre(){
  PreStaMode = digitalRead(AUTOMANUAL_MODE_PIN);
  PreStaEmer = digitalRead(MANUALPOS_ANL_MODE_PIN);
  PreBtnStart = digitalRead(ButtonStart_PIN);
  PreBtnStop = digitalRead(ButtonStop_PIN);
  PreBtnEmer = digitalRead(ButtonEmer_PIN);
  delay(20);
  PreStaMode = digitalRead(AUTOMANUAL_MODE_PIN);
  PreStaEmer = digitalRead(MANUALPOS_ANL_MODE_PIN);
  PreBtnStart = digitalRead(ButtonStart_PIN);
  PreBtnStop = digitalRead(ButtonStop_PIN);
  PreBtnEmer = digitalRead(ButtonEmer_PIN);
}

void SetupRF(){
  radio.openWritingPipe(0xe8e8f0f0e1);
  //radio.openReadingPipe(1,0xf1f1f0f0e0);
  radio.setPALevel(RF24_PA_MIN); 
  radio.setChannel(0x77); 
  radio.enableDynamicPayloads();                     
  radio.setDataRate(RF24_1MBPS);                                 
  radio.stopListening(); //Cài đặt module là TX
}

void setup_oled()
{
  //khởi tạo màng hình
  delay(100);  // This delay is needed to let the display to initialize
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(true); //tự động ngắt dòng
  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)
}

void setup_laban()
{
   Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  //khởi tạo cảm biến la bàn
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
}
