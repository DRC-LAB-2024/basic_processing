long convertBytesToLong(byte byte1, byte byte2, byte byte3, byte byte4) {
  return ((long)byte1 << 24) | ((long)byte2 << 16) | ((long)byte3 << 8) | (long)byte4;
}
