#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

const uint8_t digitTable[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

#define DIGIT1_EN (1 << PB0)
#define DIGIT2_EN (1 << PB1)
#define DIGIT3_EN (1 << PB2)
#define DIGIT4_EN (1 << PB3)

volatile uint16_t value = 0;
volatile uint8_t display_number[4] = {0, 0, 0, 0};

void display_digit(uint8_t digit, uint8_t value) {
  PORTB = ~digit; // Turn ON the specified digit (assuming common cathode)
  PORTD = ~digitTable[value % 10];
  _delay_ms(1);
}

ISR(TIMER1_COMPA_vect) {
  value = (value + 1) % 10000;
  display_number[0] = value % 10;
  display_number[1] = (value / 10) % 10;
  display_number[2] = (value / 100) % 10;
  display_number[3] = value / 1000;
}

void timer1_init() {
  TCCR1B |= (1 << WGM12);              // Configure timer1 for CTC mode
  OCR1A = 1562;                        // count till 1562 to get 1 second delay
  TIMSK |= (1 << OCIE1A);              // Enable Timer1 compare interrupt
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
}

int main(void) {
  DDRB = 0x0F;  // PB0-PB3 as outputs (digit enable)
  DDRD = 0xFF;  // PD0-PD7 as outputs (segment control)
  PORTB = 0xFF; // Initially disable all digits
  timer1_init();
  sei(); // Enable global interrupts

  uint8_t counter = 0;

  while (1) {
    display_digit(DIGIT1_EN, display_number[0]);
    display_digit(DIGIT2_EN, display_number[1]);
    display_digit(DIGIT3_EN, display_number[2]);
    display_digit(DIGIT4_EN, display_number[3]);
  }
}
