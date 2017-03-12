/* Generation 3 Pokémon Data Structure Generator
 * Copyright © 2017 Ethan Welker (nuew)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct PPBonus {
  uint8_t move1:2;
  uint8_t move2:2;
  uint8_t move3:2;
  uint8_t move4:2;
};

struct Growth {
  uint16_t species; 
  uint16_t held_item;
  uint32_t experience;
  struct PPBonus pp_bonus;
  uint8_t friendship;
  uint16_t unknown;
};

struct Attacks {
  uint16_t moves[4];
  uint8_t pp[4];
};

struct Condition {
  uint8_t hp_ev;
  uint8_t attack_ev;
  uint8_t defense_ev;
  uint8_t speed_ev;
  uint8_t special_attack_ev;
  uint8_t special_defense_ev;
  uint8_t coolness;
  uint8_t beauty;
  uint8_t cuteness;
  uint8_t smartness;
  uint8_t toughness;
  uint8_t feel;
};

struct Pokerus {
  uint8_t days_remaining:4;
  uint8_t strain:4;
};

#define GAME_COLOSSEUM_BONUS 0
#define GAME_SAPPHIRE 1
#define GAME_RUBY 2
#define GAME_EMERALD 3
#define GAME_FIRERED 4
#define GAME_LEAFGREEN 5
#define GAME_COLOSSEUM_XD 15

#define POKEBALL_MASTER 1
#define POKEBALL_ULTRA 2
#define POKEBALL_GREAT 3
#define POKEBALL_STANDARD 4
#define POKEBALL_SAFARI 5
#define POKEBALL_DIVE 7
#define POKEBALL_NEST 8
#define POKEBALL_REPEAT 9
#define POKEBALL_TIMER 10
#define POKEBALL_LUXURY 11
#define POKEBALL_PREMIER 12

#define TRAINER_MALE 0
#define TRAINER_FEMALE 1

struct Origins {
  uint16_t level_met:7;
  uint16_t game_met:4;
  uint16_t pokeball_type:4;
  uint16_t trainer_gender:1;
};

#define ABILITY_PRIMARY 0
#define ABILITY_SECONDARY 1

struct IVs {
  uint32_t hp:5;
  uint32_t attack:5;
  uint32_t defense:5;
  uint32_t speed:5;
  uint32_t special_attack:5;
  uint32_t special_defense:5;
  uint32_t egg:1;
  uint32_t ability:1;
};

struct Ribbons {
  uint32_t cool:3;
  uint32_t beauty:3;
  uint32_t cute:3;
  uint32_t smart:3;
  uint32_t tough:3;
  uint32_t champion:1;
  uint32_t winning:1;
  uint32_t victory:1;
  uint32_t artist:1;
  uint32_t effort:1;
  uint32_t special1:1;
  uint32_t special2:1;
  uint32_t special3:1;
  uint32_t special4:1;
  uint32_t special5:1;
  uint32_t special6:1;
  uint32_t padding:5;
  uint32_t obedience:1;
};

struct Misc {
  struct Pokerus pokerus;
  uint8_t met_location;
  struct Origins origins;
  struct IVs ivs;
  struct Ribbons ribbons;
};

/* Pokémon struct and associated constants */
#define NICKNAME_LENGTH (10)
#define TRAINER_NAME_LENGTH (7)
#define DATA_LENGTH (48)
#define DATUM_LENGTH (12)
#define DATUM_PER_DATA (DATA_LENGTH / DATUM_LENGTH)

#define LANGUAGE_JAPANESE 0x0201
#define LANGUAGE_ENGLISH 0x0202
#define LANGUAGE_FRENCH 0x0203
#define LANGUAGE_ITALIAN 0x0204
#define LANGUAGE_GERMAN 0x0205
#define LANGUAGE_KOREAN 0x0206
#define LANGUAGE_SPANISH 0x0207

#define MARKING_BULLET 0
#define MARKING_SQUARE 1
#define MARKING_TRIANGLE 2
#define MARKING_HEART 3

struct Status {
  uint32_t sleep:3;
  uint32_t poisoned:1;
  uint32_t burnt:1;
  uint32_t frozen:1;
  uint32_t paralyzed:1;
  uint32_t bad_poisoned:1;
};

struct Pokemon {
  uint32_t personality;
  uint32_t trainer_id;
  char nickname[NICKNAME_LENGTH];
  uint16_t language;
  char trainer_name[TRAINER_NAME_LENGTH];
  uint8_t markings;
  uint16_t checksum;
  uint16_t unknown;
  uint8_t data[DATA_LENGTH];
  struct Status status;
  uint8_t level;
  uint8_t pokerus;
  uint16_t current_health;
  uint16_t max_health;
  uint16_t attack;
  uint16_t defense;
  uint16_t speed;
  uint16_t special_attack;
  uint16_t special_defense;
};

void hexdump(void *addr, size_t len, size_t offset);
bool pcsconv(char *text, size_t len, uint16_t language);
uint16_t data_encrypt_to(
  void *dest,
  uint32_t personality,
  uint32_t trainer_id,
  struct Growth growth,
  struct Attacks attacks,
  struct Condition condition,
  struct Misc misc
);

// Encrypt the pokémon structures together
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

  // Assemble data
  for(size_t i = 0; i < DATUM_PER_DATA; i++) {
    memcpy(buf + (DATUM_LENGTH * i), datum[i], DATUM_LENGTH);
  }

  // Checksum data
  uint16_t cksum = 0;
  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint16_t)); i++) {
    cksum += ((uint16_t *) buf)[i];
  }

  // Encrypt data
  uint32_t key = personality ^ trainer_id;
  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
    ((uint32_t *) buf)[i] ^= key;
  }
  memcpy(dest, buf, DATA_LENGTH);
  return cksum;
}

// Output block addr, of size len, as an xxd style hexdump.
// offset is the offset of the starting bit numbers
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

// Convert a string into the Pokémon proprietary character set
// returns false if character in string isn't in that set
// (or is a japanese character not included in our mapping)
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
#undef CCV

