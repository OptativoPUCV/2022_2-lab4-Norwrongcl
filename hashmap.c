#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long hashFunction = hash(key, map->capacity);

    while (map->buckets[hashFunction] != NULL && map->buckets[hashFunction]->key != NULL)
    {
        if (is_equal(key, map->buckets[hashFunction]->key) == 1) return;
        hashFunction = (hashFunction+1) % map->capacity;
    }

    if (map->buckets[hashFunction] != NULL)
    {
        map->buckets[hashFunction]->key = key;
        map->buckets[hashFunction]->value = value;
    }
    else
        map->buckets[hashFunction] = createPair(key, value);
    
    map->size++;

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **oldBuckets = map->buckets;
    long oldCapacity = map->capacity;
    map->capacity *= 2;
    map->buckets = (Pair**) malloc (map->capacity * sizeof(Pair*));
    for (int i = 0; i < map->capacity; i++)
    {
        map->buckets[i] = (Pair *) malloc (sizeof(Pair));
        map->buckets[i] = NULL;
    }
    
    map->size = 0;

    for (int i = 0; i < oldCapacity; i++)
    {
        if (oldBuckets[i] != NULL)
            insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
    }
}


HashMap * createMap(long capacity){
	HashMap *map= (HashMap *)malloc(sizeof(HashMap));
	map->buckets= (Pair **)calloc(capacity,sizeof(Pair **));
	map->capacity= capacity;
	map->size= 0;
	map->current= -1;
  return map;
}

void eraseMap(HashMap * map,  char * key){
	long hashFunction = hash(key, map->capacity);

    while (map->buckets[hashFunction] != NULL && map->buckets[hashFunction]->key != NULL)
    {
        if (is_equal(key, map->buckets[hashFunction]->key) == 1)
        {
            map->buckets[hashFunction]->key = NULL;
            break;
        }
        hashFunction = (hashFunction+1) % map->capacity;
    }

    map->size--;
}

Pair * searchMap(HashMap * map,  char * key){
	long hashFunction = hash(key, map->capacity);
    Pair* searchedElement = NULL;
    while (map->buckets[hashFunction] != NULL && map->buckets[hashFunction]->key != NULL)
    {
        if (is_equal(key, map->buckets[hashFunction]->key) == 1)
        {
            searchedElement = map->buckets[hashFunction];
            map->current = hashFunction;
            return searchedElement;
        }
        hashFunction = (hashFunction+1) % map->capacity;
    }

    map->current = hashFunction;
    return NULL;
}

Pair * firstMap(HashMap * map){
	for (int i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
 	for (int i = map->current+1; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
