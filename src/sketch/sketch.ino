/* Date : 2024-12-09 
*/ 
#include <stdint.h>
#include <stdlib.h>

#define ADC_MAX 1023

// Requirements Definitions
const uint8_t required_delay[] = {1, 60}; // Min Max
const uint8_t required_freq[] = {3, 105}; // 3 to 105 Hz

// Pins Definitions
const uint8_t ADC_PIN = A0;
const uint8_t FREQUENCY_CHANGE_PIN = D1;
const uint8_t OUTPUT_PIN = D2;
const uint8_t RESET_PIN = D3;
bool reset = false;

// To store the delay  and frequency
uint16_t current_delay = 1; // To store the delay
uint8_t current_freq = 1;   // To store the frequency

void get_delay(uint16_t *_current_delay) {
  uint16_t _raw_adc_value = analogRead(ADC_PIN);
  *_current_delay = (_raw_adc_value / 17) * 1000; // 60 / 1023(adc_max)  = ~17
  Serial.println("delay changed to : " + String(*_current_delay));
}
void IRAM_ATTR change_freq() {
  Serial.println("Changing Frequency");
  if (current_freq > required_freq[1]) {
    current_freq = required_freq[0];
  }
  current_freq += 2;
  Serial.println("Frequency Changed to : " + String(current_freq));
}
void IRAM_ATTR do_reset() {
  
  reset = reset ^ 1;
  get_delay(&current_delay);

  Serial.println("Resetting" );
}
void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(FREQUENCY_CHANGE_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  attachInterrupt(FREQUENCY_CHANGE_PIN, change_freq, FALLING);
  attachInterrupt(RESET_PIN, do_reset, FALLING);
  pinMode(ADC_PIN, INPUT);
  get_delay(&current_delay);
}
void loop() {
  Serial.println("Current Delay " + String(current_delay));
  delay(2000);
  while (reset == 0) {
  Serial.println("Inside While");
  uint16_t step_delay = current_delay;
  while (step_delay > 0) {
  Serial.println("Step Delay : " + String(step_delay));
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(1000 / (current_freq  *2));
    digitalWrite(OUTPUT_PIN, LOW);
    delay(1000 / ( current_freq *  2));
    step_delay -= 1000 / (current_freq * 2);
  }
  reset = true;
}
}

