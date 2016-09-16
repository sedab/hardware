#define CPU_HZ 48000000
#define TIMER_PRESCALER 1024
#define FREQUENCY_HZ 4


bool isLEDOn = false;

void setup() {
  pinMode(6, OUTPUT);
  startTimer(FREQUENCY_HZ);
  Serial.begin(9600);
  while (!Serial) {

  }

}

void loop() {

}

void setTimerFrequency(int frequencyHz) {
  int compare_value = (CPU_HZ / (TIMER_PRESCALER * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;

  TC->CC[0].reg = compare_value;
  while (TC->STATUS.bit.SYNCBUSY == 1);
}


void startTimer(int frequencyHz) {
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID (GCM_TCC2_TC3)) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 );

  TcCount16* TC = (TcCount16*) TC3;

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;

  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC->STATUS.bit.SYNCBUSY == 1);

  setTimerFrequency(frequencyHz);

  TC->INTENSET.reg = 0;
  TC->INTENSET.bit.MC0 = 1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  // If this interrupt is due to the compare register matching the timer count
  // we toggle the LED.
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
    digitalWrite(6, isLEDOn);
    isLEDOn = !isLEDOn;
    Serial.print("Toggle bitch! - ");
    Serial.println(!isLEDOn);
  }
}
