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
//#include "Memd/mem.h"
#include "preg_replace.h"
#include "JsonHelper.h"
#include "AddTaskWindow.h"
#include "MainMenu.h"


#define DISABLE_PRINTF

#ifdef DISABLE_PRINTF
    #define printf(fmt, ...) (0)
#endif
 
main(int argc, char* argv[]){



    initMainMenu(argc, argv);

  return 0;
}
