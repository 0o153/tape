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
	stor->free_sz = 0;
	stor->used_sz = 0;
	stor->mesh_sz = 0;
	stor->bulk_sz = bulk_sz;
}


static inline int grow(Stor stor) 
{
	if (stor->mesh_sz == 0) {
		if (!(stor->mesh = malloc(sizeof(Cell))))
			return FAILURE;
	} else {
		if (!(stor->mesh = realloc(stor->mesh, sizeof(Cell) * (stor->mesh_sz + 1))))
			return FAILURE;
    }
        
	if (!(stor->mesh[stor->mesh_sz] = malloc(stor->bulk_sz * sizeof(struct cellₜ))))
        return FAILURE;
        
    Cell cell;
    
	for (int i = 0; i < stor->bulk_sz; i++) 
	{
		cell = stor->mesh[stor->mesh_sz] + i;
		cell->next = stor->free;
        cell->prev = 0;
        
		cell->unit.prev = 0;
        cell->unit.next = 0;
        
		if (stor->free)
            stor->free->prev = cell;
            
		stor->free = cell;
    }
    
	stor->free_sz += stor->bulk_sz;
    stor->mesh_sz ++;
    
	return SUCCESS;
}


Unit pull(Stor stor) 
{
	if (stor->free_sz < 2 && grow(stor) == FAILURE)
        return 0;
        
	Cell cell;
    cell = stor->free;
    
	stor->free = cell->next;
    stor->free->prev = 0;
    
	cell->next = stor->used;
    cell->prev = 0;
	stor->used = cell;
    
    stor->free_sz--;
    stor->used_sz++;
    
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
    
	stor->free_sz++;
    stor->used_sz--;
    
	return SUCCESS;
}


void wipe(Stor stor) 
{
	for (int i = 0; i < stor->mesh_sz; i++)
		if (stor->mesh[i])
            free(stor->mesh[i]);
            
	if (stor->mesh)
        free(stor->mesh);
        
	wind(stor, stor->bulk_sz);
}