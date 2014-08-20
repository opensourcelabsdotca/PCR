

boolean startbutton()
{
  int readingStart = digitalRead(startPin);
  
  if (readingStart != lastStartState) lastDebounceTime = millis();
  
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingStart != startState) 
    {
      startState = readingStart;

      // only toggle the LED if the new button state is HIGH
      if (startState == HIGH && pcrState == 0) 
      {
        pcrState = 1;
        return true;
      }
    }
  }
    lastStartState = readingStart;
    return false;
}

boolean stopbutton()
{
  int readingStop = digitalRead(stopPin);
  
  if (readingStop != lastStopState) lastDebounceTime = millis(); 
  
  if ((millis() - lastDebounceTime) > debounceDelay)
  
   if (readingStop != lastStopState) lastDebounceTime = millis();
    
    if(readingStop != stopState)
    {
      stopState = readingStop;
      
      if (stopState == HIGH) 
      {
        pcrState = 0;
        return true;
      }
    }
    
    lastStopState = readingStop;
    return false;
}




 
