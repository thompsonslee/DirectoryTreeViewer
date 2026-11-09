#include <stdio.h>
#include <dirent.h>
#include <string.h>

int handleArgs(int argc, char *argv[]);
int printDir(char *dirURL, int depth);
int hasNext(DIR *dir);

char *workingDir = "./";
char dirTree[500] = "";

int main(int argc, char* argv[]){
    
    if(handleArgs (argc, argv)!= 0){
        return -1;
    }

    if(printDir(workingDir, 0) != 0){
        return -1;
    }


}

int printDir(char *dirURL, int depth){

    DIR *dir = opendir(dirURL);

    if(dir == NULL){
        printf("error reading dir: %s\n", dirURL);
        return 1;
    }

    struct dirent *direntry = readdir(dir);

    while(direntry != NULL){
        if(strcmp(direntry->d_name, ".") == 0 || strcmp(direntry->d_name, "..") == 0){
            direntry = readdir(dir);
            continue;
        }

        if(depth == 0){
            printf("%s%s\n", dirTree, direntry->d_name);
        } else {
            if(hasNext(dir)){
                printf("%s├──%s\n", dirTree, direntry->d_name);
            } else {
                printf("%s└──%s\n", dirTree, direntry->d_name);

            }
        }

        if(direntry->d_type == DT_DIR){

            int currLength = strlen(dirTree);
            if(hasNext(dir) && depth != 0){
                strcat(dirTree, "│  ");
            } else {
                strcat(dirTree, "   ");
            }

            char subDirURL[500];
            snprintf(subDirURL, sizeof subDirURL, "%s/%s", dirURL, direntry->d_name);
            printDir(subDirURL, depth + 1);
            dirTree[currLength] = '\0';
        }
        direntry = readdir(dir);
    }

    if((closedir(dir)) != 0){
        printf("error closing dir");
        return -1;
    }

    return 0;
}

int handleArgs(int argc, char *argv[]){
    if(argc == 1){
        return 0;
    }

    if(argc > 2){
        printf("Error: more than one argument provided");
        return -1;
    }

    workingDir = argv[1];
    return 0;

}


int hasNext(DIR *dir){
    long bookmark = telldir(dir);   
    struct dirent *direntry = readdir(dir);
    seekdir(dir, bookmark);

    if(direntry == NULL){
        return 0;
    }
    return 1;

}
