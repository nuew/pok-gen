#include <stddef.h>
#include <stdio.h>
#include "pokemon.h"

void hexdump(void *addr, size_t len, size_t offset) {
  uint8_t *_addr = (uint8_t *) addr;
  char ascii_buf[17];
  size_t i;

  for(i = 0; i < len; ++i) {
    if((i % 16) == 0) {
      if(i != 0) {
        printf("  %s\n", ascii_buf);
      }

      printf("%08lx:", offset + i);
    }

    printf(" %02x", _addr[i]);

    if((_addr[i] < 0x20) || (_addr[i] > 0x7e)) {
      ascii_buf[i % 16] = '.';
    } else {
      ascii_buf[i % 16] = (char) _addr[i];
    }
    ascii_buf[(i % 16) + 1] = '\0';
  };

  for(; (i % 16) != 0; ++i) {
    printf("   ");
  }

  printf("  %s\n", ascii_buf);
}
