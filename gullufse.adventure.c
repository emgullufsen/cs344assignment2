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

const char *ROOM_NAMES[10] = {"Tsar Peters Docha","King Henrys Oubliette","Stalins SMERSH Interrogation Room","Bonapartes Foyer","Tutankhamuns Crypt","T. Roosevelts Rifle Range","Trumans Bomb Bay","Churchills Brandy Basement","Agamemnons Beach Tent", "Neros Wine Cellar"};

struct roomfile
{
	int name; // index in ROOM_NAMES
	int connections[6]; // indices in ROOM_NAMES
	const char *room_type;
};

struct room_indices
{
	int length;
	int arr[7];
};

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
	
	// so room_indexer->arr should hold our indices used with ROOM_NAMES 
	// so we need to create 7 struct roomfiles and write them

	exit(0);
}
