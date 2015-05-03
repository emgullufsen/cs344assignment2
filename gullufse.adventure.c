//eric michael gullufsen
// Oregon State University - SPRING 2015 - CS 344 OPERATING SYSTEMS
// program 2 - adventure
//

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

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

int main(int argc, char *argv[]){
	// create directory where temp files live
	char directory_name[100];
	char *old_cwd;
	char old_cwd_arr[100];
	char new_cwd_arr[100];
	char *new_cwd;
	pid_t process_id = getpid();
	int process_id_int = (int)process_id;
	
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
	exit(0);
}
