#ifndef API_SERVICE_H
#define API_SERVICE_H
#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <libnotify/notify.h>
#include <JavaScriptCore/JavaScript.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <stdio.h>
#include <string.h>
#include <pcre2.h>
#include <stdbool.h>


#include "https.h"
#include "cJSON.h"

void sendTaskToApi(cJSON* task);

char* getTask(cJSON* task);

#endif