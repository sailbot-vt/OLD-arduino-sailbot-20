void setupRCDecode() 
{
  attachInterrupt(RUDDERPIN, rudderRising, RISING);
  attachInterrupt(TRIMPIN, trimRising, RISING);
}

int getRudderPWM()
{
  return rudderStruct->pwmValue;
}

int getTrimPWM()
{
  return trimStruct->pwmValue;
}

// ISRs
static void rudderRising()
{
  attachInterrupt(RUDDERPIN, rudderFalling, FALLING);
  rudderStruct->prevRead = micros(); 
}

static void rudderFalling()
{
  attachInterrupt(RUDDERPIN, rudderRising, RISING);
  rudderStruct->pwmValue = micros() - rudderStruct->prevRead;
  //Serial.println(rudderStruct->pwmValue);
}

static void trimRising()
{
  attachInterrupt(TRIMPIN, trimFalling, FALLING);
  trimStruct->prevRead = micros();
}

static void trimFalling()
{
  attachInterrupt(TRIMPIN, trimRising, RISING);
  trimStruct->pwmValue = micros() - trimStruct->prevRead;
}
