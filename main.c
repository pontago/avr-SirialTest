
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>

const unsigned char GETS_BUFFER_SIZE = 128;

void USART_Init(void) {
  UBRRL = 51; // 9600bps
  UCSRB= (1 << RXEN) | (1 << TXEN);
  UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
}

void USART_Transmit(unsigned char data) {
  if (data != '\0') {
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = data;
  }
}

unsigned char USART_Receive(void) {
  if (!(UCSRA << RXC)) {
    return '\0';
  }
  else {
    loop_until_bit_is_set(UCSRA, RXC);
    return UDR;
  }
}

unsigned int print(unsigned char *data) {
  unsigned int count = 0;

  while (*data != '\0') {
    USART_Transmit(*data);
    data++;
    count++;
  }

  return count;
}

unsigned int println(unsigned char *data) {
  unsigned int ret = print(data);
  if (data[0] != '\0') {
    USART_Transmit('\n');
  }
  return ret;
}

unsigned int gets(unsigned char *buffer) {
  unsigned char data;
  unsigned int count = 0;

  memset(buffer, '\0', GETS_BUFFER_SIZE);

  while ((data = USART_Receive()) != '\0') {
    if (data == '\n' || data == '\r') {
      break;
    }

    *buffer = data;
    buffer++;
    count++;
  }

  return count;
}


int main(void) {
  USART_Init();

  unsigned char buffer[GETS_BUFFER_SIZE];
  for (;;) {
    gets(buffer);
    println(buffer);
  }

  return 0;
}
