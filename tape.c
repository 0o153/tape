#include <stdlib.h>
#include <sys/types.h>

#include "tape.h"


#define SUCCESS 0
#define FAILURE 1


void wind(Stor stor, Size bulk_sz) 
{
	stor->free = 0;
	stor->used = 0;
	stor->mesh = 0;
	stor->freeₛ = 0;
	stor->usedₛ = 0;
	stor->meshₛ = 0;
	stor->bulkₛ = bulk_sz;
}


static inline int grow(Stor stor) 
{
	if (stor->meshₛ == 0) {
		if (!(stor->mesh = malloc(sizeof(Cell))))
			return FAILURE;
	} else {
		if (!(stor->mesh = realloc(stor->mesh, sizeof(Cell) * (stor->meshₛ + 1))))
			return FAILURE;
    }
        
	if (!(stor->mesh[stor->meshₛ] = malloc(stor->bulkₛ * sizeof(struct cellₜ))))
        return FAILURE;
        
    Cell cell;
    
	for (int i = 0; i < stor->bulkₛ; i++) 
	{
		cell = stor->mesh[stor->meshₛ] + i;
		cell->next = stor->free;
        cell->prev = 0;
        
		cell->unit.prev = 0;
        cell->unit.next = 0;
        
		if (stor->free)
            stor->free->prev = cell;
            
		stor->free = cell;
    }
    
	stor->freeₛ += stor->bulkₛ;
    stor->meshₛ ++;
    
	return SUCCESS;
}


Unit pull(Stor stor) 
{
	if (stor->freeₛ < 2 && grow(stor) == FAILURE)
        return 0;
        
	Cell cell;
    cell = stor->free;
    
	stor->free = cell->next;
    stor->free->prev = 0;
    
	cell->next = stor->used;
    cell->prev = 0;
	stor->used = cell;
    
    stor->freeₛ--;
    stor->usedₛ++;
    
	Unit unit;
    unit = &cell->unit;
	unit->data.k = 0;
	unit->data.u = 0;
    unit->data.f = 0;
    
	return unit;
}


int bail(Unit unit, Stor stor) 
{
	void *tmp;
    Cell cell;
    
	if (!(tmp = (void*)unit) || !(cell = (Cell)(tmp - (2 * sizeof(Cell)))))
        return FAILURE;
        
	if (cell->prev)
        cell->prev->next = cell->next;
        
	if (stor->free)
        stor->free->prev = cell;
        
	cell->next = stor->free;
    cell->prev = 0;
    stor->free = cell;
    
	stor->freeₛ++;
    stor->usedₛ--;
    
	return SUCCESS;
}


void wipe(Stor stor) 
{
	for (int i = 0; i < stor->meshₛ; i++)
		if (stor->mesh[i])
            free(stor->mesh[i]);
            
	if (stor->mesh)
        free(stor->mesh);
        
	wind(stor, stor->bulkₛ);
}