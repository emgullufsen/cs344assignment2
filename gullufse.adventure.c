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
const char *ROOM_NAMES[10] = {"Tsar Peter's Docha","King Henry's Oubliette","Stalin's SMERSH Interrogation Room","Bonaparte's Foyer","Tutankhamun's Crypt","T. Roosevelt's Rifle Range","Truman's Bomb Bay","Churchill's Brandy Basement","Agamemnon's Beach Tent", "Nero's Wine Cellar"};

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
	
	printf("current working directory is : %s\n", old_cwd);
	
	printf("the dir name would be: %s\n", directory_name);
	
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
	printf("new current working directory is : %s\n", new_cwd);
	printf("that again is, : %s\n", new_cwd_arr);
	
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

	i = 0;
	for (i; i < 7; i++){
		printf("indexer %i is %i - and it's ROOM_NAME member is %s\n", i, room_indexer->arr[i], ROOM_NAMES[room_indexer->arr[i]]);
	}

	i = 0;
	for (i; i < 7; i++){
		printf("roomfile_arr index %i has ROOM_NAMES index %i which is: %s and room_type %s\n",i, roomfile_arr[i]->name, ROOM_NAMES[roomfile_arr[i]->name], roomfile_arr[i]->room_type);
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
// 	struct roomfile
// {
// 	int name; // index in ROOM_NAMES
// 	int connections[6]; // indices in an index of roomfiles 
// 	const char *room_type;
// 	int num_connections;
// 	int index;
// };
	int lay = 0;
	for (lay; lay < 7; lay++){
		printf("roomfile_arr[%i]: name - %s, connections - \n", lay, ROOM_NAMES[roomfile_arr[lay]->name]);
		int loop = roomfile_arr[lay]->num_connections;
		int boop = 0;
		for (boop; boop < loop; boop++){
			printf("connection: %s\n", ROOM_NAMES[roomfile_arr[roomfile_arr[lay]->connections[boop]]->name]);
		}
	}

	/* phew, ok so our roomfile_arr array contains all the data
	we need in order to write our files, so let's write.
	*/
	int ray = 0;
	for (ray; ray < 7; ray++){
		
	}
	exit(0);
}
