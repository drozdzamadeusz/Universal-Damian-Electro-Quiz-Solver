#ifndef MAIN_MENU_H
#define MAIN_MENU_H


#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include "TaskSolver.h"



int showMainMenu(int argc, char* argv[]);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif