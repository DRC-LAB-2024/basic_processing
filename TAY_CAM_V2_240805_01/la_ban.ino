float la_ban()
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  // Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  // Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  // Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  float X = 0, Y = 0, Z = 0, X_kalman = 0, Y_kalman = 0, Z_kalman = 0;
   X = event.magnetic.x;
   Y = event.magnetic.y;
   Z = event.magnetic.z;

  X = bo_loc.updateEstimate(X);
  Y = bo_loc.updateEstimate(Y);
  Z = bo_loc.updateEstimate(Z);

  Serial.print("X: "); Serial.print(X); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Z); Serial.print("  ");
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(Y, X);
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 

  //in ra Serial monitor
  Serial.print("heading (degrees): "); Serial.println(headingDegrees);
  return headingDegrees;
}

void calib_hmc ()
{

}