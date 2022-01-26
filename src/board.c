#include "board.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

const uint8_t DEFAULT_STATUS = 0;
const uint8_t NOT_INW_STATUS = 1;
const uint8_t IS_INW_STATUS = 3;
const uint8_t CORRECT_STATUS = 7;

#pragma region Style Constants
// Colors
const uint16_t COLOR_BLACK_FG = 0x0001;
const uint16_t COLOR_BLACK_BG = 0x0010;
const uint16_t COLOR_RED_FG = 0x0002;
const uint16_t COLOR_RED_BG = 0x0020;
const uint16_t COLOR_GREEN_FG = 0x0003;
const uint16_t COLOR_GREEN_BG = 0x0030;
const uint16_t COLOR_YELLOW_FG = 0x0004;
const uint16_t COLOR_YELLOW_BG = 0x0040;
const uint16_t COLOR_BLUE_FG = 0x0005;
const uint16_t COLOR_BLUE_BG = 0x0050;
const uint16_t COLOR_MAGENTA_FG = 0x0006;
const uint16_t COLOR_MAGENTA_BG = 0x0060;
const uint16_t COLOR_CYAN_FG = 0x0007;
const uint16_t COLOR_CYAN_BG = 0x0070;
const uint16_t COLOR_WHITE_FG = 0x0008;
const uint16_t COLOR_WHITE_BG = 0x0080;

// Print Styles
const uint16_t STYLE_DEFAULT = 0x0000;
const uint16_t STYLE_BOLD = 0x0100;
const uint16_t STYLE_ITALICS = 0x0200;
const uint16_t STYLE_UNDERLINE = 0x0400;
const uint16_t STYLE_STRIKETHROUGH = 0x0800;
#pragma endregion


int key_i(char key) {
    switch(key) {
        case 'Q':
            return 0;
        case 'W':
            return 1;
        case 'E':
            return 2;
        case 'R':
            return 3;
        case 'T':
            return 4;
        case 'Y':
            return 5;
        case 'U':
            return 6;
        case 'I':
            return 7;
        case 'O':
            return 8;
        case 'P':
            return 9;
        case 'A':
            return 10;
        case 'S':
            return 11;
        case 'D':
            return 12;
        case 'F':
            return 13;
        case 'G':
            return 14;
        case 'H':
            return 15;
        case 'J':
            return 16;
        case 'K':
            return 17;
        case 'L':
            return 18;
        case 'Z':
            return 19;
        case 'X':
            return 20;
        case 'C':
            return 21;
        case 'V':
            return 22;
        case 'B':
            return 23;
        case 'N':
            return 24;
        case 'M':
            return 25;
        default:
            return -1;
    }
}

Rules *create_default_rules(int word_len) {
    uint16_t defaultStyle = STYLE_DEFAULT | COLOR_WHITE_FG;
    uint16_t notInwStyle = STYLE_DEFAULT | COLOR_RED_FG;
    uint16_t isInwStyle = STYLE_DEFAULT | COLOR_YELLOW_FG;
    uint16_t correctStyle = STYLE_DEFAULT | COLOR_GREEN_FG;
    return create_rules(word_len, 6, true, defaultStyle, notInwStyle, isInwStyle, correctStyle);
}

Rules *create_rules(int word_len, uint16_t max_guess, bool print_keyboard, uint16_t default_style,
    uint16_t not_inw_style, uint16_t is_inw_style, uint16_t correct_style) {
    
    Rules *rules;

    rules = malloc(sizeof(Rules));

    rules->WORD_LEN = word_len;
    rules->MAX_GUESS = max_guess;
    rules->PRINT_KEYBOARD = print_keyboard;
    rules->DEFAULT_STYLE = default_style;
    rules->NOT_INW_STYLE = not_inw_style;
    rules->IS_INW_STYLE = is_inw_style;
    rules->CORRECT_STYLE = correct_style;
    
    return rules;
}

Key *make_keys() {
    Key *keys;
    keys = calloc(26, sizeof(Key));

    keys[0] = (Key){ 'Q', DEFAULT_STATUS };
    keys[1] = (Key){ 'W', DEFAULT_STATUS };
    keys[2] = (Key){ 'E', DEFAULT_STATUS };
    keys[3] = (Key){ 'R', DEFAULT_STATUS };
    keys[4] = (Key){ 'T', DEFAULT_STATUS };
    keys[5] = (Key){ 'Y', DEFAULT_STATUS };
    keys[6] = (Key){ 'U', DEFAULT_STATUS };
    keys[7] = (Key){ 'I', DEFAULT_STATUS };
    keys[8] = (Key){ 'O', DEFAULT_STATUS };
    keys[9] = (Key){ 'P', DEFAULT_STATUS };

    keys[10] = (Key){ 'A', DEFAULT_STATUS };
    keys[11] = (Key){ 'S', DEFAULT_STATUS };
    keys[12] = (Key){ 'D', DEFAULT_STATUS };
    keys[13] = (Key){ 'F', DEFAULT_STATUS };
    keys[14] = (Key){ 'G', DEFAULT_STATUS };
    keys[15] = (Key){ 'H', DEFAULT_STATUS };
    keys[16] = (Key){ 'J', DEFAULT_STATUS };
    keys[17] = (Key){ 'K', DEFAULT_STATUS };
    keys[18] = (Key){ 'L', DEFAULT_STATUS };

    keys[19] = (Key){ 'Z', DEFAULT_STATUS };
    keys[20] = (Key){ 'X', DEFAULT_STATUS };
    keys[21] = (Key){ 'C', DEFAULT_STATUS };
    keys[22] = (Key){ 'V', DEFAULT_STATUS };
    keys[23] = (Key){ 'B', DEFAULT_STATUS };
    keys[24] = (Key){ 'N', DEFAULT_STATUS };
    keys[25] = (Key){ 'M', DEFAULT_STATUS };

    return keys;
}

