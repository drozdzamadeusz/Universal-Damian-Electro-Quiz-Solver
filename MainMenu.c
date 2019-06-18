#include "MainMenu.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4


char* menuChoices[] = {
                        "Solve the task                    ",
                        "Add a task                        ",
                        "Exit                              ",
                        (char *)NULL,
                  };


int initMainMenu(int argc, char* argv[]){
	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
	

    int windowHeight, windowWidth;


	/* Initialize curses */
	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);


	getmaxyx(stdscr, windowHeight, windowWidth);

    curs_set(0);

	/* Create items */
        n_choices = ARRAY_SIZE(menuChoices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(menuChoices[i], "");

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(10, 40, 5, 20);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
        //set_menu_format(my_menu, 5, 3);

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " > ");

	/* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
	printInMiddle(my_menu_win, 1, 0, 40, "Universal Electro Quiz Solver   ", COLOR_PAIR(1));
	
	wattron(my_menu_win, COLOR_PAIR(2));
	mvwprintw(my_menu_win, 1, 34, "%s", "BETA");
	wattroff(my_menu_win, COLOR_PAIR(2));
	refresh();


	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	//mvprintw(LINES - 2, 0, "F1 to exit");
	
	mvprintw(LINES - 3, 0, "                                       ");
	mvprintw(LINES - 2, 0, "API connected successfully!");
	mvprintw(LINES - 1, 0, "Use Arrow Keys to navigate (F1 to Exit)");



	refresh();
        
	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);


	while((c = wgetch(my_menu_win)) != KEY_F(1)){    
		switch(c){
	        case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: /* Enter */
				move(20, 0);
				clrtoeol();
				//mvprintw(LINES - 2, 0, "Item selected is : %s", item_name(current_item(my_menu)));

				wattron(my_menu_win, COLOR_PAIR(2));
				mvprintw(LINES - 3, 0, "CLOSE WINDOW TO CHANGE MODE!");
				wattroff(my_menu_win, COLOR_PAIR(2));

				refresh();

				pos_menu_cursor(my_menu);
                refresh();

                char option[43];// = strcpy(item_name(current_item(my_menu)));


				strncpy(option, item_name(current_item(my_menu)), 43);


				if(strcmp(option, menuChoices[0]) == 0){
					initSolveTaskWindow(argc, argv);
				}else if(strcmp(option, menuChoices[1]) == 0){
					initAddTaskWindow(argc, argv);
				}else{
					goto end;
				}

				unpost_menu(my_menu);
			    free_menu(my_menu);
			    for(i = 0; i < n_choices; ++i)
			        free_item(my_items[i]);
				endwin();
				
				initMainMenu(argc, argv);
                
				//goto end;

				return 0;

				break;
		}

					  
	}



	/* Unpost and free all the memory taken up */
	end:
	//if(!exited){
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
		endwin();
	//}
	
}

void printInMiddle(WINDOW *win, int starty, int startx, int width, char *string, chtype color){	
	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
