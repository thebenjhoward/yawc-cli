#ifndef WORDS_H
#define WORDS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char **words;
    size_t size;
    int word_len;
} HashTable;




// Reads all words from a properly formatted word file
HashTable *read_guesses(char *path);

// Selects a random answer from a properly formatted answer
// list.
char *select_answer(char *path);

// Frees all memory associated with the hash table
void delete_table(HashTable *table);

// Looks up a word to verify if it is in the guess list
bool valid_guess(char *guess, HashTable *table);

#endif