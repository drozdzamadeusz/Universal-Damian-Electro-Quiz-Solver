#ifndef MATH_EQUATIONS_PARSER_H
#define MATH_EQUATIONS_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include <libnotify/notify.h>
#include <JavaScriptCore/JavaScript.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <stdio.h>
#include <string.h>
#include <pcre2.h>
#include <stdbool.h>



#include<ncurses.h>
 

#include "tinyexpr.h"

#include "TaskVariable.h"
#include "TextProcessing.h"
#include "MathEquationsParser.h"
#include "ApiService.h"
#include "preg_replace.h"

typedef enum CalculationStatus{
    CALCULATED_CORRECTLY = 0,
    PARSE_ERROR = 1
} CalculationStatus;


typedef struct CalculationResult{
    CalculationStatus status;
    double result;
    int errorPosition;
} CalculationResult;



bool isDouble(const char *str);
double convertToDouble(const char *str);
CalculationResult* calculateTask(VariableObject* variableListHead, char* taskFormula);

#endif