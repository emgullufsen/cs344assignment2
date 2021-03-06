//eric michael gullufsen
// Oregon State University - SPRING 2015 - CS 344 OPERATING SYSTEMS
// program 2 - adventure
//

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>

const char *username = "gullufse.rooms.";

// room types
const char *START = "START_ROOM";

const char *MID = "MID_ROOM";

const char *END = "END_ROOM";

// room names
const char *ROOM_NAMES[10] = {"Docha","Oubliette","Library","Foyer","Crypt","Range","Bay","Basement","Tent","Cellar"};

const char *CON = "CONNECTION";

const char *RN = "ROOM NAME";

const char *RT = "ROOM TYPE";

struct roomfile
{
	int name; // index in ROOM_NAMES
	int connections[6]; // indices in an index of roomfiles 
	const char *room_type;
	int num_connections;
	int index;
};

struct room_indices
{
	int length;
	int arr[7];
};

int make_connection(struct roomfile *room1, struct roomfile *room2){
	if (!is_connected(room1,room2) && !(room1->num_connections == 6) && !(room2->num_connections == 6)){

		room1->connections[room1->num_connections] = room2->index;
		room1->num_connections++;
		room2->connections[room2->num_connections] = room1->index;
		room2->num_connections++;
		return 1;
	}

	return 0;
}

int is_connected(struct roomfile *room1, struct roomfile *room2){
	int k = room1->num_connections;
	int y = 0;
	for (y; y<k; y++){
		if (room1->connections[y] == room2->index){
			return 1;
		}
	}

	return 0;
}


int is_used(struct room_indices *roomy, int index){
	int i;
	for (i = 0; i < roomy->length; i++){
		if (index == roomy->arr[i]){
			return 1;
		}
	}
	return 0;
}

struct room{
	char name[40];
	char connections_s[6][40];
	char room_ty[40];
	int number;
};

int readinto(struct room *theroom, FILE * thefile){
	char st[40];
	while(fgets(st,40,thefile) != NULL){
		if (strstr(st,RN)){
			strcpy(theroom->name, st+11);
			theroom->name[strlen(theroom->name) - 1] = '\0';
		}
		if (strstr(st,RT)){
			strcpy(theroom->room_ty, st+11);
			theroom->room_ty[strlen(theroom->room_ty) -1] = '\0';
		}
		if (strstr(st,CON)){
			strcpy(theroom->connections_s[theroom->number], st+14);
			theroom->connections_s[theroom->number][(int)strlen(theroom->connections_s[theroom->number]) - 1] = '\0';
			theroom->number++;

		}
	}
}

int is_possible_connection(char *inp, struct room *theroom){

	int g = 0;
	for (g; g < theroom->number; g++){
		if (strcmp(inp, theroom->connections_s[g]) == 0)
			return 1;
	}
	return 0;

}

struct path{
	int pathlength;
	char patharray[100][40];
};

//display the formattet interface
int displayinfo(struct room *theroom){

	printf("CURRENT LOCATION: %s\n", theroom->name);
	printf("POSSIBLE CONNECTIONS: ");
	int p = 0;
	for (p; p < theroom->number; p++){
		if (p != (theroom->number - 1))
			printf("%s, ", theroom->connections_s[p]);
		else
			printf("%s.\n", theroom->connections_s[p]);
	}
	printf("WHERE TO? >");
}

int savepath(struct room *theroom, struct path *thepath){
	strcpy(thepath->patharray[thepath->pathlength],theroom->name);
	thepath->pathlength++;
}

int displayresults(struct path *thepath){
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\nYOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", thepath->pathlength);
	int u = 0;
	for (u; u < thepath->pathlength; u++){
		printf("%s\n", thepath->patharray[u]);
	} 
}

