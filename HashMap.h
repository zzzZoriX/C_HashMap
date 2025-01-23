#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* структура ячейки хэш таблицы */
typedef struct HashMap{
    int value;
    char* key;
} HashMap;

/*
структура типа, который хранит в себе все ячейки таблицы и ее размер 

через него удобнее управлять размером таблицы
*/
typedef struct HashMap_t{
    HashMap* cell_list;
    size_t current_hashmap_size;
} HashMap_t;


/* 
хэш функция 

та кже может использоваться для получения индекса по ключу
*/
static inline size_t
Hash(size_t hashmap_size, char* key){
    return (int)*key % hashmap_size;
}


/* создает указатель на ячейки таблицы */
HashMap_t
Create_HashMap(){
    HashMap_t new_map;

    new_map.cell_list = (HashMap*)calloc(32, sizeof(HashMap));
    if(!new_map.cell_list){
        fprintf(stderr, "Memory allocate error\n");
        exit(1);
    }

    new_map.current_hashmap_size = 32;

    for(size_t i = 0; i < new_map.current_hashmap_size; ++i){
        new_map.cell_list[i].key = NULL;
        new_map.cell_list[i].value = INT_MIN;
    }

    return new_map;
}

/* освобождает выделенную память на таблицу */
void
Frees_HashMap(HashMap_t* map_poiner){
    if(!map_poiner || !map_poiner->cell_list){
        fprintf(stderr, "Empty HashMap\n");
        exit(1);
    }

    for(size_t i = 0; i < map_poiner->current_hashmap_size; ++i){
        if(map_poiner->cell_list[i].key != NULL){
            printf("%s\n", map_poiner->cell_list[i].key);
            printf("test %d\n", i);
            free(map_poiner->cell_list[i].key);
        }
    }


    free(map_poiner->cell_list);

    map_poiner->cell_list = NULL;
    map_poiner->current_hashmap_size = 0;

    printf("test\n");
}

/* realloc для таблицы */
void
HashMap_realloc(HashMap_t* map_pointer){
    HashMap* temp_cell_list = (HashMap*)realloc(map_pointer->cell_list, map_pointer->current_hashmap_size * 2 * sizeof(HashMap));
    if(!temp_cell_list){
        fprintf(stderr, "Memory re-allocate error\n");
        exit(1);
    }

    map_pointer->cell_list = temp_cell_list;
    map_pointer->current_hashmap_size *= 2;

    free(temp_cell_list);

    printf("test %ld\n", sizeof(map_pointer->cell_list));
}


/* 
ищет первую свободную ячейку
если ее нет - вызывает функцию HashMap_realloc
*/
size_t
Find_free_cell(HashMap_t map_pointer){
    size_t i = 0;

    for(; i < map_pointer.current_hashmap_size; ++i){
        if(map_pointer.cell_list[i].value == INT_MIN)
            return i;
    }

    HashMap_realloc(&map_pointer);
    return ++i;
}


/* выводим всю таблицу */
void
Print_full_HashMap(HashMap_t map_pointer){
    for(size_t i = 0; i < map_pointer.current_hashmap_size; ++i)
        printf("Key: %s\nValue: %d\n", map_pointer.cell_list[i].key, map_pointer.cell_list[i].value);
}

/* выводит значение по ключу и его индекс в таблице */
void
Print_value_by_key(HashMap_t map_pointer, char* key){
    size_t index = Hash(map_pointer.current_hashmap_size, key);
    printf("Key: %s\nValue: %d\n", map_pointer.cell_list[index].key, map_pointer.cell_list[index].value, index);   
}


/* добавляет значение в хэш таблицу */
void
Insert(HashMap_t map_pointer, char* key, int value){
    size_t index = Hash(map_pointer.current_hashmap_size, key);
    if(map_pointer.cell_list[index].value != INT_MIN)
        index = Find_free_cell(map_pointer);

    map_pointer.cell_list[index].key = strdup(key);
    map_pointer.cell_list[index].value = value;
}

/* удаляет элемент */
void
Pop(HashMap_t map_pointer, char* key){
    size_t index = Hash(map_pointer.current_hashmap_size, key);
    
    if(index < 0 && index > map_pointer.current_hashmap_size){
        index = 0;
    }

    map_pointer.cell_list[index].value = INT_MIN;
    map_pointer.cell_list[index].key = NULL;
}

#endif