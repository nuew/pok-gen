#include <stdlib.h>
#include <string.h>

#include "pokemon.h"

static void crypt_data(
  uint8_t *buf,
  uint32_t personality,
  uint32_t trainer_id
) {
  uint32_t *data = (uint32_t *) buf;
  uint32_t key = personality ^ trainer_id;

  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
    data[i] ^= key;
  }
}

static uint16_t checksum(uint8_t *buf) {
  uint16_t *data = (uint16_t *) buf;
  uint16_t sum = 0;

  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint16_t)); i++) {
    sum += data[i];
  }

  return sum;
}

uint16_t data_encrypt_to(
  void *dest,
  uint32_t personality,
  uint32_t trainer_id,
  struct Growth growth,
  struct Attacks attacks,
  struct Condition condition,
  struct Misc misc
) {
  uint8_t buf[DATA_LENGTH];
  void *datum[DATUM_PER_DATA];
  uint32_t order = personality % 24;

  // Find data ordering
  switch(order) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    datum[0] = &growth;
    break;
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
    datum[0] = &attacks;
    break;
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
    datum[0] = &condition;
    break;
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
    datum[0] = &misc;
    break;
  default: abort();
  };

  switch(order) {
  case 0:
  case 1:
  case 14:
  case 15:
  case 20:
  case 21:
    datum[1] = &attacks;
    break;
  case 2:
  case 3:
  case 8:
  case 9:
  case 22:
  case 23:
    datum[1] = &condition;
    break;
  case 4:
  case 5:
  case 10:
  case 11:
  case 16:
  case 17:
    datum[1] = &misc;
    break;
  case 6:
  case 7:
  case 12:
  case 13:
  case 18:
  case 19:
    datum[1] = &growth;
    break;
  default: abort();
  };

  switch(order) {
  case 0:
  case 5:
  case 6:
  case 11:
  case 19:
  case 21:
    datum[2] = &condition;
    break;
  case 1:
  case 3:
  case 7:
  case 9:
  case 13:
  case 15:
    datum[2] = &misc;
    break;
  case 2:
  case 4:
  case 12:
  case 17:
  case 18:
  case 23:
    datum[2] = &attacks;
    break;
  case 8:
  case 10:
  case 14:
  case 16:
  case 20:
  case 22:
    datum[2] = &growth;
    break;
  default: abort();
  };

  switch(order) {
  case 0:
  case 2:
  case 6:
  case 8:
  case 12:
  case 14:
    datum[3] = &misc;
    break;
  case 1:
  case 4:
  case 7:
  case 10:
  case 18:
  case 20:
    datum[3] = &condition;
    break;
  case 3:
  case 5:
  case 13:
  case 16:
  case 19:
  case 22:
    datum[3] = &attacks;
    break;
  case 9:
  case 11:
  case 15:
  case 17:
  case 21:
  case 23:
    datum[3] = &growth;
    break;
  default: abort();
  };

  // Assemble and checksum data
  for(size_t i = 0; i < DATUM_PER_DATA; i++) {
    memcpy(buf + (DATUM_LENGTH * i), datum[i], DATUM_LENGTH);
  }
  uint16_t cksum = checksum(buf);

  // Encrypt data
  crypt_data(buf, personality, trainer_id);
  memcpy(dest, buf, DATA_LENGTH);
  return cksum;
}
