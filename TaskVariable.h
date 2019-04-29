#ifndef TASK_VARIABLE_H
#define TASK_VARIABLE_H

#include <stdio.h>
#include <stdlib.h>

#include <libnotify/notify.h>
#include <JavaScriptCore/JavaScript.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <string.h>
#include <pcre2.h>
#include <stdbool.h>

#include "tinyexpr.h"

#include "cJSON.h"


typedef enum VariableIdentificationMethod{
	BETWEEN_PHARSES = 0,
	AFTER_PHARSE = 1,
	REGEX_EXPRESSION = 2
} VariableIdentificationMethod;

typedef struct VariableObject{
	short int index;
	enum VariableIdentificationMethod identificationMethod;
	char* variableName;
	char* regexInput0;
	char* regexInput1;
	char* generatedRegex;
	char* valueFound;
	double convertedValue;
	bool isVaildValue;
	struct VariableObject* nextVariable;
} VariableObject;



VariableObject* varibleListHead;

VariableObject* getLastVariableObject(VariableObject* head);

VariableObject* findVariableObjectByIndex(short int index);

void printAllVariableObjects(VariableObject* head);

cJSON* variableObjectToJson(VariableObject* object);

cJSON* variablesObjectsToJsonArray(VariableObject* head);

VariableObject* addVariableObjectToList(short int index, short selectedMode, char* variableName, char* regexInput0, char* regexInput1, char* regex, char* valueFound);

#endif
