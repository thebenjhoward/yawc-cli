#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "words.h"
#include "game.h"


int main(int argc, char *argv[]) {
    /*{
        Board *b = create_board(create_default_rules());

        print_board(b, 1);

        read_line(NULL, NULL, NULL);

        b->keys[20].status = IS_INW_STATUS;
        b->keys[21].status = NOT_INW_STATUS;
        b->keys[22].status = CORRECT_STATUS;

        Guess guess;
        Key keys[5] = {{'P', CORRECT_STATUS}, {'R', IS_INW_STATUS}, {'O', NOT_INW_STATUS}, {'X', CORRECT_STATUS}, {'Y', NOT_INW_STATUS}};
        guess.keys = keys;

        add_guess(guess, b);

        print_board(b, 1);

        delete_board(b);

        HashTable *table = read_guesses("/home/benh/dev/yawc-cli/lists/wordle/guesses.txt");

        printf("Testing ZYGON\n");
        if(valid_guess("ZYGON", table)) {
            printf("Validated ZYGON\n");
        } else {
            printf("Failed to validate ZYGON\n");
        }

        printf("Negative testing AAAAA\n");
        if(valid_guess("AAAAA", table)) {
            printf("Failed to reject AAAAA\n");
        } else {
            printf("Rejected AAAAA\n");
        }

        char *word = select_answer("/home/benh/dev/yawc-cli/lists/wordle/answers.txt");

        printf("Selected %s\n", word);

        delete_table(table);
        free(word);
        table = NULL;
    }*/


    

    return play_default_game();
}