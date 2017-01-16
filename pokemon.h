#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct PPBonus {
  uint8_t move1:2;
  uint8_t move2:2;
  uint8_t move3:2;
  uint8_t move4:2;
};

struct Growth {
  uint16_t species; // bulbapedia:List_of_Pokémon_by_index_number_(Generation_III)
  uint16_t held_item; // bulbapedia:List_of_items_by_index_number_(Generation_III)
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
  uint8_t met_location; // bulbapedia:List_of_locations_by_index_number_(Generation_III)
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
