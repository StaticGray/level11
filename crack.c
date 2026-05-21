#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"
#include "fileutil.h"

#define PASS_LEN 50
#define HASH_LEN 33

void trim_newline(char str[]);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        return 1;
    }

    /*============ LOAD HASH FILE ============*/

    int size;
    int found = 0;
    char **hashes = loadFileAA(argv[1], &size);

    /*============ OPEN DICTIONARY FILE ============*/

    FILE *dict_fp = fopen(argv[2], "r");

    if (dict_fp == NULL) {

        printf("Could not open dictionary file.\n");
        freeAA(hashes, size);
        return 1;
    }

    /*============ CRACK LOOP ============*/

    char password[PASS_LEN];

    while (fgets(password, sizeof(password), dict_fp)) {

        trim_newline(password);
        char *hash = md5(password, strlen(password));

        if (exactSearchAA(hash, hashes, size) != NULL) {
            printf("%s -> %s\n", password, hash);
            found++;
        }
        free(hash);
    }

    /*============ CLEANUP ============*/

    fclose(dict_fp);
    freeAA(hashes, size);
    printf("Hashes cracked: %d\n", found);

    return 0;
}

/*============ TRIM NEWLINE ============*/

void trim_newline(char str[]) {
    str[strcspn(str, "\r\n")] = '\0';
}
