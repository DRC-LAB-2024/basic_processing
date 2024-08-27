# ORC_Utilities 

## Task 1: Check whether the input String matches the given format or not.
 

### Code:
```c
// Function to check format of the String
bool CheckFormat(const String &input, const String &header, const String &footer)
{
  // True format: "header, ...footer"

  if(input.startsWith(header) && input.endsWith(footer))
    return true;
  return false;
}
```
> - The startsWith() function return true if the input String starts with the header String, return false if not.
> - The endsWith() function return true if the input String ends with the footer String, return false if not.
> - Combine two functions above to check the format of the input String with the format "header,...footer".

## Task 2: Write a function to parse a String into a long array.
### Main code:
```c
// Function to get data from a String
bool ParseString(const String &input, const String &prefix, long *data, const int length)
{
  if(!input.startsWith(prefix) || !input.endsWith("#"))
    return false;

  String content = input.substring(prefix.length() + 1, input.length() - 1);
  // Serial.println(content);
  int i = 0;
  int start = 0;
  int end = content.indexOf(',');

  while (end != -1 && i < length) 
  {
    data[i++] = convertString2Long(removeDotinString(content.substring(start, end)));
    start = end + 1;
    end = content.indexOf(',', start);
  }

  if (i < length) {
    data[i++] = convertString2Long(removeDotinString(content.substring(start))); 
  }

  if(i == length)
    return true;

  Serial.println("Your length is greater than the number of data in the String.");
  return false;
}
```
> - If the String is not match the given format ("*prefix,..,..,..#"), return false.
> - Create a String content to store the input String after substring header and footer.
> - Take the data separated by ',' and remove dot if exist with removeDotinString() function. Convert it into a long number with convertString2Long() function.
> - If there is no more ',', take the last data and return true.
> - If the input length is greater than the numbers of data in the input String, return false.

### convertString2Long function:
```c
/*-----------------------------------------------------------
 * ------CONVERT STRING TO LONG: "123678" -> 123678
---------------------------------------------------------------*/
long convertString2Long(String in)
{
  String string_in = in, data = "";
  int len = string_in.length();
  int temp1 = 0;
  long out = 0, temp2 = 0;;
  while(len > 0)
  {
    data = string_in.substring(0,1);
    if(isStringNumber(&temp1, data[0])) {   
	temp2 = (long)temp1;
    out = out*10 + temp2;}
    string_in.remove(0, 1);    
    len = string_in.length();
  }  
  return out;
}
```

### isStringNumber function:
```c
bool isStringNumber(int *out, char in)
{
  switch(in){
    case '0':
    *out = 0; return true;
    case '1':
    *out = 1; return true;
    case '2':
    *out = 2; return true;
    case '3':
    *out = 3; return true;
    case '4':
    *out = 4; return true;
    case '5':
    *out = 5; return true;
    case '6':
    *out = 6; return true;
    case '7':
    *out = 7; return true;
    case '8':
    *out = 8; return true;
    case '9':
    *out = 9; return true;
    default:
      return false;
    }   
}
```

### removeDotinString function:
```c
/*--------------------------------------------------------
* Remove dot in string: "123.45" -> "12345"
----------------------------------------------------------*/
String removeDotinString(String stringIn)
{
  String in = stringIn;
  int Pos=in.indexOf('.'); //TIM DAU PHAY CUA DU LIEU LONG
  if(Pos > 0){
    in.remove(Pos,1);
    // *status_in = true; 
    return(in);
    }else
    {
      // *status_in = false; 
      return(in);
    }
}
```

## Task 3: Write a function to calculate checksum of a long number.
### Code:
```c
// Function to calculate checksum for a long number
long CalculateChecksum(const long input) {
  byte byteData[4] = {0,0,0,0};
  byteData[0] = (input >> 24) & 0xFF;
  byteData[1] = (input >> 16) & 0xFF;
  byteData[2] = (input >> 8) & 0xFF;
  byteData[3] = input & 0xFF;
  return ((long)byteData[0] + (long)byteData[1] + (long)byteData[2] + (long)byteData[3]);
}
```
> - Transform the input long number into 4 bytes and return the sum of that 4 bytes

