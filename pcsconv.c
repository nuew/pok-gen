#include "pokemon.h"

#define CCV(c, h) case (c): text[i] = (char)(h); break
bool pcsconv(char *text, size_t len, uint16_t language) {
  if(language != LANGUAGE_ENGLISH) return false;

  for(size_t i = 0; i < len; i++) {
    switch(text[i]) {
    CCV(' ', 0x00);
    CCV('&', 0x2d);
    CCV('+', 0x2e);
    CCV('=', 0x35);
    CCV('%', 0x5b);
    CCV('(', 0x5c);
    CCV(')', 0x5d);
    CCV('0', 0xa1);
    CCV('1', 0xa2);
    CCV('2', 0xa3);
    CCV('3', 0xa4);
    CCV('4', 0xa5);
    CCV('5', 0xa6);
    CCV('6', 0xa7);
    CCV('7', 0xa8);
    CCV('8', 0xa9);
    CCV('9', 0xaa);
    CCV('!', 0xab);
    CCV('?', 0xac);
    CCV('.', 0xad);
    CCV('-', 0xae);
    CCV(',', 0xb8);
    CCV('/', 0xba);
    CCV('A', 0xbb);
    CCV('B', 0xbc);
    CCV('C', 0xbd);
    CCV('D', 0xbe);
    CCV('E', 0xbf);
    CCV('F', 0xc0);
    CCV('G', 0xc1);
    CCV('H', 0xc2);
    CCV('I', 0xc3);
    CCV('J', 0xc4);
    CCV('K', 0xc5);
    CCV('L', 0xc6);
    CCV('M', 0xc7);
    CCV('N', 0xc8);
    CCV('O', 0xc9);
    CCV('P', 0xca);
    CCV('Q', 0xcb);
    CCV('R', 0xcc);
    CCV('S', 0xcd);
    CCV('T', 0xce);
    CCV('U', 0xcf);
    CCV('V', 0xd0);
    CCV('W', 0xd1);
    CCV('X', 0xd2);
    CCV('Y', 0xd3);
    CCV('Z', 0xd4);
    CCV('a', 0xd5);
    CCV('b', 0xd6);
    CCV('c', 0xd7);
    CCV('d', 0xd8);
    CCV('e', 0xd9);
    CCV('f', 0xda);
    CCV('g', 0xdb);
    CCV('h', 0xdc);
    CCV('i', 0xdd);
    CCV('j', 0xde);
    CCV('k', 0xdf);
    CCV('l', 0xe0);
    CCV('m', 0xe1);
    CCV('n', 0xe2);
    CCV('o', 0xe3);
    CCV('p', 0xe4);
    CCV('q', 0xe5);
    CCV('r', 0xe6);
    CCV('s', 0xe7);
    CCV('t', 0xe8);
    CCV('u', 0xe9);
    CCV('v', 0xea);
    CCV('w', 0xeb);
    CCV('x', 0xec);
    CCV('y', 0xed);
    CCV('z', 0xee);
    CCV(':', 0xf0);
    CCV('\n', 0xfe);
    CCV('\0', 0xff);
    default:
      return false;
    }
  }

  return true;
}

