void screen() 
{
  display.clearDisplay();   // clears the screen and buffer

 // text display tests
 display.setTextSize(1);
 display.setTextColor(BLACK);
 display.setCursor(0,0);
 
 switch(pcrState)
 {
   case 0:
     display.println("Standing by"); 
     break;
   case 1:
     display.println("Denaturing");
     break;
   case 2:
     display.println("Annealing");
     break;
   case 3:
     display.println("Extending");
     break;
   case 4:
     display.println("Holding");
     break;
 };

// display.print("Internal Temp = ");
// display.println(thermocouple.readInternal());
 double c = thermocouple.readCelsius();
  if (isnan(c)) 
  {
    display.println("Bad thing!");
  } 
  
  else 
  {
    display.print("Block: "); 
    display.println(c);
    Serial.println(c);
    display.print("DeltaT: ");
    display.println((deltaTemp[0]+deltaTemp[1]+deltaTemp[2])/3);
    
    //blinking without elay, adapted for temperature vs. time
    unsigned long currentMillis = millis();
    
    if(currentMillis - previousMillis > interval) 
    {
      previousMillis = currentMillis;
    
      deltaTemp[2] = deltaTemp[1];
      deltaTemp[1] = deltaTemp[0];
      
      deltaTemp[0] = (blockTemp() - previousTemp);
      previousTemp = blockTemp();
    }
    if (pcrState != 0)
    {
      long deltaTime = millis() - startTime;
      display.print("RunTime:");
      display.print(deltaTime/60000);
      display.print(":");
      if((deltaTime/1000) % 60 < 10) display.print("0");
      display.println((deltaTime/1000) % 60);
    }
    
    display.print("Cycle:");
    display.println(cycleCount);
    
    display.print("Lid:");
    display.println(analogRead(thermistor));
  }
}
