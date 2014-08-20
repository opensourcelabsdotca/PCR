//thermistor is with Alex's thermocouple

void heat()
{
   digitalWrite(Relay1, LOW);
   digitalWrite(Relay2, HIGH);
   //display.print("Heating");
}

void cool() 
{
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, LOW);
  //display.print("Cooling");
}

void off() 
{
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  //display.print("Off");
}

void hold(int pastState)
{
  intpcrState = pastState;
  pcrState = 4;
  holdTemp = blockTemp();
  holdStart = millis(); 
}

void debugHold()
{
  display.clearDisplay();   // clears the screen and buffer

  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  
  display.print("Temp: ");
  display.println(blockTemp());
}
  
