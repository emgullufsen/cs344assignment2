// eric
//

#include<stdio.h>

int main()
{
	const char *ROOM_NAMES[10] = {"START_ROOM","END_ROOM","MID_ROOM","Tsar Peters Docha","King Henrys Tea Room","Napoleons Foyer","Tutankhamuns Crypt","T. Roosevelts Rifle Range","Churchills Brandy Basement","Stalins SMERSH Interrogation Cellar"};

	int i;
	for (i=0; i < 10; i++)
	{
		printf("i: %i, room-name: %s...\n", i, ROOM_NAMES[i]);
	}
	return 0;
}
