#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>


#include "utils.h"
#include "dir_file.h"

directory_t* create_dir(char* name) {
    directory_t *dir;

    dir = (directory_t*)malloc(sizeof(directory_t));

    memcpy(dir->name, name, MAX_NAME_SIZE);

    dir->num_files = 0;
    dir->num_dirs = 0;

    return dir;

}

directory_t* find_dir(directory_t* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);
    assert(strlen(name) > 0);

    for (int i = 0; i < dir->num_dirs; i++) {
        if (strcmp(dir->dir_list[i]->name, name) == 0)
            return dir->dir_list[i];
    }

    return NULL;
}

// This find_create_dir() find directory_t matched with name variable, in parent_dir.
// Otherwise, if this directory not exists, creates directory.
directory_t* find_create_dir(directory_t* parent_dir, char* name, bool* is_create) {
    directory_t *dir;

    assert(parent_dir != NULL);
    // printf("\nthis is parent dir: %s", parent_dir->name);
    // printf("\n-----------------------------");
    // printf("\nthis is name: %s", name);
    // printf("\n-----------------------------");

    dir = find_dir(parent_dir, name);
  //  printf("\nin func?\n");
    if (dir == NULL) {
       // printf("\nin func?\n");
        dir = create_dir(name);
     //   printf("\nin func?\n");
        *is_create = true; //segmentaion fault
      //  printf("\nin func?\n");
    }
    //printf("\nin func?\n");
    return dir;
}

file_t* create_file(char* name) {
    file_t *file;

    file = (file_t*)malloc(sizeof(file_t));

    memcpy(file->name, name, MAX_NAME_SIZE);

    return file;
}

file_t* find_file(directory_t* dir, char* name) {
    assert(dir != NULL);
    assert(name != NULL);
    assert(strlen(name) > 0);

    for (int i = 0; i < dir->num_files; i++) {
        if (strcmp(dir->file_list[i]->name, name) == 0)
            return dir->file_list[i];
    }

    return NULL;
}

// This find_create_file() find file matched with name variable, in dir.
// Otherwise, if this file not exists, create the file.
file_t* find_create_file(directory_t* dir, char* name, bool* is_create) {
    file_t *file;

    assert(dir != NULL);

    file = find_file(dir, name);

    if (file == NULL) {
        file = create_file(name);
        *is_create = true;
    }

    return file;
}

// This make_dir_and_file() make hierarchy of file and directory which is indicated by token_list.
// Everything starts in root_dir. You can implement this function using find_create_dir() and find_create_file().


void make_dir_and_file(directory_t* root_dir, char** token_list, int num_tokens) {

   directory_t* current_dir;
   current_dir = root_dir;
   directory_t* tmp_dir;
   directory_t* origin_dir;
   origin_dir = root_dir; //오로지 free만을 위해 만든 함수.
   int i=0, index = 0;
   bool* is_create = (bool*)malloc(sizeof(bool));

//     int k =0;
//    while(k != num_tokens){
//     printf("\n%d\n", num_tokens);
//     printf("\n%d %s\n",k, token_list[k]);
//     k++;
//    }

   for(i; i < num_tokens-1; i++){

    *is_create = false;

    // printf("\nwhat is creat_dir name? -> %s", current_dir->name);
    // printf("\n---------------------------------\n");
    // printf("\nwhat is token_list? -> %s", token_list[i]);
    // printf("\n---------------------------------\n");
    find_create_dir(current_dir, token_list[i], is_create);

    if(*is_create == true){
    //    printf("\nis_create in\n");
        tmp_dir = create_dir(token_list[i]);

        //printf("\n%s", tmp_dir->name);

        current_dir->dir_list[current_dir->num_dirs] = tmp_dir;

        //printf("\ndir name: %s\n", current_dir->name);
        //printf("\nnew dir name: %s\n", current_dir->dir_list[current_dir->num_dirs]->name);


       //printf("\nnew dirs num: %d\n", current_dir->dir_list[current_dir->num_dirs]->num_dirs);

        current_dir->num_dirs++;
        //printf("\ndirs num: %d\n", current_dir->num_dirs);
        //printf("\n----------------------------------------------------\n");
        current_dir = tmp_dir;
    }

    else{
         //printf("\n this is else\n");
         //printf("\ndir name: %s\n", current_dir->name);
         //printf("\ndirs num: %d\n", current_dir->num_dirs);
         //printf("\n----------------------------------------------------\n");
         current_dir = find_dir(current_dir, token_list[i]);
        }

   }

    *is_create = false;
    find_create_file(current_dir, token_list[i], is_create);

    if(*is_create == 1){

        //  printf("\n%s\n", "file in");

        file_t* tmp_file = (file_t*)malloc(sizeof(file_t));

        tmp_file = create_file(token_list[i]);
        current_dir->file_list[current_dir->num_files] = tmp_file;
        //printf("\nfile name: %s\n", current_dir->file_list[current_dir->num_files]->name);
        current_dir->num_files++;
        //printf("\nfile num: %d\n", current_dir->num_files);
        //printf("\n----------------------------------------------------\n");
    }
   // current_dir = origin_dir;
}



void free_dir_and_file(directory_t* dir) {

    for (int i = 0; i < dir->num_files; i++)
        free(dir->file_list[i]);

    for (int i = 0; i < dir->num_dirs; i++)
        free_dir_and_file(dir->dir_list[i]);
    //printf("\n free func in? \n");
    free(dir);
    //printf("\n free func in? \n");
}

// This find_target_dir() find the directoy which is indicated as full path by token_list
// and return directory_t* entry of the found directory or NULL when such directory not exists.
directory_t* find_target_dir(directory_t* root_dir, char** token_list, int num_tokens) {
    directory_t *current_dir, *child_dir;
    char *token;
    char path[MAX_BUFFER_SIZE];

    memset(path, 0, MAX_BUFFER_SIZE);

    current_dir = root_dir;

    for (int i = 0; i < num_tokens; i++) {
        strcat(path, "/");
        token = token_list[i];
        child_dir = find_dir(current_dir, token);

        if (child_dir == NULL) {
            fprintf(stderr, "Not found '%s' directory in %s\n", token, path);
            return NULL;
        }

        current_dir = child_dir;
        strcat(path, token);
        child_dir = NULL;
    }

    return current_dir;
}

void print_files_on_dir(directory_t* dir) {
    //printf("\n print func in? \n");
    for (int i = 0; i < dir->num_files; i++)
        printf("%s\n", dir->file_list[i]->name);
}