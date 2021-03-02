#pragma once

#include <stdint.h>

// prime should be bigger than the size of the alphabet
// string are in ASCII form alphabet size = 128
#define HT_PRIME_1 167
#define HT_PRIME_2 229
#define HT_INITIAL_BASE_SIZE 50

typedef struct
{
  uint8_t *key; // char * == uint8_t *
  uint8_t *value;
} ht_item;

typedef struct
{
  uint32_t size;
  uint32_t base_size;
  uint32_t count;
  ht_item **items;
} ht_hash_table;


ht_hash_table *
ht_new();

void
ht_del_hash_table(ht_hash_table* ht);

void
ht_insert(ht_hash_table* ht, const uint8_t* key, const uint8_t* value);
uint8_t*
ht_search(ht_hash_table* ht, const uint8_t* key);
void 
ht_delete(ht_hash_table* ht, const uint8_t* key);
