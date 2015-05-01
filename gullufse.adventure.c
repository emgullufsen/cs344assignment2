//eric michael gullufsen
// Oregon State University - SPRING 2015 - CS 344 OPERATING SYSTEMS
// program 2 - adventure
//

#include<stdio.h>

// room types

const char *START = "START_ROOM";

const char *MID = "MID_ROOM";

const char *END = "END_ROOM";

// room names

const char *ROOM_NAMES[10] = {"Tsar Peters Docha","King Henrys Oubliette","Stalins SMERSH Interrogation Room","Bonapartes Foyer","Tutankhamuns Crypt","T. Roosevelts Rifle Range","Trumans Bomb Bay","Churchills Brandy Basement","Agamemnons Beach Tent", "Neros Wine Cellar"};

//create directory gullufse.rooms.<PID>

//create seven room files

//create 7 room structs and write to files

struct roomfile
{
	int name; // index in ROOM_NAMES
	int connections[6]; //indices in ROOM
};
