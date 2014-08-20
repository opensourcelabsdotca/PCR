
//in works, PID control for thermal block
void thermalBlock(double power)
{
  if (power>255) power = 255;
  if (power<0)  power = 0;
  analogWrite(5, power);
  
  
  display.print("Power = ");
  display.println(power);
}

//PID control for heated lid
void pid()
{
  lidError = lidSetpoint - analogRead(thermistor);
  //integral = integral + error*0.1;
  //display.print("integral = ");
  //display.println(integral);
  //analogWrite(lid, analogRead(thermistor)*(Kp*error + Ki*integral));
  lidPower = Kp*lidError;
  if(lidPower > 256) lidPower = 250;
  if(lidPower < 0) lidPower = 0;
  analogWrite(lid, lidPower);
  //analogWrite(ppin, 125);  //(test)
}

void debugLid()
{
  display.clearDisplay();   // clears the screen and buffer

 // text display tests
 display.setTextSize(1);
 display.setTextColor(BLACK);
 display.setCursor(0,0);
 
 display.print("power: ");
 display.println(lidPower);
 display.print("error: ");
 display.println(lidError);
 //display.print("integral = ");
 //display.println(integral);
 display.print("Lid: ");
 display.println(analogRead(thermistor));
 display.print("Temp: ");
 display.println(blockTemp());
}
