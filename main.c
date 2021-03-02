#include <stdio.h>

#include "hash_table.h"

int
main(void)
{
    ht_hash_table *ht = ht_new();
    
    ht_insert(ht, "cars", "car 1, car 2, car 3");
    ht_insert(ht, "planes", "plane 1, plane 2, plane 3");

    printf("[Test:] - \"%s\"\n", ht_search(ht, "cars"));
    printf("[Test:] - \"%s\"\n", ht_search(ht, "planes"));

    ht_del_hash_table(ht);
}