#ifndef GAME_H
#define GAME_H

#include "words.h"
#include "board.h"
#include <stddef.h>



void read_line(char *prompt, char **line, size_t *size);

int play_default_game();

#endif