int main(int argc, char *argv[]){
	// create directory where temp files live
	srand(time(NULL));

	struct roomfile *roomfile_arr[7];
	int j;
	for (j=0;j<7;j++){
		roomfile_arr[j] = (struct roomfile *)malloc(sizeof(struct roomfile));
		roomfile_arr[j]->num_connections = 0;
		roomfile_arr[j]->index = j;
	}

	char directory_name[100];
	char *old_cwd;
	char old_cwd_arr[100];
	char new_cwd_arr[100];
	char *new_cwd;
	pid_t process_id = getpid();
	int process_id_int = (int)process_id;
	struct room_indices *room_indexer = (struct room_indices *)malloc(sizeof(struct room_indices));
	room_indexer->length = 0;
	// in this call we "send the formatted string to" directory_name - StackOverflow
	sprintf(directory_name, "%s%i", username, process_id_int);
	
	old_cwd = getcwd(old_cwd_arr, 100);
	
	// create directory with rwx for user, group, rx for others.
	int checky0 = mkdir(directory_name, 0777);
	if (checky0){
		printf("problem making directory\n");
	}
	int checky1 = chdir(directory_name);
	
	if(checky1 != 0){
		printf("problem changing directory\n");
	}
	
	new_cwd = getcwd(new_cwd_arr, 100);
	
	int i = 0;
	int nummy;
	// get the seven indices to be used from ROOM_NAMES
	// set names for the seven roomfiles (via an int representing an index in ROOM_NAMES)
	for (i; i < 7; i++){
		while (1){
			nummy = rand() % 10;
			if (!is_used(room_indexer, nummy)){
				room_indexer->arr[i] = nummy;
				room_indexer->length++;
				roomfile_arr[i]->name = nummy;
				if (i == 0){
					roomfile_arr[i]->room_type = START;
				}
				else if (i == 6){
					roomfile_arr[i]->room_type = END;
				}
				else {
					roomfile_arr[i]->room_type = MID;					
				}
				break;
			}
		}
	}

	/*

Here we will establish the connections forming a path from start to finish,
so that this is guaraunteed. We'll get a random number from 0-5, and take that many consecutive
entries from our roomfile_arr array of roomfile structs. These entries consist of  
randomly assigned names from ROOM_NAMES. So effectively, our path is random.
	 */

	int inbetween = rand() % 7;

	int eye = 0;
	if (inbetween != 0){
		for (eye; eye < inbetween; eye++){
			make_connection(roomfile_arr[eye], roomfile_arr[eye + 1]);
		}
		if (inbetween != 6){
			if (eye == (inbetween - 1)){
				make_connection(roomfile_arr[eye], roomfile_arr[6]);
			}
		}
	}
	else{
		make_connection(roomfile_arr[0], roomfile_arr[6]);
	}

	/*
	Here we add connections to each of the entries in roomfile_arr until each entry
	has between 3 and 6 total connections, this number being assigned randomly
	*/

	int jay = 0;
	for (jay; jay < 7; jay++){
		int numconnex = ((rand() % 4) + 3);
		while (roomfile_arr[jay]->num_connections < numconnex){
			int to_con = (rand() % 7);
			if (to_con != jay){
				int consuc = make_connection(roomfile_arr[jay], roomfile_arr[to_con]);
			}

		}
	}

	/* phew, ok so our roomfile_arr array contains all the data
	we need in order to write our files, so let's write.
	*/
	int ray = 0;
	for (ray; ray < 7; ray++){
		char file_name[30];
		sprintf(file_name,"%s",ROOM_NAMES[roomfile_arr[ray]->name]);
		FILE *file = fopen(file_name,"w");
		fprintf(file, "ROOM NAME: %s\n", ROOM_NAMES[roomfile_arr[ray]->name]);
		int rye = 0;
		for (rye; rye < roomfile_arr[ray]->num_connections; rye++){
			fprintf(file, "%s %i: %s\n", CON, (rye+1), ROOM_NAMES[roomfile_arr[roomfile_arr[ray]->connections[rye]]->name]);
		}
		fprintf(file, "ROOM TYPE: %s\n", roomfile_arr[ray]->room_type);

		fclose(file);
	}

	struct room *therooms[7];

	int ye = 0;
	for (ye; ye < 7; ye++){
		therooms[ye] = (struct room *)malloc(sizeof(struct room));
	}
	int jai = 0;
	for (jai; jai < 7; jai++){
		FILE * filly = fopen(ROOM_NAMES[room_indexer->arr[jai]], "r");
		readinto(therooms[jai], filly);
		fclose(filly);
	}

	//play the game!!!
	// finally
	int moves = 0;
	displayinfo(therooms[0]);
	struct room *activeroom = therooms[0];
	struct path *pathy = (struct path *)malloc(sizeof(struct path));
	pathy->pathlength = 0;
	savepath(activeroom, pathy);
	char mo[40];
	while (pathy->pathlength < 100){
		fgets(mo, 40, stdin);
		if (mo[strlen(mo) - 1] == '\n')
			mo[strlen(mo) - 1] = '\0';

		if (is_possible_connection(mo, activeroom)){
			int one = 0;
			for (one; one < 7; one++){
				if (strcmp(mo, therooms[one]->name) == 0){
					activeroom = therooms[one];
					savepath(activeroom, pathy);
					break;
				}
			}
			if (strcmp(activeroom->room_ty, END) == 0){
				displayresults(pathy);
				break;
			}
			displayinfo(activeroom);
		}
		else{
			printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
			displayinfo(activeroom);
		}
	}

	exit(0);
}
