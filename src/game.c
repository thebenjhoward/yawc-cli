#include "game.h"
#include "board.h"
#include "words.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void read_line(char *prompt, char **line, size_t *len) {
    if(line == NULL || len == NULL) {
        char *my_line = NULL;
        size_t my_len = 0;
        getline(&my_line, &my_len, stdin);

        free(my_line);
    } else {
        printf("%s", prompt);
        getline(line, len, stdin);
    }
}

char **word_list(char *name) {
    const char *word_lists = getenv("YAWC_LISTS");
    if(!word_lists) {
        fprintf(stderr, "[\033[31mERROR\033[0m]: YAWC_LISTS environment variable not defined\n");
    }
    return NULL;

    char *guesses = malloc((strlen(word_lists) + strlen(name) + 14) * sizeof(char));
    char *answers = malloc((strlen(word_lists) + strlen(name) + 14) * sizeof(char));
    
    sprintf(guesses, "%s/%s/%s", word_lists, name, "guesses.txt");
    sprintf(answers, "%s/%s/%s", word_lists, name, "answers.txt");

    int f_guess = access(guesses, F_OK);
    int r_guess = access(guesses, R_OK);
    int f_answer = access(answers, F_OK);
    int r_answer = access(answers, R_OK);
    if((f_guess | r_guess | f_answer | r_answer) != 0) {
        fprintf(stderr, "[\033[31mERROR\033[0m]: default variables point to inaccessible files:\n");
        if(f_guess != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m does not exist\n", guesses);
        } else if(r_guess != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m is not readable\n", guesses);
        }

        if(f_answer != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m does not exist\n", answers);
        } else if(r_answer != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m is not readable\n", answers);
        }
        free(guesses);
        free(answers);
        return NULL;
    } else {
        char **defaults = (char**)(calloc(2, sizeof(char**)));
        defaults[0] = guesses;
        defaults[1] = answers;
        return defaults;
    }

}

char **default_words() {
    const char *word_lists = getenv("YAWC_LISTS");
    const char *default_list = getenv("YAWC_DEFAULT");
    if(!word_lists || !default_list) {
        if(!word_lists) {
            fprintf(stderr, "[\033[31mERROR\033[0m]: YAWC_LISTS environment variable not defined\n");
        }
        if(!default_list) {
            fprintf(stderr, "[\033[31mERROR\033[0m]: YAWC_DEFAULT environment variable not defined\n");
        }
        return NULL;
    }

    char *guesses = malloc((strlen(word_lists) + strlen(default_list) + 14) * sizeof(char));
    char *answers = malloc((strlen(word_lists) + strlen(default_list) + 14) * sizeof(char));
    
    sprintf(guesses, "%s/%s/%s", word_lists, default_list, "guesses.txt");
    sprintf(answers, "%s/%s/%s", word_lists, default_list, "answers.txt");

    int f_guess = access(guesses, F_OK);
    int r_guess = access(guesses, R_OK);
    int f_answer = access(answers, F_OK);
    int r_answer = access(answers, R_OK);
    if((f_guess | r_guess | f_answer | r_answer) != 0) {
        fprintf(stderr, "[\033[31mERROR\033[0m]: default variables point to inaccessible files:\n");
        if(f_guess != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m does not exist\n", guesses);
        } else if(r_guess != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m is not readable\n", guesses);
        }

        if(f_answer != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m does not exist\n", answers);
        } else if(r_answer != 0) {
            fprintf(stderr, "  - \033[34m%s\033[0m is not readable\n", answers);
        }
        free(guesses);
        free(answers);
        return NULL;
    } else {
        char **defaults = (char**)(calloc(2, sizeof(char**)));
        defaults[0] = guesses;
        defaults[1] = answers;
        return defaults;
    }
}

uint8_t char_status(char *guess, int index, char *answer) {
    char c = guess[index];
    for(int i = 0; i < strlen(answer); i++) {
        if(answer[i] == c) {
            if(i == index) {
                return CORRECT_STATUS;
            } else {
                return IS_INW_STATUS;
            }
        }
    }
    return NOT_INW_STATUS;
}

Guess play_turn(Board *board, HashTable *words, char *answer) {
    char *line = NULL;
    size_t len = 0;

    read_line("> ", &line, &len);
    len = strlen(line);
    while(len - 1 != board->rules->WORD_LEN) {
        sprintf(board->status_message, "Guess must be %d letters long", board->rules->WORD_LEN);
        print_board(board, true);
        read_line("> ", &line, &len);
        len = strlen(line);
    }

    line[len - 1] = '\0';

    for(int i = 0; i < board->rules->WORD_LEN; i++) {
        if(line[i] >= 97 && line[i] <= 122) {
            line[i] -= 32;
        }
    }
    while(!valid_guess(line, words)) {
        sprintf(board->status_message, "Not in word list");
        print_board(board, true);
        read_line("> ", &line, &len);
        for(int i = 0; i < board->rules->WORD_LEN; i++) {
            if(line[i] >= 97 && line[i] <= 122) {
                line[i] -= 32;
            }
        }
    }

    Key *keys = malloc(board->rules->WORD_LEN * sizeof(Key));
    for(int i = 0; i < board->rules->WORD_LEN; i++) {
        keys[i].ch = line[i];
        keys[i].status = NOT_INW_STATUS;
        for(int j = 0; j < board->rules->WORD_LEN; j++) {
            if(answer[j] == line[i]) {
                if(j == i) {
                    keys[i].status |= CORRECT_STATUS;
                } else {
                    keys[i].status |= IS_INW_STATUS;
                }
            }
        }
    }

    // check for duplicates so that colors are correct
    for(int i = 0; i < board->rules->WORD_LEN; i++) {
        if(keys[i].status == IS_INW_STATUS) {
            bool correct_status = false;
            for(int j = 0; j < board->rules->WORD_LEN; j++) {
                if(j == i) { continue; }
                if(answer[j] == keys[i].ch && keys[j].ch != keys[i].ch) {
                    correct_status = true;
                    break;
                }
            }
            if(!correct_status) {
                keys[i].status = NOT_INW_STATUS;
            }
        }
    }
    
    return (Guess){keys};
}


int play_default_game() {
    char **defaults;
    if(!(defaults = default_words())) {
        return -2;
    }

    HashTable *words = read_guesses(defaults[0]);
    char *target_word = select_answer(defaults[1]);

    free(defaults[0]);
    free(defaults[1]);
    free(defaults);

    if(strlen(target_word) != words->word_len) {
        printf("[\033[31mERROR\033[0m]: answers list contains words longer than guess list\n");
        return -2;
    }
    if(!valid_guess(target_word, words)) {
        printf("[\033[31mERROR\033[0m]: selected answer (%s) does not occur in guess list\n", target_word);
        return -2;
    }

    Rules *rules = create_default_rules(words->word_len);
    Board *board = create_board(rules);

    print_board(board, true);

    while(board->guess_num < board->rules->MAX_GUESS) {
        Guess g = play_turn(board, words, target_word);
        add_guess(g, board);

        uint8_t status = CORRECT_STATUS;
        for(int i = 0; i < board->rules->WORD_LEN; i++) {
            status &= g.keys[i].status;
        }

        if(status == CORRECT_STATUS) {
            sprintf(board->status_message, "Congrats! You got the word after %i guesses.", board->guess_num);
            print_board(board, true);
            return 0;
        }

        print_board(board, true);
    }
        



    

    return 0;
}