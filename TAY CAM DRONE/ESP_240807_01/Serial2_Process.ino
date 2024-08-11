/*----------------------------------------------------------------------------
 * GOI CHUOI DU LIEU QUA MEGA
-----------------------------------------------------------------------------*/
void sendStringDatatoMega(String sendData){
  if(MegaPort.availableForWrite()>=63) //Cho buffer trong moi truyen 
    MegaPort.println(sendData);
}
/*----------------------------------------------------------------------------
 * GOI DU LIEU BAY QUA MEGA
-----------------------------------------------------------------------------*/

void sendDatabySerial2(long (*dataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long indexRow)
{
  String tempString;
  while(MegaPort.availableForWrite()<63) //Cho buffer trong moi truyen
  {;}
  tempString = formatFlyPointData(dataMemory, indexRow);
  if(tempString != "")
  {
    MegaPort.println(tempString);
    Serial.println(tempString);
    
  }else //Truong hop loi ==> Phat loa  
//    digitalWrite(loa_pin, HIGH);  
    panelReact(loa_pin, 1);
}

/*------------------------------------------------------------------------
--------------------------------------------------------------------------*/
String formatFlyPointData(long (*dataMemory)[POINT_MEMORY_INDEX_LEN_ALL], long indexRow)
{
  char temp1[55];  
  if((indexRow >= 0) && (indexRow < POINT_MEMORY_INDEX_LONG_ALL))
  {
    sprintf(temp1,"*%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld#", dataMemory[indexRow][POINT_MEMORY_INDEX_MODE], dataMemory[indexRow][POINT_MEMORY_INDEX_LAT], 
    dataMemory[indexRow][POINT_MEMORY_INDEX_LONG],dataMemory[indexRow][POINT_MEMORY_INDEX_Z],dataMemory[indexRow][POINT_MEMORY_INDEX_TIME],
    dataMemory[indexRow][POINT_MEMORY_INDEX_LEN], dataMemory[indexRow][POINT_MEMORY_INDEX_INDEX], dataMemory[indexRow][POINT_MEMORY_INDEX_CHECKSUM]);
    return (String (temp1));     
  }else //Truong hop loi ==> ""  
    return (String (""));  
}
