#ifndef HASHMAP_H
#define HASHMAP_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* структура ячейки хэш таблицы */
typedef struct Cell{
    int value;
    char* key;
} Cell;

/*
структура типа, который хранит в себе все ячейки таблицы и ее размер 

через него удобнее управлять размером таблицы
*/
typedef struct HashMap{
    Cell* cell_list;
    size_t current_hashmap_size;
} HashMap;


/* 
хэш функция 

та кже может использоваться для получения индекса по ключу
*/
static inline size_t
Hash(size_t hashmap_size, char* key){
    return (int)*key % hashmap_size;
}

/* realloc для таблицы */
static inline void
Realloc_HashMap(HashMap* map_pointer){
    map_pointer->cell_list = (Cell*)realloc(map_pointer->cell_list, map_pointer->current_hashmap_size * 2 * sizeof(Cell));
    if(!map_pointer->cell_list){
        fprintf(stderr, "Memory re-allocate error\n");
        return;
    }

    map_pointer->current_hashmap_size *= 2;
}

/*
находит первую свободную ячейку при промахе

если такой нет - вызывает Realloc_HashMap
*/
static inline size_t
Find_first_free(HashMap* map){
    size_t i = 0;
    for(; i < map->current_hashmap_size; ++i)
        if(map->cell_list[i].value == INT_MIN)
            return i;
    
    Realloc_HashMap(map);
    return i++;
}


/* выводит значение ячейки по ключу */
static inline void
Print_by_key(HashMap* map, char* key){
    size_t index = Hash(map->current_hashmap_size, key);
    fprintf(stdout, "Key - %s\nValue - %d\n\n", key, map->cell_list[index].value);
}

/* выводит всю таблицу */
static inline void
Print_full_map(HashMap* map){
    for(size_t i = 0; i < map->current_hashmap_size; ++i)
        fprintf(stdout, "Key - %s , Value - %d\n", map->cell_list[i].key, map->cell_list[i].value);
}


/* инициализирует таблицу */
HashMap*
Init_HashMap(){
    HashMap* new_map = (HashMap*)malloc(sizeof(HashMap));
    new_map->cell_list = (Cell*)malloc(32 * sizeof(Cell));
    if(!new_map || !new_map->cell_list){
        fprintf(stderr, "Memory allocate error\n");
        return NULL;
    }

    new_map->current_hashmap_size = 32;

    for(size_t i = 0; i < new_map->current_hashmap_size; ++i){
        new_map->cell_list[i].key = NULL;
        new_map->cell_list[i].value = INT_MIN;
    }

    return new_map;
}

/* вставляет значение по ключу */
void
Insert(HashMap* map_poiner, char* key, int value){
    size_t index = Hash(map_poiner->current_hashmap_size, key);
    if(map_poiner->cell_list[index].value != INT_MIN)
        index = Find_first_free(map_poiner);
    
    map_poiner->cell_list[index].key = strdup(key);
    map_poiner->cell_list[index].value = value;
}

/* удаляет объект по ключу */
void
Pop(HashMap* map_pointer, char* key){
    size_t index = Hash(map_pointer->current_hashmap_size, key);
    if(map_pointer->cell_list[index].key)

    free(map_pointer->cell_list[index].key);
    map_pointer->cell_list[index].key = NULL;
    map_pointer->cell_list[index].value = INT_MIN;
}


#endif