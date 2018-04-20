#include <stdlib.h>
#include <sys/types.h>

#include "tape.h"


#define SUCCESS 0
#define FAILURE 1


void wind(Tape tape, size_t bulk_sz) 
{
    tape->free = 0;
    tape->used = 0;
    tape->list = 0;
    tape->free_sz = 0;
    tape->used_sz = 0;
    tape->list_sz = 0;
    tape->bulk_sz = bulk_sz;
}


static inline int grow(Tape tape) 
{
    if (tape->list_sz == 0) {
        if (!(tape->list = malloc(sizeof(Cell))))
            return FAILURE;
    } else {
        if (!(tape->list = realloc(tape->list, sizeof(Cell) * (tape->list_sz + 1))))
            return FAILURE;
    }

    if (!(tape->list[tape->list_sz] = malloc(tape->bulk_sz * sizeof(struct cell_t))))
        return FAILURE;

    Cell cell;

    for (int i = 0; i < tape->bulk_sz; i++) 
    {
        cell = tape->list[tape->list_sz] + i;

        cell->next = tape->free;
        cell->prev = 0;

        cell->unit.prev = 0;
        cell->unit.next = 0;
        cell->unit.data = 0;

        if (tape->free)
            tape->free->prev = cell;

        tape->free = cell;
    }

    tape->free_sz += tape->bulk_sz;
    tape->list_sz ++;

    return SUCCESS;
}


Unit pull(Tape tape) 
{
    if (tape->free_sz < 2 && grow(tape) == FAILURE)
        return 0;

    Cell  cell = tape->free;
    tape->free = cell->next;

    tape->free->prev = 0;

    cell->next = tape->used;
    cell->prev = 0;
    tape->used = cell;
    
    tape->free_sz--;
    tape->used_sz++;

    return &cell->unit;
}


int bail(Unit unit, Tape tape) 
{
    void *tmp;
    Cell cell;

    if (!(tmp = (void*)unit) || !(cell = (Cell)(tmp - (2 * sizeof(Cell)))))
        return FAILURE;

    if (cell->prev)
        cell->prev->next = cell->next;

    if (tape->free)
        tape->free->prev = cell;

    cell->next = tape->free;
    cell->prev = 0;

    tape->free = cell;
    tape->free_sz++;
    tape->used_sz--;

    return SUCCESS;
}


void wipe(Tape tape) 
{
    for (int i = 0; i < tape->list_sz; i++)
        if (tape->list[i])
            free(tape->list[i]);

    if (tape->list)
        free(tape->list);
    
    wind(tape, tape->bulk_sz);
}