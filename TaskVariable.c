#include "TaskVariable.h"


void freeAllVariables(VariableObject* head){
	if (!head) {
		return;
	}
	freeAllVariables(head->nextVariable);
	g_free(head->variableName);
	g_free(head->regexInput0);
	g_free(head->regexInput1);
	g_free(head->generatedRegex);
	g_free(head->valueFound);
	g_free(head->nextVariable);
}


VariableObject* getLastVariableObject(VariableObject* head){
	VariableObject* currentObj = head;
	while(currentObj->nextVariable){
		currentObj = currentObj->nextVariable;	
	}
	return currentObj;
}


VariableObject* findVariableObjectByIndex(VariableObject* head, short int index){
	VariableObject* searchedObject = head;
	while(searchedObject){

		if(searchedObject->index == index){
			return searchedObject;
		}

		searchedObject = searchedObject->nextVariable;
	}
	return NULL;
}


void printAllVariableObjects(VariableObject* head){
	VariableObject* currentObj = head;
	printf("\n-----------------------------------------");
	while(currentObj){
		printf("\nindex: %d\nidentificationMethod: %d\nvariableName: %s\nregexInput0: %s\nregexInput1: %s\nregex: %s\nvalueFound: %s\nconvertedValue: %f\nisVaildValue: %d\n\n", currentObj->index, currentObj->identificationMethod, currentObj->variableName, currentObj->regexInput0, currentObj->regexInput1, currentObj->generatedRegex, currentObj->valueFound, currentObj->convertedValue, currentObj->isVaildValue);
		currentObj = currentObj->nextVariable;	
	}
	printf("\n-----------------------------------------");
}


cJSON* variableObjectToJson(VariableObject* object){

	cJSON *objectJson = cJSON_CreateObject();
	if(object){
		if(objectJson == NULL) goto end;

		if(cJSON_AddNumberToObject(objectJson, "index", object->index) == NULL) goto end;
		if(cJSON_AddNumberToObject(objectJson, "identificationMethod", object->identificationMethod) == NULL) goto end;
		if(cJSON_AddStringToObject(objectJson, "variableName", object->variableName) == NULL) goto end;
		if(cJSON_AddStringToObject(objectJson, "regexInput0", object->regexInput0) == NULL) goto end;
		if(cJSON_AddStringToObject(objectJson, "regexInput1", object->regexInput1) == NULL) goto end;
		if(cJSON_AddStringToObject(objectJson, "generatedRegex", object->generatedRegex) == NULL) goto end;
		if(cJSON_AddStringToObject(objectJson, "valueFound", object->valueFound) == NULL) goto end;
		if(cJSON_AddNumberToObject(objectJson, "isVaildValue", object->isVaildValue) == NULL) goto end;
    }
	end:
    	return objectJson;
}

cJSON* variablesObjectsToJsonArray(VariableObject* head){
	VariableObject* currentObj = head;

	cJSON *variable = NULL;

	cJSON *variables = cJSON_CreateObject();
	if(variables == NULL) goto end;

 	cJSON *variablesArray = cJSON_CreateArray();
 	if(variablesArray == NULL) goto end;

 	cJSON_AddItemToObject(variables, "variables", variablesArray);

	while(currentObj){

		variable = variableObjectToJson(currentObj);

		cJSON_AddItemToArray(variablesArray, variable);

		currentObj = currentObj->nextVariable;	
	}
	end:
		return variables;
}

VariableObject* addVariableObjectToList(short int index, short int selectedMode, char* variableName, char* regexInput0, char* regexInput1, char* regex, char* valueFound){
	VariableIdentificationMethod identificationMethod = {0};
	switch(selectedMode){
		case 0:
			identificationMethod = BETWEEN_PHARSES;
			break;
		case 1:
			identificationMethod = AFTER_PHARSE;
			break;
		case 2:
			identificationMethod = REGEX_EXPRESSION;
	}


	if(variableListHead){
		VariableObject* searchedObject = findVariableObjectByIndex(variableListHead, index);
		if(searchedObject){

			g_free(searchedObject->variableName);
			g_free(searchedObject->regexInput0);
			g_free(searchedObject->regexInput1);
			g_free(searchedObject->generatedRegex);
			g_free(searchedObject->valueFound);



			searchedObject->identificationMethod = identificationMethod;
			searchedObject->variableName = variableName;
			searchedObject->regexInput0 = regexInput0;
			searchedObject->regexInput1 = regexInput1;
			searchedObject->generatedRegex = regex;
			searchedObject->valueFound = valueFound;

			printf("UPDATER\n");
			//printAllVariableObjects(variableListHead);

			return searchedObject;

		}else{

			VariableObject* newVariableObject = (VariableObject *)g_malloc(sizeof(VariableObject));

			newVariableObject->index = index;
			newVariableObject->identificationMethod = identificationMethod;
			newVariableObject->variableName = variableName;
			newVariableObject->regexInput0 = regexInput0;
			newVariableObject->regexInput1 = regexInput1;
			newVariableObject->generatedRegex = regex;
			newVariableObject->valueFound = valueFound;
			newVariableObject->nextVariable = NULL;

			getLastVariableObject(variableListHead)->nextVariable = newVariableObject;	

			printf("DODWANIE\n");

			//printAllVariableObjects(variableListHead);

			return newVariableObject;
		}
	}else{
		variableListHead = (VariableObject *)g_malloc(sizeof(VariableObject));

		variableListHead->index = index;
		variableListHead->identificationMethod = identificationMethod;
		variableListHead->variableName = variableName;
		variableListHead->regexInput0 = regexInput0;
		variableListHead->regexInput1 = regexInput1;
		variableListHead->generatedRegex = regex;
		variableListHead->valueFound = valueFound;
		variableListHead->nextVariable = NULL;

		printf("PIERWSZY\n");

		//printAllVariableObjects(variableListHead);

		return variableListHead;
	}

	return NULL;
}


