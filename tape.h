#pragma once


#include <sys/types.h>


typedef struct unit_t* Unit;


struct unit_t {
    Unit prev;
    Unit next;
    int  data;
};


typedef struct cell_t* Cell;


struct cell_t {    
    Cell prev;
    Cell next;

    struct unit_t unit;
};


typedef struct tape_t* Tape;


typedef struct tape_t {
    Cell   free;
    size_t free_sz;

    Cell   used;
    size_t used_sz;

    Cell*  list;
    size_t list_sz;
    size_t bulk_sz;
} memory_t;


extern void wind(Tape, size_t);


extern Unit pull(Tape);


extern int  bail(Unit, Tape);


extern void wipe(Tape);