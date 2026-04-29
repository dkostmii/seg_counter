#include <avr/io.h>
#include <util/delay.h>

void write_digit(uint8_t digit) {
  if (digit < 0) {
    digit *= -1;
  }

  if (digit > 9) {
    digit = 0;
  }

  switch (digit) {
    case 0:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD6);
      PORTD |= (1 << PORTD7);

      PORTB &= ~(1 << PORTB0);
      break;
    case 1:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);

      PORTD &= ~(1 << PORTD2);
      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      PORTB &= ~(1 << PORTB0);
      break;
    case 2:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD6);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD4);
      PORTD &= ~(1 << PORTD7);
      break;
    case 3:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      break;
    case 4:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD2);
      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      break;
    case 5:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD3);
      PORTD &= ~(1 << PORTD6);
      break;
    case 6:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);
      PORTD |= (1 << PORTD6);

      PORTD &= ~(1 << PORTD3);
      break;
    case 7:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);

      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      PORTB &= ~(1 << PORTB0);

      break;
    case 8:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTD |= (1 << PORTD6);
      PORTB |= (1 << PORTB0);

      break;
    case 9:
      PORTD |= (1 << PORTD2);
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD6);
      break;
    default:
      PORTD &= ~(1 << PORTD3);
      PORTD &= ~(1 << PORTD4);
      PORTD &= ~(1 << PORTD2);
      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      PORTB &= ~(1 << PORTB0);
      PORTB &= ~(1 << PORTB1);
      break;
  }
}

int main(void) {
  // Set all the segment pins to output
  DDRD |= (1 << DDD2); // a
  DDRD |= (1 << DDD3); // b
  DDRD |= (1 << DDD4); // c
  DDRD |= (1 << DDD5); // d
  DDRD |= (1 << DDD6); // e
  DDRD |= (1 << DDD7); // f

  DDRB |= (1 << DDB0); // g
  DDRB |= (1 << DDB1); // DP

  uint8_t counter = 0;

  while (1) {
    if (counter < 9) {
      counter++;
    } else {
      counter = 0;
    }

    write_digit(counter);

    if (counter & 0b1) {
      PORTB |= (1 << PORTB1);
    } else {
      PORTB &= ~(1 << PORTB1);
    }

    _delay_ms(1000);
  }

  return 0;
}
