#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "dir_file.h"
#include "utils.h"

int open_file(char* fname, FILE** input) {
    if (access(fname, F_OK) == -1) {
        ERR_PRINT("The '%s' file does not exists\n", fname);
        return -1;
    }

    *input = fopen(fname, "r");
    if (input == NULL) {
        ERR_PRINT("Failed open '%s' file\n", fname);
        return -1;
    }

    return 1;
}

// This parse_str_to_list() split string to the tokens, and put the tokens in token_list.
// The value of return is the number of tokens.
int parse_str_to_list(const char* str, char** token_list) {
    //fill this func
    char** token_tmp = token_list;
    int nr_token = 0;
    char* token=strtok(str, "/\n");
  //  printf("12\n");

    for(char* tmp = token; tmp != NULL;){
      //  printf("in\n");
        token_list[nr_token]=(char*)malloc(sizeof(char)*256);
        strcpy(token_list[nr_token], tmp);
        nr_token++;
        tmp = strtok(NULL, "/\n");
    }
    //printf("12\n");
    // for(int k = 0; k<nr_token; k++){
    //      printf("%s\n", token_list[k]);

    return nr_token;
}

void free_token_list(char** token_list, int num_tokens) {
    for (int i = 0; i < num_tokens; i++) {
        free(token_list[i]);
    }
    free(token_list);
}
