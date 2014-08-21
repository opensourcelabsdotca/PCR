
void alarm()
{
  if(blockTemp() > alarmThreshold)
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}
  
