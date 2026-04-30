#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR (INT0_vect);
void handle_edit_mode(uint8_t digit, uint8_t tick);
void handle_normal_mode(uint8_t digit, uint8_t tick);
void clear_digit(void);
void write_digit(uint8_t digit);

volatile uint8_t edit_mode = 0;

int main(void) {
  // NOTE: PD2 is used for INT0 now
  DDRD &= ~(1 << DDD2);

  // Enable internal pull-up
  PORTD |= (1 << PORTD2);

  // Configure INT0 trigger sense
  // Trigger INT0 on falling edge (set ISC01 bit to 1 and ISC00 bit to 0)
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  // Enable INT0 in the External Interrupt Mask Register
  EIMSK = (1 << INT0);

  // Enable Global Interrupts
  sei();

  // Set all the segment pins to output
  DDRD |= (1 << DDD3); // b
  DDRD |= (1 << DDD4); // c
  DDRD |= (1 << DDD5); // d
  DDRD |= (1 << DDD6); // e
  DDRD |= (1 << DDD7); // f

  DDRB |= (1 << DDB0); // g
  DDRB |= (1 << DDB1); // DP
  DDRB |= (1 << DDB2); // a

  uint8_t digit = 0;
  uint8_t tick = 0;

  while (1) {
    tick ^= 0b1;

    if (edit_mode & 0b1) {
      handle_edit_mode(digit, tick);
    } else {
      if (digit < 9) {
        digit++;
      } else {
        digit = 0;
      }

      handle_normal_mode(digit, tick);
    }
  }

  return 0;
}

ISR (INT0_vect) {
  edit_mode ^= 0b1;
}

void handle_edit_mode(uint8_t digit, uint8_t tick) {
  // Keep DP on
  PORTB |= (1 << PORTB1);

  if (tick & 0b1) {
    write_digit(digit);
  } else {
    clear_digit();
  }

  _delay_ms(500);
}

void handle_normal_mode(uint8_t digit, uint8_t tick) {
  write_digit(digit);

  if (tick & 0b1) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }

  _delay_ms(1000);
}

void clear_digit(void) {
  PORTD &= ~(1 << PORTD3);
  PORTD &= ~(1 << PORTD4);
  PORTD &= ~(1 << PORTD5);
  PORTD &= ~(1 << PORTD6);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB2);
}

void write_digit(uint8_t digit) {
  if (digit < 0) {
    digit *= -1;
  }

  if (digit > 9) {
    digit = 0;
  }

  // NOTE: PD2 is used for INT0 now
  switch (digit) {
    case 0:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD6);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB2);

      PORTB &= ~(1 << PORTB0);
      break;
    case 1:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);

      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      PORTB &= ~(1 << PORTB0);
      PORTB &= ~(1 << PORTB2);
      break;
    case 2:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD6);
      PORTB |= (1 << PORTB0);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD4);
      PORTD &= ~(1 << PORTD7);
      break;
    case 3:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTB |= (1 << PORTB0);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      break;
    case 4:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);

      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTB &= ~(1 << PORTB2);
      break;
    case 5:
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD3);
      PORTD &= ~(1 << PORTD6);
      break;
    case 6:
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);
      PORTD |= (1 << PORTD6);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD3);
      break;
    case 7:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD5);
      PORTD &= ~(1 << PORTD6);
      PORTD &= ~(1 << PORTD7);
      PORTB &= ~(1 << PORTB0);

      break;
    case 8:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTD |= (1 << PORTD6);
      PORTB |= (1 << PORTB0);
      PORTB |= (1 << PORTB2);

      break;
    case 9:
      PORTD |= (1 << PORTD3);
      PORTD |= (1 << PORTD4);
      PORTD |= (1 << PORTD5);
      PORTD |= (1 << PORTD7);
      PORTB |= (1 << PORTB0);
      PORTB |= (1 << PORTB2);

      PORTD &= ~(1 << PORTD6);
      break;
    default:
      clear_digit();

      break;
  }
}
