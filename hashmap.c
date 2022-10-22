#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
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


void insertMap(HashMap * map, char * key, void * value) 
{
  Pair *newPair = createPair(key, value);
  size_t i = hash(key, map->capacity);
  //size_t es el mismo tipo de variable que un long long

  while(1)
  {
    if(map->buckets[i] == NULL) 
    {
      map->buckets[i] = newPair;
      map->size++;
      map->current = i;
      break;
    }
    i++;

    if(i == map->capacity)
    {
      i = 0;
    }
  }
  
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  Pair **old_buckets = map->buckets;

  map->capacity *= 2;

  map->buckets = (Pair **) malloc (sizeof(Pair *) * map->capacity);

  map->size = 0;

  for(size_t i = 0; i < (map->capacity/2); i++)
  {
    if(old_buckets[i] != NULL)
    {
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
    }
  }
}


HashMap * createMap(long capacity) 
{
  HashMap *map = (HashMap *) malloc (sizeof(HashMap));

  map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  /*Da lo mismo si se almacena sizeof(Pair) o sizof(Pair *), ya que 
  almacena igual cantidad de bytes*/
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;//Se debería poner en cualquier posicion
  
  return map;
}

void eraseMap(HashMap * map,  char * key) 
{    
  Pair *dato = searchMap(map, key);
  
  if(dato != NULL)
  {
    dato->key = NULL;
    dato->value = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) 
{   
  size_t i = hash(key, map->capacity);
  size_t cont=0;
  
  while(1)
  {
    if(map->buckets == NULL)
    {
      break;
    }
    if(map->buckets[i] == NULL)
    {
      break;
    }
    if(is_equal(map->buckets[i]->key, key))
    {
      map->current = i;
      return(map->buckets[i]);
    }
    else
    {
      i++;
      cont++;
      if(i == map->capacity)
      {
        i=0;
      }
      if(cont == map->capacity)
      {
        break;
      }
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  size_t i = 0;
  
  if(map->size == 0)
  {
    return NULL;
  }
  if(map->capacity == 0)
  {
    return NULL;
  }
  if(map->buckets == NULL)
  {
    return NULL;
  }
  if(map->size > 0)
  {
    for(i=0; i < map->capacity; i++)
    {
      if(map->buckets[i] != NULL)
      {
        if(map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];  
        }
        
      }
    }
  }
  
  return NULL;
}

Pair * nextMap(HashMap * map) {
  size_t i;
  map->current++;
  if(map->size > 0)
  {
    for(i = map->current; i < map->capacity; i++)
    {
      if(map->buckets[i] != NULL)
      {
        if(map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];  
        }
      }
    }
  }
  return NULL;
}