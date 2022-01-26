#include "words.h"
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


// gets the length of an integer value in a given base
size_t log_length(int x, int base) {
    return pow(base, (unsigned int)((log(x) / log(base)) + 1));
}

// pretty crappy hash function I put together in like 5 mins. On the test list
// collisions were in the vast minority, but were still decently high (20%?).
size_t get_hash(char *word, size_t table_size, size_t word_len) {
    size_t sum = 0;
    for(size_t i = 0; i < word_len; i++) {
        sum += word[i];
    }

    size_t hash = sum;

    for(size_t i = 0; i < word_len; i++) {
        hash = (hash * word[i] + word[i]) % table_size;
    }

    hash = (hash - sum) % table_size;

    return hash;
}

char *scan_word(FILE *fp, unsigned int word_len) {
    char *word = (char*)malloc((word_len + 1) * sizeof(char));
    fscanf(fp, "%s\n", word);
    assert(strlen(word) == word_len);
    return word;
}

void add_word(char *word, HashTable *table) {
    size_t hash = get_hash(word, table->size, table->word_len);

    size_t i = hash;

    do {
        if(table->words[i] == NULL) {
            table->words[i] = word;
            return;
        } else {
            i = (i + 1) % table->size;
        }
    } while(i != hash);
}

size_t rand_seeded(size_t max) {
    time_t now = time(0);
    srand(now);

    return rand() % (max+1);
}

char *select_answer(char *path) {
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        perror("Failed: ");
        return NULL;
    }

    unsigned int word_len;

    fscanf(fp, "%u\n", &word_len);

    long curr_pos = ftell(fp);

    fseek(fp, 0, SEEK_END);

    long end_pos = ftell(fp);

    fseek(fp, curr_pos, SEEK_SET);

    size_t word_count = (end_pos - curr_pos) / (word_len + 1);

    size_t word_num = rand_seeded(word_count);

    fseek(fp, word_num * (word_len + 1), SEEK_CUR);

    char *word = scan_word(fp, word_len);

    fclose(fp);

    return word;
}


HashTable *read_guesses(char *path) {
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        perror("Failed: ");
        return NULL;
    }

    unsigned int word_len;

    fscanf(fp, "%u\n", &word_len);

    long curr_pos = ftell(fp);

    fseek(fp, 0, SEEK_END);

    long end_pos = ftell(fp);

    fseek(fp, curr_pos, SEEK_SET);

    // printf("Word Length: %d\n", word_len);

    // printf("Pos Difference: %ld\n", end_pos - curr_pos);

    // printf("Assumed word count: %ld\n", (end_pos - curr_pos) / (word_len + 1));

    // printf("HashTable Size: %zu\n", log_length((end_pos - curr_pos) / (word_len + 1), 2));

    long word_count = (end_pos - curr_pos) / (word_len + 1);

    HashTable *table = (HashTable*)malloc(sizeof(HashTable));

    table->size = log_length(word_count, 2);
    table->word_len = word_len;

    table->words = (char**)calloc(table->size, sizeof(char*));
    for(size_t i = 0; i < word_count; i++) {
        char *word = scan_word(fp, word_len);
        add_word(word, table);
    }
    

    fclose(fp);

    return table;
}

void delete_table(HashTable *table) {
    for(size_t i = 0; i < table->size; i++) {
        if(table->words[i] != NULL) {
            free(table->words[i]);
        }
    }
    free(table->words);
    free(table);
}

bool valid_guess(char *guess, HashTable *table) {
    size_t hash = get_hash(guess, table->size, table->word_len);
    if(strcmp(guess, table->words[hash]) == 0) {
        return true;
    }

    size_t i = hash + 1;


    while(i != hash) {
        if(table->words[i] == NULL) {
            return false;
        } else if(strcmp(guess, table->words[i]) == 0) {
            return true;
        } else {
            i = (i + 1) % table->size;
        }
    }

    return false;
}

