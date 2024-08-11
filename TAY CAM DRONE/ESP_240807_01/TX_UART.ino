void TX_UART()
{
  for (int i = 0; i < 3; i++)
  {
    MegaPort.write((uint8_t*)&sendData[i], sizeof(sendData[i]));
  }
}
