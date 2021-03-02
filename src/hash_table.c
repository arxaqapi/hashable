#include "hash_table.h"
#include "prime.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static ht_item HT_DELETED_ITEM = {NULL, NULL};

//The function is marked as static because it will only ever be called
// by code internal to the hash table.
/**
 * @brief Creates and allocates memory for a new item
 * 
 * @param k key string 
 * @param v value string
 * @return ht_item* pointer to the newly created ht_item
 */
static ht_item *
ht_new_item(const uint8_t *k, const uint8_t *v)
{
    ht_item *e = calloc(1, sizeof(ht_item));
    e->key = strdup(k);   // duplicate k
    e->value = strdup(v); // duplicate k
    return e;
}

static ht_hash_table*
ht_new_sized(const uint32_t base_size)
{
    ht_hash_table *ht = calloc(1, sizeof(ht_hash_table));
    ht->base_size = base_size;
    // (50 << size_index)
    ht->size = next_prime(base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
}

ht_hash_table *
ht_new()
{
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static void 
ht_resize(ht_hash_table* ht, const uint32_t base_size)
{
    if (base_size < HT_INITIAL_BASE_SIZE)
        return; // don't resize

    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (uint32_t i = 0; i < ht->size; i++)
    {
        ht_item* item = ht->items[i];
        if(item != NULL && item != &HT_DELETED_ITEM)
        {
            ht_insert(new_ht, item->key, item->value);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items (swaperidoo)
    const uint32_t temp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = temp_size;

    ht_item** temp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = temp_items;

    ht_del_hash_table(new_ht);
}

static void
ht_resize_up(ht_hash_table* ht)
{
    ht_resize(ht, ht->base_size * 2);
}

static void
ht_resize_down(ht_hash_table* ht)
{
    // int t = ht->base_size / 2;
    ht_resize(ht, ht->base_size / 2);
}



static void
ht_del_item(ht_item *e)
{
    free(e->key);
    free(e->value);
    free(e);
}

void ht_del_hash_table(ht_hash_table *ht)
{
    for (uint32_t i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

static uint64_t
ht_hash(const uint8_t *s, const uint32_t a, const uint32_t m)
{
    uint64_t hash = 0;
    const uint32_t len_s = strlen(s);
    for (uint32_t i = 0; i < len_s; i++)
    {
        hash += (uint64_t)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (uint64_t)hash;
}

static uint64_t
ht_get_hash(const uint8_t *s, const uint32_t num_buckets, const uint32_t attempt)
{
    const uint32_t hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const uint32_t hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void
ht_insert(ht_hash_table* ht, const uint8_t* key, const uint8_t* value)
{
    // check if necessary to resize
    const uint32_t load = ht->count * 100 / ht->size;
    if(load > 70)
        ht_resize_up(ht);

    ht_item * item = ht_new_item(key, value);
    uint64_t index = ht_get_hash(item->key, ht->size, 0);
    ht_item * cur_item = ht->items[index];
    uint32_t i = 1;
    // "skip" / "jump over" deleted nodes
    while (cur_item != NULL)
    {
        if (cur_item != &HT_DELETED_ITEM && strcmp(cur_item->key, key) == 0)
        {
            ht_del_item(cur_item);
            ht->items[index] = item;
            return;
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i ++;
    }
    ht->items[index] = item;
    ht->count ++; 
}

u_int8_t*
ht_search(ht_hash_table* ht, const u_int8_t* key)
{
    uint64_t index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    uint32_t i = 1;
    while (item != NULL)
    {
        if ( item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0 )
        {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i ++;
    }
    return NULL;
}

void 
ht_delete(ht_hash_table* ht, const u_int8_t* key)
{
    // check if necessary to resize
    const uint32_t load = ht->count * 100 / ht->size;
    if (load < 10)
        ht_resize_down(ht);

    uint32_t index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    uint32_t i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i ++;
    }
    ht->count --;
}