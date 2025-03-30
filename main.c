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
    for (uint8_t i = 0; i < 4; i++) {
      PORTD = ~digitTable[digits[i]]; // Ustaw segmenty dla cyfry
      PORTB = digits[i];              // Włącz tylko jedną cyfrę
      _delay_ms(1);                   // Szybsze przełączanie

      // Debug: Zmień cyfry co 1 sekundę
      if (i == 3) {
        _delay_ms(1000);
        digits[0] = (digits[0] + 1) % 10;
        digits[1] = (digits[1] + 1) % 10;
        digits[2] = (digits[2] + 1) % 10;
        digits[3] = (digits[3] + 1) % 10;
      }
    }
  }
}
