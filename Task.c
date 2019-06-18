#include "Task.h"

void initTask(){
	task = (Task *)g_malloc(sizeof(Task)+1000);

    task->content = NULL;
    task->contentSlug = NULL;
    task->vriablesRegistered = NULL;
    task->formula = NULL;
    task->unit = NULL;
    task->additionalInformation = NULL;

}

void freeTask(){
   	if(task->content) g_free(task->content);
    if(task->contentSlug) g_free(task->contentSlug);
    if(task->vriablesRegistered) g_free(task->vriablesRegistered);
    if(task->formula) g_free(task->formula);
    if(task->unit) g_free(task->unit);
    if(task->additionalInformation) g_free(task->additionalInformation);
	

    freeAllVariables(variableListHead);
    variableListHead = NULL;
}


cJSON* taskToJson(){
    
    cJSON* json = variablesObjectsToJsonArray(variableListHead);


    char resultValue[22];
    snprintf(resultValue, 22, "%.4lf", task->resultValue);

    task->contentSlug =  getContentSlug(task->content);

    cJSON_AddStringToObject(json, "content", task->content);
    cJSON_AddStringToObject(json, "contentSlug", task->contentSlug);
    cJSON_AddStringToObject(json, "vriablesRegistered", task->vriablesRegistered);
    cJSON_AddStringToObject(json, "formula", task->formula);
    cJSON_AddStringToObject(json, "unit", task->unit);
    cJSON_AddStringToObject(json, "additionalInformation", task->additionalInformation);
    cJSON_AddStringToObject(json, "resultValue", resultValue);

    return json;
}