int main(int argc, char **argv) {
  // Ensure structs have correct sizes
  assert(sizeof(struct PPBonus) == 1);
  assert(sizeof(struct Growth) == 12);
  assert(sizeof(struct Attacks) == 12);
  assert(sizeof(struct Condition) == 12);
  assert(sizeof(struct Pokerus) == 1);
  assert(sizeof(struct Origins) == 2);
  assert(sizeof(struct IVs) == 4);
  assert(sizeof(struct Ribbons) == 4);
  assert(sizeof(struct Misc) == 12);
  assert(sizeof(struct Status) == 4);
  assert(sizeof(struct Pokemon) == 100);

  // Init RNG
  srand((unsigned int) time(NULL));

  // Construct Structure
  struct Growth growth = {
    .species = 1,
    .held_item = 0,
    .experience = 0,
    .pp_bonus = (struct PPBonus) {
      .move1 = 0,
      .move2 = 0,
      .move3 = 0,
      .move4 = 0
    },
    .friendship = 0xff,
    .unknown = 0,
  };

  struct Attacks attacks = {
    .moves = {0, 0, 0, 0},
    .pp = {0, 0, 0, 0}
  };

  struct Condition condition = {
    .hp_ev = 0xff,
    .attack_ev = 0xff,
    .defense_ev = 0xff,
    .speed_ev = 0xff,
    .special_attack_ev = 0xff,
    .special_defense_ev = 0xff,
    .coolness = 0xff,
    .beauty = 0xff,
    .cuteness = 0xff,
    .smartness = 0xff,
    .toughness = 0xff,
    .feel = 0,
  };

  struct Misc misc = {
    .pokerus = (struct Pokerus) {
      .days_remaining = 0x0,
      .strain = 0x0,
    },
    .met_location = 0xff, // Fateful Encounter
    .origins = (struct Origins) {
      .level_met = 1,
      .game_met = GAME_SAPPHIRE,
      .pokeball_type = POKEBALL_STANDARD,
      .trainer_gender = TRAINER_MALE
    },
    .ivs = (struct IVs) {
      .hp = 0x1f,
      .attack = 0x1f,
      .defense = 0x1f,
      .speed = 0x1f,
      .special_attack = 0x1f,
      .special_defense = 0x1f,
      .egg = 0,
      .ability = ABILITY_PRIMARY
    },
    .ribbons = (struct Ribbons) {
      .cool = 0,
      .beauty = 0,
      .cute = 0,
      .smart = 0,
      .tough = 0,
      .champion = 0,
      .winning = 0,
      .victory = 0,
      .artist = 0,
      .effort = 0,
      .special1 = 0,
      .special2 = 0,
      .special3 = 0,
      .special4 = 0,
      .special5 = 0,
      .special6 = 0,
      .padding = 0,
      .obedience = 0
    }
  };

  struct Pokemon pkmn = {
    .personality = (unsigned int) rand(),
    .trainer_id = (unsigned int) rand(),
    .language = 0x0202,
    .markings = MARKING_BULLET,
    .unknown = 0,
    .status = (struct Status) {
      .sleep = 0,
      .poisoned = 0,
      .burnt = 0,
      .frozen = 0,
      .paralyzed = 0,
      .bad_poisoned = 0
    },
    .level = 1,
    .pokerus = 0,
    .current_health = 0xff,
    .max_health = 0xff,
    .attack = 0xff,
    .defense = 0xff,
    .speed = 0xff,
    .special_attack = 0xff,
    .special_defense = 0xff
  };

  // Parse Options
  static const char optstring[] = 
    "12a:A:b:B:c:C:d:D:e:E:f:F:gG:hH:i:I:j:k:K:l:L:"
    "m:M:n:N:oOp:P:q:Q:r:R:s:S:t:T:u:U:v:V:x:y:Y:";
  const char usage[] = 
    "Usage: %s [options] <trainer name> <pokémon name>\n"
    "\n"
    "\t-s, --species <index>    The index number of the pokémon's species.\n"
    "\t                         The default is 1 [Bulbasaur].\n"
    "\t-i, --item <index>       The index number of the item to be held.\n"
    "\t                         The default is 0 [Nothing].\n"
    "\t-x, --experience <int>   The amount of experience that the pokémon shall have.\n"
    "\t                         The default is 0.\n"
    "\t-B, --pp-bonus <a>:<b>:<c>:<d>   A set of values from 1-3 of how many\n"
    "\t                                 PP-Bonuses shall have been applied to\n"
    "\t                                 each move slot. Each defaults to 0.\n"
    "\t-f, --friendship <int>   The friendship of the generated pokémon.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-m, --moves <a>:<b>:<c>:<d>  The index numbers of the moves the pokémon shall have.\n"
    "\t-P, --moves-pp <a>:<b>:<c>:<d>   The current PP of each move slot.\n"
    "\t-j, --ev-hp <int>        The pokémon's HP effort value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-v, --ev-attack <int>    The pokémon's attack effort value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-e, --ev-defense <int>   The pokémon's defense effort value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-V, --ev-speed <int>     The pokémon's speed effort value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-K, --ev-special-attack <int>    The pokémon's special attack effort value.\n"
    "\t                                 Must be between 0-255; the default is 255.\n"
    "\t-E, --ev-special-defense <int>   The pokémon's HP effort value.\n"
    "\t                                 Must be between 0-255; the default is 255.\n"
    "\t-c, --coolness <int>   The pokémon's coolness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-y, --beauty <int>     The pokémon's beauty condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-C, --cuteness <int>   The pokémon's cuteness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-r, --smartness <int>  The pokémon's smartness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-T, --toughness <int>  The pokémon's toughness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-F, --feel <int>       The pokémon's feel (also known as luster).\n"
    "\t                       Must be between 0-255; the default is 0.\n"
    "\t-R, --pokerus <days remaining>:<strain>  Set pokérus status.\n"
    "\t                       Both may be between 0-15; the defaults are 0.\n"
    "\t-k, --met-location <index>   Set the index of the location the pokémon was met at.\n"
    "\t                             The default is 255 [fateful encounter].\n"
    "\t-M, --met-level <int>    Set the level the pokémon was met at.\n"
    "\t                         Must be between 1-100; the default is 1.\n"
    "\t-G, --met-game <colosseum-bonus|sapphire|ruby|emerald|firered|leafgreen|colosseum-xd>\n"
    "\t                         Set the game the pokémon was met in.\n"
    "\t-b, --pokeball <master|ultra|great|standard|safari|dive|nest|repeat|timer|luxury|premier>\n"
    "\t                         Set the pokéball that the pokémon was caught in.\n"
    "\t-H, --iv-hp <int>        The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-a, --iv-attack <int>    The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-d, --iv-defense <int>   The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-S, --iv-speed <int>     The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-A, --iv-special-attack <int>    The pokémon's HP initial value.\n"
    "\t                                 Must be between 0-255; the default is 255.\n"
    "\t-D, --iv-special-defense <int>   The pokémon's HP initial value.\n"
    "\t                                 Must be between 0-255; the default is 255.\n"
    "\t-g, --egg                The pokémon will be generated as an egg.\n"
    "\t-1, --ability-primary    The pokémon will use it's primary ability.\n"
    "\t-2, --ability-secondary  The pokémon will use it's secondary ability.\n"
    "\t-p, --personality <personality>  The pokémon's personality.\n"
    "\t                                 Must be an integer between 0-4,294,967,295.\n"
    "\t                                 The default is a random number in that range.\n"
    "\t-t, --trainer <trainer id>:<male|female>   The pokémon's trainer's id and gender.\n"
    "\t                                           The trainer ID must be an integer between\n"
    "\t                                           0-4,294,967,295. The default is a random number\n"
    "\t                                           in that range and male.\n"
    "\t-N, --met-language <ja|en|fr|it|de|ko|es>  The language the pokémon was met in.\n"
    "\t                                           The default is en [English].\n"
    "\t-l, --level <int>          The pokémon's level.\n"
    "\t                           Must be between 1-100; the default is 1.\n"
    "\t-Y, --pokerus-left <int>   Time remaining in pokérus infection.\n"
    "\t-L, --hp <int>             Current HP. Must be between 0-255; the default is 255.\n"
    "\t-n, --max-hp <int>         Maximum HP. Must be between 0-255; the default is 255.\n"
    "\t-q, --attack <int>         Attack stat. Must be between 0-255; the default is 255.\n"
    "\t-u, --defense <int>        Defense stat. Must be between 0-255; the default is 255.\n"
    "\t-I, --speed <int>          Speed stat. Must be between 0-255; the default is 255.\n"
    "\t-Q, --special-attack <int>   Special Attack stat.\n"
    "\t                             Must be between 0-255; the default is 255.\n"
    "\t-U, --special-defense <int>  Special Defense stat.\n"
    "\t                             Must be between 0-255; the default is 255.\n"
    "\t-o, --raw                  Output as raw bytes.\n"
    "\t-O, --dump                 Output as a hexdump.\n"
    "\t-h, --help                 Display this message.\n"
    "\n";
  static struct option long_options[] = {
    {"species", required_argument, NULL, 's'},
    {"item", required_argument, NULL, 'i'},
    {"experience", required_argument, NULL, 'x'},
    {"pp-bonus", required_argument, NULL, 'B'},
    {"friendship", required_argument, NULL, 'f'},
    {"moves", required_argument, NULL, 'm'},
    {"moves-pp", required_argument, NULL, 'P'},
    {"ev-hp", required_argument, NULL, 'j'},
    {"ev-attack", required_argument, NULL, 'v'},
    {"ev-defense", required_argument, NULL, 'e'},
    {"ev-speed", required_argument, NULL, 'V'},
    {"ev-special-attack", required_argument, NULL, 'K'},
    {"ev-special-defense", required_argument, NULL, 'E'},
    {"coolness", required_argument, NULL, 'c'},
    {"beauty", required_argument, NULL, 'y'},
    {"cuteness", required_argument, NULL, 'C'},
    {"smartness", required_argument, NULL, 'r'},
    {"toughness", required_argument, NULL, 'T'},
    {"feel", required_argument, NULL, 'F'},
    {"pokerus", required_argument, NULL, 'R'},
    {"met-location", required_argument, NULL, 'k'},
    {"met-level", required_argument, NULL, 'M'},
    {"met-game", required_argument, NULL, 'G'},
    {"pokeball", required_argument, NULL, 'b'},
    {"iv-hp", required_argument, NULL, 'H'},
    {"iv-attack", required_argument, NULL, 'a'},
    {"iv-defense", required_argument, NULL, 'd'},
    {"iv-speed", required_argument, NULL, 'S'},
    {"iv-special-attack", required_argument, NULL, 'A'},
    {"iv-special-defense", required_argument, NULL, 'D'},
    {"egg", no_argument, NULL, 'g'},
    {"ability-primary", no_argument, NULL, '1'},
    {"ability-secondary", no_argument, NULL, '2'},
    {"personality", required_argument, NULL, 'p'},
    {"trainer", required_argument, NULL, 't'},
    {"met-language", required_argument, NULL, 'N'},
    {"level", required_argument, NULL, 'l'},
    {"pokerus-left", required_argument, NULL, 'Y'},
    {"hp", required_argument, NULL, 'L'},
    {"max-hp", required_argument, NULL, 'n'},
    {"attack", required_argument, NULL, 'q'},
    {"defense", required_argument, NULL, 'u'},
    {"speed", required_argument, NULL, 'I'},
    {"special-attack", required_argument, NULL, 'Q'},
    {"special-defense", required_argument, NULL, 'U'},
    {"raw", no_argument, NULL, 'o'},
    {"dump", no_argument, NULL, 'O'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
  };

  if(argc <= 2) {
    fprintf(stderr, usage, argv[0]);
    return 1;
  }

  int c;
  bool dump = true;
  while((c = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
    switch(c) {
    case 's': // species; see bulbapedia:List_of_Pokémon_by_index_number_(Generation_III)
      growth.species = (uint16_t) atoi(optarg);
      break;
    case 'i': // held item; see bulbapedia:List_of_items_by_index_number_(Generation_III)
      growth.held_item = (uint16_t) atoi(optarg);
      break;
    case 'x': // experience
      growth.experience = (uint32_t) atoi(optarg);
      break;
    case 'B': // pp bonuses
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");

        growth.pp_bonus.move1 = (uint16_t) atoi(one);
        growth.pp_bonus.move2 = (uint16_t) atoi(two);
        growth.pp_bonus.move3 = (uint16_t) atoi(three);
        growth.pp_bonus.move4 = (uint16_t) atoi(four);
      }
      break;
    case 'f': // friendship
      growth.friendship = (uint8_t) atoi(optarg);
      break;
    case 'm': // moves
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");

        attacks.moves[0] = (uint16_t) atoi(one);
        attacks.moves[1] = (uint16_t) atoi(two);
        attacks.moves[2] = (uint16_t) atoi(three);
        attacks.moves[3] = (uint16_t) atoi(four);
      }
      break;
    case 'P': // pp for moves
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");

        attacks.pp[0] = (uint8_t) atoi(one);
        attacks.pp[1] = (uint8_t) atoi(two);
        attacks.pp[2] = (uint8_t) atoi(three);
        attacks.pp[3] = (uint8_t) atoi(four);
      }
      break;
    case 'j': // hp ev
      condition.hp_ev = (uint8_t) atoi(optarg);
      break;
    case 'v': // attack ev
      condition.attack_ev = (uint8_t) atoi(optarg);
      break;
    case 'e': // defense ev
      condition.defense_ev = (uint8_t) atoi(optarg);
      break;
    case 'V': // speed ev
      condition.speed_ev = (uint8_t) atoi(optarg);
      break;
    case 'K': // special attack ev
      condition.special_attack_ev = (uint8_t) atoi(optarg);
      break;
    case 'E': // special defense ev
      condition.special_defense_ev = (uint8_t) atoi(optarg);
      break;
    case 'c': // coolness
      condition.coolness = (uint8_t) atoi(optarg);
      break;
    case 'y': // beauty
      condition.beauty = (uint8_t) atoi(optarg);
      break;
    case 'C': // cuteness
      condition.cuteness = (uint8_t) atoi(optarg);
      break;
    case 'r': // smartness
      condition.smartness = (uint8_t) atoi(optarg);
      break;
    case 'T': // toughness
      condition.toughness = (uint8_t) atoi(optarg);
      break;
    case 'F': // feel
      condition.feel = (uint8_t) atoi(optarg);
      break;
    case 'R': // pokérus (days remaining:strain)
      {
        char *days = strtok(optarg, ":");
        char *strain = strtok(NULL, ":");

        misc.pokerus.days_remaining = atoi(days);
        misc.pokerus.strain = atoi(strain);
      }
      break;
    case 'k': // location met at; see bulbapedia:List_of_locations_by_index_number_(Generation_III)
      misc.met_location = (uint8_t) atoi(optarg);
      break;
    case 'M': // level met at
      misc.origins.level_met = atoi(optarg);
      break;
    case 'G': // game met in
      if(!strcmp(optarg, "colosseum-bonus")) {
        misc.origins.game_met = GAME_COLOSSEUM_BONUS;
      } else if(!strcmp(optarg, "sapphire")) {
        misc.origins.game_met = GAME_SAPPHIRE;
      } else if(!strcmp(optarg, "ruby")) {
        misc.origins.game_met = GAME_RUBY;
      } else if(!strcmp(optarg, "emerald")) {
        misc.origins.game_met = GAME_EMERALD;
      } else if(!strcmp(optarg, "firered")) {
        misc.origins.game_met = GAME_FIRERED;
      } else if(!strcmp(optarg, "leafgreen")) {
        misc.origins.game_met = GAME_LEAFGREEN;
      } else if(!strcmp(optarg, "colosseum-xd")) {
        misc.origins.game_met = GAME_COLOSSEUM_XD;
      } else {
        fputs("game must be one of colosseum-bonus|sapphire|ruby|emerald|"
              "firered|leafgreen|colosseum-xd\n", stderr);
        return 1;
      }
      break;
    case 'b': // pokeball used
      if(!strcmp(optarg, "master")) {
        misc.origins.pokeball_type = POKEBALL_MASTER;
      } else if(!strcmp(optarg, "ultra"))  {
        misc.origins.pokeball_type = POKEBALL_ULTRA;
      } else if(!strcmp(optarg, "great"))  {
        misc.origins.pokeball_type = POKEBALL_GREAT;
      } else if(!strcmp(optarg, "standard")) {
        misc.origins.pokeball_type = POKEBALL_STANDARD;
      } else if(!strcmp(optarg, "safari")) {
        misc.origins.pokeball_type = POKEBALL_SAFARI;
      } else if(!strcmp(optarg, "dive")) {
        misc.origins.pokeball_type = POKEBALL_DIVE;
      } else if(!strcmp(optarg, "nest")) {
        misc.origins.pokeball_type = POKEBALL_NEST;
      } else if(!strcmp(optarg, "repeat")) {
        misc.origins.pokeball_type = POKEBALL_REPEAT;
      } else if(!strcmp(optarg, "timer")) {
        misc.origins.pokeball_type = POKEBALL_TIMER;
      } else if(!strcmp(optarg, "luxury")) {
        misc.origins.pokeball_type = POKEBALL_LUXURY;
      } else if(!strcmp(optarg, "premier")) {
        misc.origins.pokeball_type = POKEBALL_PREMIER;
      } else {
        fputs("pokeball must be one of master|ultra|great|standard|safari|"
              "dive|nest|repeat|timer|luxury|premier\n", stderr);
        return 1;
      }
      break;
    case 'H': // hp iv
      misc.ivs.hp = atoi(optarg);
      break;
    case 'a': // attack iv
      misc.ivs.attack = atoi(optarg);
      break;
    case 'd': // defense iv
      misc.ivs.defense = atoi(optarg);
      break;
    case 'S': // speed iv
      misc.ivs.speed = atoi(optarg);
      break;
    case 'A': // special attack iv
      misc.ivs.special_attack = atoi(optarg);
      break;
    case 'D': // special defense iv
      misc.ivs.special_defense = atoi(optarg);
      break;
    case 'g': // is an egg?
      misc.ivs.egg = 1;
      break;
    case '1': // use primary ability (default)
      misc.ivs.ability = ABILITY_PRIMARY;
      break;
    case '2': // use secondary ability
      misc.ivs.ability = ABILITY_SECONDARY;
      break;
    case 'p': // personality
      pkmn.personality = (uint32_t) atoi(optarg);
      break;
    case 't': // trainer id:trainer gender
      {
        char *tid = strtok(optarg, ":");
        char *gender = strtok(NULL, ":");

        pkmn.trainer_id = (uint32_t) atoi(tid);

        if(!strcmp(gender, "male")) {
          misc.origins.trainer_gender = TRAINER_MALE;
        } else if(!strcmp(gender, "female")) {
          misc.origins.trainer_gender = TRAINER_FEMALE;
        } else {
          fputs("gender must be 'male' or 'female'\n", stderr);
          return 1;
        }
      }
      break;
    case 'N': // language met in
      {
        if(!strcmp(optarg, "ja")) {
          pkmn.language = LANGUAGE_JAPANESE;
        } else if(!strcmp(optarg, "en")) {
          pkmn.language = LANGUAGE_ENGLISH;
        } else if(!strcmp(optarg, "fr")) {
          pkmn.language = LANGUAGE_FRENCH;
        } else if(!strcmp(optarg, "it")) {
          pkmn.language = LANGUAGE_ITALIAN;
        } else if(!strcmp(optarg, "de")) {
          pkmn.language = LANGUAGE_GERMAN;
        } else if(!strcmp(optarg, "ko")) {
          pkmn.language = LANGUAGE_KOREAN;
        } else if(!strcmp(optarg, "es")) {
          pkmn.language = LANGUAGE_SPANISH;
        } else {
          fputs("language must be one of ja|en|fr|it|de|ko|es\n", stderr);
          return 1;
        }
      }
      break;
    case 'l': // pokemon level (recalculated on game save/load)
      pkmn.level = (uint8_t) atoi(optarg);
      break;
    case 'Y': // pokérus remaining cache
      pkmn.pokerus = (uint8_t) atoi(optarg);
      break;
    case 'L': // current health
      pkmn.current_health = (uint16_t) atoi(optarg);
      break;
    case 'n': // max health cache
      pkmn.max_health = (uint16_t) atoi(optarg);
      break;
    case 'q': // attack cache
      pkmn.attack = (uint16_t) atoi(optarg);
      break;
    case 'u': // defense cache
      pkmn.defense = (uint16_t) atoi(optarg);
      break;
    case 'I': // speed cache
      pkmn.speed = (uint16_t) atoi(optarg);
      break;
    case 'Q': // special attack cache
      pkmn.special_attack = (uint16_t) atoi(optarg);
      break;
    case 'U': // special defense cache
      pkmn.special_defense = (uint16_t) atoi(optarg);
      break;
    case 'o': // dump raw
      dump = false;
      break;
    case 'O': // hexdump
      dump = true;
      break;
    case 'h':
    default:
      fprintf(stderr, usage, argv[0]);
      return 0;
    }
  }

  // copy positional arguments  
  char nickname[NICKNAME_LENGTH], trainer_name[TRAINER_NAME_LENGTH];

  strncpy(nickname, argv[optind], NICKNAME_LENGTH);
  nickname[NICKNAME_LENGTH-1] = 0;

  strncpy(trainer_name, argv[optind + 1], TRAINER_NAME_LENGTH);
  nickname[TRAINER_NAME_LENGTH-1] = 0;

  // Finalize structure
  assert(pcsconv(nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);
  memcpy(pkmn.nickname, nickname, NICKNAME_LENGTH);

  assert(pcsconv(trainer_name, TRAINER_NAME_LENGTH, LANGUAGE_ENGLISH) == true);
  memcpy(pkmn.trainer_name, trainer_name, TRAINER_NAME_LENGTH);

  pkmn.checksum = data_encrypt_to(
    pkmn.data,
    pkmn.personality,
    pkmn.trainer_id,
    growth,
    attacks,
    condition,
    misc
  );

  if(dump) {
    hexdump(&pkmn, sizeof(struct Pokemon), 0x03004360 + 100);
  } else {
    fwrite(&pkmn, sizeof(struct Pokemon), 1, stdout);
  }
}
