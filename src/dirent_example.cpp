#include <stdio.h>
#include <dirent.h>
#include <iostream>

using namespace std;

int listdir( const char *path ) {
	struct dirent *entry;
	DIR *dp;
	cout << "input path: " << path << endl;
	dp = opendir(path);
	if (dp == NULL){
		cout << "error" << endl;
		perror("opendir");
		return -1;
	}

	while(( entry = readdir(dp) )){
		cout << entry->d_name << endl;		
	}

	closedir(dp);
	return 0;
}

int main(int argc, char **argv) {
	

	listdir(argv[1]);
	cout << argc << endl;	

	return 0;
}