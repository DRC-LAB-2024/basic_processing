char *Example_string = "*GD,1645.246,106.8222825,10.8103755,200,23.423,78.474,270824#";

void setup() {
  Serial.begin(115200);

  if(ConnectToDB())
    Serial.println("Connected.");
  else
    Serial.println("Can't Connect.");

  long data[7];

  int dataLength = 9;
  
  long newdata[dataLength];

  if(ParseString(Example_string, "*GD", data, 7))
  {   
    for(int i = 0; i < 7; i++)
    {
      newdata[i] = data[i];
    }

    newdata[7] = CalculateChecksum(data[0]);
    newdata[8] = CalculateChecksum(data[1]);

    Serial.println("Done parsing.");
  }
  else
    Serial.println("ERROR.");

  String dataNames[dataLength] = {"UTC","LATITUDE","LONGITUDE","Knots","KmPerHour","Z","DAY","Lat_checksum","Long_checksum"};
  
  Serial.println("Sending data...");
  if(SendDataToDB(CheckConnectionToDB(), newdata, dataNames, dataLength))
    Serial.println("Success.");
  else
    Serial.println("Something Wrong.");
}

void loop() {
}
