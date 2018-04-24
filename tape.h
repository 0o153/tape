#pragma once


#include <sys/types.h>


typedef struct machₜ* Mach;


typedef struct tapeₜ* Stor;


typedef struct cellₜ* Cell;


typedef size_t Size;


struct machₜ {
	Stor stor;

	Cell make;

	Cell main;
	Cell fork;

	Size ceil;
};


typedef struct tapeₜ {
	Cell  free;
	Size  free_sz;

	Cell  used;
	Size  used_sz;
	
	Cell *mesh;
	Size  mesh_sz;
	Size  bulk_sz;
} Tape;


typedef int Knot;


typedef struct unitₜ* Unit;


typedef void (*Func)(Mach);


typedef union dataₜ {
	Knot k;
	Unit u;
	Func f;
} Data;


struct unitₜ {
	Unit prev;
	Unit next;
	Data data;
};


struct cellₜ {	
	Cell prev;
	Cell next;
	struct unitₜ unit;
};


extern void wind(Stor, Size);


extern Unit pull(Stor);


extern int  bail(Unit, Stor);


extern void wipe(Stor);


// TODO:

extern void fork(Mach);


extern void push(Mach);


extern void join(Mach);


extern Unit take(Mach);


extern void drop(Mach);


extern void swap(Mach);


extern void test(Mach);