char *get_format(uint8_t status, Rules *rules) {
    // TODO: Support styles other than fg color
    uint16_t format;
    if(status == DEFAULT_STATUS) {
        format = rules->DEFAULT_STYLE;
    } else if(status == NOT_INW_STATUS) {
        format = rules->NOT_INW_STYLE;
    } else if(status == IS_INW_STATUS) {
        format = rules->IS_INW_STYLE;
    } else if(status == CORRECT_STATUS) {
        format = rules->CORRECT_STYLE;
    }

    format = format & 0x000F;
    if(format == COLOR_BLACK_FG) {
        return "\033[30m%c\033[0m ";
    } else if (format == COLOR_RED_FG) {
        return "\033[31m%c\033[0m ";
    } else if (format == COLOR_GREEN_FG) {
        return "\033[32m%c\033[0m ";
    } else if (format == COLOR_YELLOW_FG) {
        return "\033[33m%c\033[0m ";
    } else if (format == COLOR_BLUE_FG) {
        return "\033[34m%c\033[0m ";
    } else if (format == COLOR_MAGENTA_FG) {
        return "\033[35m%c\033[0m ";
    } else if (format == COLOR_CYAN_FG) {
        return "\033[36m%c\033[0m ";
    } else if (format == COLOR_WHITE_FG) {
        return "\033[37m%c\033[0m ";
    } else {
        return "%c ";
    }
}

Board *create_board(Rules *rules) {
    Board *board;
    board = malloc(sizeof(Board));

    board->status_message = calloc(1024, sizeof(char));
    strcpy(board->status_message, "Welcome to YAWC");

    board->keys = make_keys();

    Guess *guesses;
    guesses = calloc(rules->MAX_GUESS, sizeof(Guess));
    board->guesses = guesses;
    board->guess_num = 0;

    board->rules = rules;

    board->default_format = get_format(DEFAULT_STATUS, rules);
    board->not_inw_format = get_format(NOT_INW_STATUS, rules);
    board->is_inw_format = get_format(IS_INW_STATUS, rules);
    board->correct_format = get_format(CORRECT_STATUS, rules);

    return board;
}

void delete_board(Board *board) {
    for(int i = 0; i < board->guess_num; i++) {
        free(board->guesses[i].keys);
    }
    free(board->status_message);
    free(board->guesses);
    free(board->rules);
    free(board);
}

void add_guess(Guess guess, Board *board) {
    assert(board->guess_num < board->rules->MAX_GUESS);

    board->guesses[board->guess_num].keys = guess.keys;

    for(int i = 0; i < board->rules->WORD_LEN; i++) {
        board->keys[key_i(guess.keys[i].ch)].status |= guess.keys[i].status;
    } 

    board->guess_num++;
}



void print_key(Key key, Board *board) {
    if(key.status == DEFAULT_STATUS) {
        printf(board->default_format, key.ch);
    } else if(key.status == NOT_INW_STATUS) {
        printf(board->not_inw_format, key.ch);
    } else if(key.status == IS_INW_STATUS) {
        printf(board->is_inw_format, key.ch);
    } else if(key.status == CORRECT_STATUS) {
        printf(board->correct_format, key.ch);
    }
}

void print_guesses(Board *board) {
    for(int i = 0; i < board->rules->MAX_GUESS; i++) {
        // TODO: Dynamic centering, or just right align it for custom lengths
        if(i == board->guess_num) {
            printf("   > ");
        } else {
            printf("     ");
        }


        if(i < board->guess_num) {
            for(int j = 0; j < board->rules->WORD_LEN; j++) {
                print_key(board->guesses[i].keys[j], board);
            }
        } else {
            for(int j = 0; j < board->rules->WORD_LEN; j++) {
                printf("_ ");
            }
        }
        printf("\n");
    }
}

// Prints a representation of the keyboard in QWERTY order.
void print_keyboard(Board *board) {
    for(int i = 0; i < 26; i++) {
        if(i == 10) {
            printf(" ");
        }
        if(i == 19) {
            printf("   ");
        }

        print_key(board->keys[i], board);

        if(i == 9 || i ==18 || i == 25) {
            printf("\n");
        }
    }
}

void print_board(Board *board, bool reprint) {
    if(reprint) {
        printf("\033[2J\033[H");
    }
    printf("%s\n\n", board->status_message);

    print_guesses(board);

    printf("\n");

    print_keyboard(board);
}