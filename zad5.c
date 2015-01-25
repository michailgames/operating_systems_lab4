#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }

int characters_to_skip = 0;

char* join_path(char *path, char *file) {
	int p = strlen(path) ? 2 : 1;
    char *result = (char*) malloc(strlen(path) + strlen(file) + p);
    strcpy(result, path);
    if(strlen(path)) {
    	strcat(result, "/");
    }
    strcat(result, file);
    return result;
}

void scan_directory(char *dirname, char *path) {
	char *path_to_dir = join_path(path, dirname); 
	DIR *dir = opendir(path_to_dir);
	if(dir == NULL) {
		ERROR(path_to_dir);
	}
	struct dirent *dir_entry;
	struct stat file_info;
	while((dir_entry = readdir(dir)) != NULL) {
		char *path_to_file = join_path(path_to_dir, dir_entry->d_name);
		if(stat(path_to_file, &file_info) == -1) {
			ERROR(path_to_file);
		}
		if(file_info.st_mode & S_IFDIR) { //directory
			if(strcmp(dir_entry->d_name, ".") != 0 &&
			   strcmp(dir_entry->d_name, "..") != 0) {
				scan_directory(dir_entry->d_name, path_to_dir);
			}
		}
		else { // other files
			printf("%s %d %o\n", path_to_file + characters_to_skip,
					(int)file_info.st_size, file_info.st_mode % 01000);
		}
		free(path_to_file);
	}
	free(path_to_dir);
	if(closedir(dir) == -1) {
		ERROR(path_to_dir);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: ./zad5 filename.\n");
		return 0;
	}
	characters_to_skip = strlen(argv[1]) + 1;
	scan_directory(argv[1], "");
	return 0;
}
