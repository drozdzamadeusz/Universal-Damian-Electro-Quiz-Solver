#ifndef MATH_EQUATIONS_PARSER_H
#define MATH_EQUATIONS_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include <libnotify/notify.h>
#include <JavaScriptCore/JavaScript.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <string.h>
#include <pcre2.h>
#include <stdbool.h>

#include <ctype.h>
#include <stdbool.h>

bool isDouble(const char *str);

double convertToDouble(const char *str);


#endif