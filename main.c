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

// Cyfry do wyświetlenia (np. "1234")
uint8_t digits[4] = {0b0001, 0b0010, 0b0100, 0b1000};

int main(void) {
  DDRB = 0x0F; // PB0-PB3 jako wyjścia (sterowanie cyframi)
  DDRD = 0xFF; // PD0-PD7 jako wyjścia (segmenty A-G + DP)
               //

  while (1) {
    for (uint8_t i = 0; i < 9; i++) {
      PORTB = ~digits[0];                // Ustaw cyfrę
      PORTD = ~digitTable[i % 10];       // Ustaw segmenty dla cyfry
      _delay_ms(1);                      // Szybsze przełączanie
      PORTB = ~digits[1];                // Ustaw cyfrę
      PORTD = ~digitTable[(i + 1) % 10]; // Ustaw segmenty dla cyfry
      _delay_ms(1);                      // Szybsze przełączanie
      PORTB = ~digits[2];                // Ustaw cyfrę
      PORTD = ~digitTable[(i + 2) % 10]; // Ustaw segmenty dla cyfry
      _delay_ms(1);                      // Szybsze przełączanie
      PORTB = ~digits[3];                // Ustaw cyfrę
      PORTD = ~digitTable[(i + 3) % 10]; // Ustaw segmenty dla cyfry
      _delay_ms(1000);
    }
  }
}
