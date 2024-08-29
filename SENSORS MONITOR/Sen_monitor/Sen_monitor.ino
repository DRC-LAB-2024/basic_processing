#include "system_config.h"

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17);
  Serial2.begin(9600, SERIAL_8N1, 2, 15);

  BLEsetup();

  pinMode(RS232_LED, OUTPUT);
  pinMode(RS485_LED, OUTPUT);
  pinMode(ANALOG01_LED, OUTPUT);
  pinMode(ANALOG02_LED, OUTPUT);
  pinMode(DIGITAL01_LED, OUTPUT);
  pinMode(DIGITAL02_LED, OUTPUT);

  pinMode(ANALOG01_PIN, INPUT);
  pinMode(ANALOG02_PIN, INPUT);
  pinMode(DIGITAL01_PIN, INPUT_PULLDOWN);
  pinMode(DIGITAL02_PIN, INPUT_PULLDOWN);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  node.begin(1, Serial1);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Wire.begin(SDA_PIN, SCL_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(1000); 

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Setting up..."));
  display.display();
  delay(1000);

  ConnectToDB();
}

void loop() {
  if(millis() - read_data_timer > 50 || read_data_timer == 0)
  {
    read_data_timer = millis();

    if(millis() - rs485_timer > 1000 || rs485_timer == 0)
    {
      rs485_timer = millis();
      uint8_t result = node.readInputRegisters(0x0001, 2);
      Serial.println("Data Requested");

      if (result == node.ku8MBSuccess) {
        temp = float(node.getResponseBuffer(0) / 10.00F);
        humid = float(node.getResponseBuffer(1) / 10.00F);
      }  

      if(temp != 0) 
        digitalWrite(RS485_LED, HIGH);  
    }

    if(Serial2.available())
    {
      String data = Serial2.readString();
      Serial.println(data);
    }

    a1 = analogRead(ANALOG01_PIN);
    a2 = analogRead(ANALOG02_PIN);
    d1 = digitalRead(DIGITAL01_PIN);
    d2 = digitalRead(DIGITAL02_PIN);

    if(a1 != 0) 
        digitalWrite(ANALOG01_LED, HIGH);
    if(a2 != 0) 
        digitalWrite(ANALOG02_LED, HIGH);
    if(d1 != 0) 
        digitalWrite(DIGITAL01_LED, HIGH);
    if(d2 != 0) 
        digitalWrite(DIGITAL02_LED, HIGH); 
  }

  if(millis() - send_data_timer > 1000 || send_data_timer == 0)
  {
    send_data_timer = millis();
    if(conn.connected())
    {
      SendData(temp, humid, a1, a2, d1, d2);
    }
    else
    {
      conn.close();
      if(conn.connect(server_addr, 3306, user, password, db)) 
      {
        Serial.printf("Reconnected to database: %s", db);
      }
    }
  }

  if(millis() - show_data_timer > 200 || show_data_timer == 0)
  {
    show_data_timer = millis();
    ShowOnLCD(ssid, temp, humid, a1, a2, d1, d2);
  }
}

void devide()
{
  display.drawLine(0, 0, 128, 0, WHITE);
  display.drawLine(0, 0, 0, 63, WHITE);
  display.drawLine(0, 63, 127, 63, WHITE);
  display.drawLine(127, 0, 127, 63, WHITE);
  
  display.drawLine(64, 16, 64, 64, WHITE);
  display.drawLine(0, 16, 128, 16, WHITE);
  display.drawLine(0, 32, 128, 32, WHITE);
  display.drawLine(0, 48, 128, 48, WHITE);
}

void ShowOnLCD(char* SSID, float temp, float humid, int a1, int a2, int d1, int d2){
  display.clearDisplay();
  devide();

  display.setCursor(24, 4);
  display.print(F("WiFi: "));
  display.println(SSID);

  display.setCursor(5, 20);
  display.print(F("T"));
  display.write(248);
  display.print(F(": "));
  display.println(temp);

  display.setCursor(69, 20);
  display.print(F("H%: "));
  display.println(humid);

  display.setCursor(5, 36);
  display.print(F("a1: "));
  display.println(a1);

  display.setCursor(69, 36);
  display.print(F("d1: "));
  display.println(d1);

  display.setCursor(5, 52);
  display.print(F("a2: "));
  display.println(a2);

  display.setCursor(69, 52);
  display.print(F("d2: "));
  display.println(d2);

  display.display();
}

void ConnectToDB()
{
  Serial.println("Connecting to WiFi...");
  ssid = stoca(Essid());
  pass = stoca(Epass());
  WiFi.begin(ssid, pass);

  display.clearDisplay();
  display.setCursor(4, 10);
  char dpl[128];
  sprintf(dpl, "Connecting to %s ...", ssid); 
  display.println(dpl);
  display.display();

  int count = 20;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count--;
    if(count == 0)
    {
      while(1)
      {
        BLErepeat();

        display.clearDisplay();
        display.setCursor(20, 10); 
        display.println("Scan Bluetooth");
        display.setCursor(20, 20);
        display.println("to enter WiFi.");
        display.display();

        if(WiFi.status() == WL_CONNECTED)
          break;
      }
    }
  }
 
  display.clearDisplay();
  display.setCursor(4, 20);
  display.println("Connected.");
  display.display();
  delay(500);

  Serial.println("\nConnected to WiFi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Attempting to connect to MySQL...");

  if(conn.connect(server_addr, 3306, user, password, db)) 
  {
    display.clearDisplay();
    display.setCursor(4, 10);
    display.println("The monitor is ready");
    display.display();
    delay(500);
  } 
  else
  {
    while(1)
    {
      if(conn.connect(server_addr, 3306, user, password, db)) 
      {
        delay(500);
        Serial.printf("Reconnected to database: %s", db);
        break;
      }
    }
  }
}

// Convert String into char array
char* stoca(const String& str) {
  char* charArray = new char[str.length() + 1];
  str.toCharArray(charArray, str.length() + 1);
  return charArray;
}

void SendData(float temp, float humid, int a1, int a2, int d1, int d2)
{
  char PROGMEM query[256];
  sprintf(query, "UPDATE iot_monitor SET temperature = %.2f, humidity = %.2f, analog_1 = %d, analog_2 = %d, digital_1 = %d, digital_2 = %d WHERE iot_id = '%s'", temp, humid, a1, a2, d1, d2, device_id);


  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query);
  delete cur_mem;
  // Serial.println("Data updated to MySQL.");
}

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