## Task 4: Write a function to calculate checksum of a long array.
### Code:
```c
// Function to calculate checksum for an array
long ArrayChecksum(const long *inputArr, int length)
{
  long total = 0;
  for(int i = 0; i < length; i++)
    total += CalculateChecksum(inputArr[i]);

  return total;
}
```
> - Use for loop to calculate checksum of each of number in the array and add it to the variable total.

## Task 5: Write a fucntion to connect to a Database, return in bool.
### Code:
```c
// Function to connect to Database
bool ConnectToDB()
{
  // MySQL information
  IPAddress server_addr(103, 200, 23, 149);
  int MySQL_port = 3306;
  char* user = "optimaro_hoang_dev_esp32";
  char* password = "Hoang2468@";
  char* db = "optimaro_hoang_dev";

  // WiFi information
  char* ssid = "Z";
  char* pass = "56575859";

  if(CheckWiFi(ssid, pass, 20))   // Retry 20 times to connect to WiFi
  {
    Serial.println("Connecting to MySQL server...");
    if (conn.connect(server_addr, MySQL_port, user, password, db)) 
    {
      delay(1000);
      Serial.printf("Connected to database: %s", db);
      Serial.println();
      return true;
    } 
    else 
    {
      Serial.println("Can't connect to Database.");
      return false;
    }
  }
  else
  {
    Serial.println("Can't connect to WiFi.");
    return false;
  }
}
```
> - MySQL information is used to authenticate to the Database.
> - WiFi information is the SSID and Password for the ESP32 to connect to the Internet.
> - If the connection to WiFi is OK, start the connection to Database else return false.
> - If the connection to Database is OK, return true else return false.

### Function to check connection to WiFi
```c
// Function to check connection to WiFi
bool CheckWiFi(char* ssid, char* pass, int retry)
{  
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry--;
    if(retry == 0)
      return false;
  }

  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
  return true;
}
```

## Task 6: Write a function to check connection to Database.
### Code:
```c
// Function to check connection to Database
bool CheckConnectionToDB()
{
  return conn.connected();
}
```

> - The conn.connected() is equal true if the connection is fine and false in opposite.

## Task 7: Write a function to send data to the Database.
### Code:
```c
// Function to send data to database:
bool SendDataToDB(bool connStatus, long *data, String *dataName)
{
  cursor = new MySQL_Cursor(&conn);

  String result = formatArrays(data, dataName, dataName->length());

  char PROGMEM query[256];
  sprintf(query, "UPDATE ORC_Utilities SET %s WHERE id = '1'", stoca(result));
  
  if(connStatus)
  {
    cursor->execute(query);
    Serial.println("Done sending.");
    return true;
  }
  Serial.println("ERROR");
  return false;
}

// Format String for SQL query
String formatArrays(long *A, String *B, int n) {
  String result = "";

  for (int i = 0; i < n; ++i) 
  {
    if (i > 0) 
      result += ", ";
    result += B[i] + " = " + String(A[i]);
  }

  return result;
}
```

> - The input of this function is the connection status of Database, an array of long numbers, an array of the names of datas.
> - First, we need to combine the data array(ex: {7624354,2873478,200,7423,334}) and dataname array (ex: {"lat","long","Z","lat_check","long_check"}) into a String result = "lat = 7624354, long = 2873478, Z = 200, lat_check = 7423, long_check = 334" to make a SQL query.
> - The example query to update to Database is: UPDATE ORC_Utilities SET lat = 7624354, long = 2873478, Z = 200, lat_check = 7423, long_check = 334 WHERE id = '1'. This query will update the table ORC_Utilities with the follow collums and data where the id collum euqual to '1'.


### Format String function:
```c
// Format String for SQL query
String formatArrays(long *A, String *B, int n) {
  String result = "";

  for (int i = 0; i < n; ++i) 
  {
    if (i > 0) 
      result += ", ";
    result += B[i] + " = " + String(A[i]);
  }

  return result;
}
```

### Function to covert String into a char array:
```c
// Convert String into char array
char* stoca(const String& str) {
  char* charArray = new char[str.length() + 1];
  str.toCharArray(charArray, str.length() + 1);
  return charArray;
}
```