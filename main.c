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

// Digit control pins (adjust based on your wiring)
#define DIGIT1_EN (1 << PB0)
#define DIGIT2_EN (1 << PB1)
#define DIGIT3_EN (1 << PB2)
#define DIGIT4_EN (1 << PB3)

// Array to hold the digits to display
uint8_t displayDigits[4] = {1, 2, 3, 4}; // Start with 1, 2, 3, 4
volatile uint8_t currentDigit = 0;

void display_digit(uint8_t digit, uint8_t value) {
  PORTB = ~digit; // Turn ON the specified digit (assuming common cathode)
  PORTD = ~digitTable[value % 10];
  _delay_ms(1);
}

ISR(TIMER1_COMPA_vect) { currentDigit = (currentDigit + 1) % 10; }

void timer1_init() {
  TCCR1B |= (1 << WGM12); // Tryb CTC
  OCR1A = 15624;          // Przerwanie co 1 sekundę (dla 8 MHz, prescaler 1024)
  TIMSK |= (1 << OCIE1A); // Włącz przerwanie Timer1 CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
}

int main(void) {
  DDRB = 0x0F;  // PB0-PB3 as outputs (digit enable)
  DDRD = 0xFF;  // PD0-PD7 as outputs (segment control)
  PORTB = 0xFF; // Initially disable all digits
  timer1_init();
  sei();

  uint8_t counter = 0;

  while (1) {
    PORTB = 0x0F; // PB0-PB3 as outputs (digit enable)
    _delay_ms(1);
    display_digit(DIGIT1_EN, currentDigit);
    display_digit(DIGIT2_EN, (currentDigit + 1) % 10);
    display_digit(DIGIT3_EN, (currentDigit + 2) % 10);
    display_digit(DIGIT4_EN, (currentDigit + 3) % 10);
  }
}
