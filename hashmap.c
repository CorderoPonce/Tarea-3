#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;


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


void insertMap(HashMap * map, void * key, void * value) {
  int pos = hash(key, map -> capacity);

  
  while(map -> buckets[pos] != NULL && strcmp(map->buckets[pos] -> key, key) != 0){
    pos = ((pos + 1) % map -> capacity);
  }
  if(map -> buckets[pos] == NULL){
    Pair * Valor = createPair(key, value);
    map -> buckets[pos] = Valor;
    map -> size++;
  }
  else{
    map -> buckets[pos] -> key = key;
    map -> buckets[pos] -> value = value;
  }
  map -> current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
  Pair **old_buckets = map->buckets;
  int capacidad = map->capacity;
  map->capacity *= 2;
  map->size = 0;
  map->buckets = calloc(map->capacity, sizeof(Pair *));
  for (int i = 0; i < capacidad; i++) {
    if (old_buckets[i] != NULL) {
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
    }
  }
}

HashMap * createMap(long capacity) {
  
  HashMap * mapa = (HashMap *) malloc (sizeof(HashMap));
  mapa->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  
  mapa->capacity = capacity;
  mapa->size = 0;
  mapa->current = -1;

  return mapa;
}

void eraseMap(HashMap * map, char * key){
  int pos = hash(key, map -> capacity);
  int inicio = pos;

  while(map -> buckets[pos] != NULL){
    if(strcmp(map -> buckets[pos] -> key, key) == 0){
      map -> buckets[pos] = NULL;
      map -> size--;
      return;
    }
     pos = ((pos + 1) % map -> capacity);
    if(inicio == pos){
      break;
    }
  }
}


void* searchMap(HashMap* map, void* key) {
  int pos = hash(key, map->capacity);
  int inicio = pos;

  while (map->buckets[pos] != NULL) {
    if (strcmp(map->buckets[pos]->key, key) == 0) {
      map->current = pos;
      return map->buckets[pos]->value;
    }
    pos = (pos + 1) % map->capacity;
    if (inicio == pos)
      return NULL;
  }

  return NULL;
}


Pair * firstMap(HashMap * map) {
  for (int i = 0 ; i < map->capacity ; i++){
    map->current = i;
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if (map->current == map->capacity) return NULL;
  
  for (int i = map->current + 1 ; i < map->capacity ; i++){
    map->current = i;
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      return map->buckets[i];
    }
  }
  return NULL;
}
