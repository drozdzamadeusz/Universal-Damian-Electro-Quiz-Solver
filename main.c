#include<stdio.h>
#include<stdlib.h>

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <libnotify/notify.h>
#include <JavaScriptCore/JavaScript.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <stdio.h>
#include <string.h>
#include <pcre2.h>
#include <stdbool.h>


#include <time.h>

#include <ncurses.h>
 

#include "tinyexpr.h"

#include "Task.h"
#include "TaskVariable.h"
#include "TextProcessing.h"
#include "MathEquationsParser.h"
#include "ApiService.h"
#include "Memd/mem.h"
#include "preg_replace.h"
#include "JsonHelper.h"
#include "AddTaskWindow.h"
#include "MainMenu.h"



 
main(int argc, char* argv[]){

showMainMenu(argc, argv);

/*
  char choice=0;
  while(choice!='3')
  {
    printf("\n\tMENU DEMONSTRATION");
    printf("\n\t------------------------------");
    printf("\n\n\t 1. ADD A TASK");
    printf("\n\t 2. SOLVE THE TASK");
    printf("\n\t 3. EXIT");
    printf("\n\n Enter Your Choice: ");
    scanf(" %c", &choice);
    switch(choice)
    {
    case '1':
        initAddTaskWindow(argc, argv);
        break;
    case '2':
        solveTask();
        break;
    case '3':
        printf("\n\nYOU SELECTED OPTION 3\n");
        break;
    default:
        printf("\n\nINVALID SELECTION...Please try again\n");
    }
    (void)getch();
  }*/
  return 0;
}
