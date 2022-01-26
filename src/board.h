#ifndef BOARD_H
#define BOARD_H
#include <stdint.h>
#include <stdbool.h>


extern const uint8_t DEFAULT_STATUS;
extern const uint8_t NOT_INW_STATUS;
extern const uint8_t IS_INW_STATUS;
extern const uint8_t CORRECT_STATUS;

#pragma region Style Constants
// Colors
extern const uint16_t COLOR_BLACK_FG;
extern const uint16_t COLOR_BLACK_BG;
extern const uint16_t COLOR_RED_FG;
extern const uint16_t COLOR_RED_BG;
extern const uint16_t COLOR_GREEN_FG;
extern const uint16_t COLOR_GREEN_BG;
extern const uint16_t COLOR_YELLOW_FG;
extern const uint16_t COLOR_YELLOW_BG;
extern const uint16_t COLOR_BLUE_FG;
extern const uint16_t COLOR_BLUE_BG;
extern const uint16_t COLOR_MAGENTA_FG;
extern const uint16_t COLOR_MAGENTA_BG;
extern const uint16_t COLOR_CYAN_FG;
extern const uint16_t COLOR_CYAN_BG;
extern const uint16_t COLOR_WHITE_FG;
extern const uint16_t COLOR_WHITE_BG;

// Print Styles
extern const uint16_t STYLE_DEFAULT;
extern const uint16_t STYLE_BOLD;
extern const uint16_t STYLE_ITALICS;
extern const uint16_t STYLE_UNDERLINE;
extern const uint16_t STYLE_STRIKETHROUGH;
#pragma endregion

// A key and its status. Used for printing
typedef struct {
    char ch;
    uint8_t status;
} Key;

// the string of a guess and the array of statuses for each letter
typedef struct {
    Key *keys;
} Guess;

// the rules of the game
typedef struct {
    // the length of the word being guessed
    int WORD_LEN;

    // maximum number of guesses allowed
    uint16_t MAX_GUESS;

    // whether or not to print the keys as a keyboard or as a list of
    // each category
    bool PRINT_KEYBOARD;

    // the style to print each key state as. Made by
    // or-ing together a STYLE const and a COLOR const
    // (ex: CORRECT_STYLE = STYLE_BOLD | COLOR_GREEN)
    uint16_t DEFAULT_STYLE;
    uint16_t NOT_INW_STYLE;
    uint16_t IS_INW_STYLE;
    uint16_t CORRECT_STYLE;
} Rules;

// The current state of the board, and the game.
typedef struct {
    // list of each key and its current status (e.g. Green, Grey, Yellow, White). 
    // Should be size 26
    Key *keys; 
    
    Guess *guesses;
    char guess_num;

    char *status_message;

    Rules *rules;

    char *default_format;
    char *not_inw_format;
    char *is_inw_format;
    char *correct_format;

} Board;

// Get the qwerty index of a given char, if applicable 
int key_i(char key);

// Create a rules object with all the default values aside from the word length
Rules *create_default_rules(int word_len);

// Create a rules object, setting each value individually
Rules *create_rules(int word_len, uint16_t max_guess, bool print_keyboard, uint16_t default_style,
    uint16_t not_inw_style, uint16_t is_inw_style, uint16_t correct_style);

// creates a board with a given ruleset
Board *create_board(Rules *rules);

// Deletes a board and ruleset
void delete_board(Board *board);

// Adds a new guess to the board. Does not modify the keys
// array. This will be done by the game
void add_guess(Guess guess, Board *board);


// Prints a board. If reprint is true, sets the cursor
// to the beginning of the previous board and prints over it.
void print_board(Board *board, bool reprint);

#endif