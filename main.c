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



#include<ncurses.h>
 

#include "tinyexpr.h"

#include "TaskVariable.h"
#include "TextProcessing.h"
#include "MathEquationsParser.h"
#include "ApiService.h"
#include "Memd/mem.h"
#include "preg_replace.h"



/* Class initialize */

typedef struct Task{
    char* content;
    char* contentSlug;
    char* vriablesRegistered;
    char* formula;
    char* unit;
    char* additionalInformation;
    double resultValue;
} Task;

Task* task;


static void DataExchangeInitCb(JSContextRef ctx, JSObjectRef object){
/* Inits DataExchange */
    task = (Task *)g_malloc(sizeof(Task));
}


/* Class constructor */
static JSObjectRef DataExchangeConstructorCb(JSContextRef context, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){
    return constructor;
}

/* Class finalize */
static void DataExchangeDestroyCd(JSObjectRef object){
}


char* getStringValue(JSContextRef context, const JSValueRef jsValue){
    JSStringRef valueJSString = JSValueToStringCopy(context, jsValue, NULL);
    char* value = g_new(char, JSStringGetMaximumUTF8CStringSize(valueJSString));
    JSStringGetUTF8CString(valueJSString, value, JSStringGetMaximumUTF8CStringSize(valueJSString));

	JSStringRelease(valueJSString);

	return value;
}

typedef enum CalculationStatus{
    CALCULATED_CORRECTLY = 0,
    PARSE_ERROR = 1
} CalculationStatus;


typedef struct CalculationResult{
    CalculationStatus status;
    double result;
    int errorPosition;
} CalculationResult;



CalculationResult* calculateTask(VariableObject* variableListHead, char* taskFormula){
        VariableObject* currentObj = variableListHead;
        CalculationResult* result = (CalculationResult *)g_malloc(sizeof(CalculationResult)+100);
        result->status = CALCULATED_CORRECTLY;

        short int numberOfVaiables = 0;
        while(currentObj){
            if(!currentObj -> isVaildValue)
                result->status = PARSE_ERROR;
            numberOfVaiables++;
            currentObj = currentObj->nextVariable;  
        }



        if(result->status == CALCULATED_CORRECTLY){
            currentObj = variableListHead;
            te_variable vars[numberOfVaiables];

            for(int i=0;currentObj;i++){
                te_variable uwu = {currentObj->variableName, &(currentObj->convertedValue)};
                vars[i] = uwu;

                g_print("GANG ALBANII: %s - %lf", currentObj->variableName, currentObj->convertedValue);

                if(currentObj->nextVariable)
                    currentObj = currentObj->nextVariable; 
                else
                    break;
            }

            //g_print("JAVASCRIP KURWO: taskFormula: %svariableName: %s valueFound: %s",taskFormula, currentObj->variableName, currentObj->valueFound);
            //g_print("GANG ALBANII: %f", currentObj->convertedValue);


            int err;
            te_expr *expr = te_compile(taskFormula, vars, numberOfVaiables, &err);

            double r = 0.0;
            if (expr) {
                r = te_eval(expr);
                printf("KALKULATORED:%f\n", r);

                te_free(expr);
                result->status = CALCULATED_CORRECTLY;
            
            }else{
                printf("Parse error at %d\n", err);
                result->status = PARSE_ERROR;
                result->errorPosition = err;
            }
            result->result = r;
        }

        return result;

}


/* DataExchange.getSubmittedTaskCb method callback implementation */
static JSValueRef valiidateTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){
    /* At least, one argument must be received */
    //if (argumentCount == 1 && JSValueIsString(context, arguments[0])) {
        /* Converts JSValue to char */


        char* taskContent = getStringValue(context, arguments[0]);
        char* taskVriablesRegistered = getStringValue(context, arguments[1]);
        char* taskFormula = getStringValue(context, arguments[2]);
        char* taskUnit = getStringValue(context, arguments[3]);
        char* taskAdditionalInformation = getStringValue(context, arguments[4]);


        VariableObject* currentObj = variableListHead;

        if(!currentObj || taskVriablesRegistered == "" || taskFormula == "") return JSValueMakeUndefined(context);


        task->content = taskContent;
        task->vriablesRegistered = taskVriablesRegistered;
        task->formula = taskFormula;
        task->unit = taskUnit;
        task->additionalInformation = taskAdditionalInformation;

        //size_t formulaSize = strlen(taskFormula)+10;


        CalculationResult* calculationResult = calculateTask(variableListHead, taskFormula);

        if(calculationResult->status == CALCULATED_CORRECTLY){

            task->resultValue = calculationResult->result;

            char* result = g_malloc(sizeof(task->resultValue)+1000);
            sprintf(result,"%f",task->resultValue);

            JSStringRef v = JSStringCreateWithUTF8CString(result);

            g_print("%s\n", cJSON_Print(variablesObjectsToJsonArray(variableListHead)));

            g_free(result);
            g_free(calculationResult);
            return JSValueMakeString(context, v);
        }

   // }
    g_free(calculationResult);
    return JSValueMakeUndefined(context);
}



static JSValueRef submitTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){

    cJSON* json = variablesObjectsToJsonArray(variableListHead);

    
   // memcpy(resultValue,&(task->resultValue),sizeof(task->resultValue));


    char resultValue[22];
    snprintf(resultValue, 22, "%.4f", task->resultValue);

    cJSON_AddStringToObject(json, "content", task->content);
    cJSON_AddStringToObject(json, "vriablesRegistered", task->vriablesRegistered);
    cJSON_AddStringToObject(json, "formula", task->formula);
    cJSON_AddStringToObject(json, "unit", task->unit);
    cJSON_AddStringToObject(json, "additionalInformation", task->additionalInformation);
    cJSON_AddStringToObject(json, "resultValue", resultValue);
    

    g_print("%s\n", cJSON_Print(json));

    char* contentSlug =  getContentSlug(task->content);

    cJSON_AddStringToObject(json, "contentSlug", contentSlug);
    

    sendTaskToApi(json);

    cJSON_Delete(json);
    g_free(contentSlug);

    return JSValueMakeUndefined(context);
}


/* DataExchange.findVariableCb method callback implementation */
static JSValueRef findVariableCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){
    if (argumentCount == 6) {

    	short int index = (int) JSValueToNumber(context, arguments[0], NULL);
    	short int selectedMode = (int) JSValueToNumber(context, arguments[1], NULL);
        char* variableName = getStringValue(context, arguments[2]);
        char* regexInput0 = getStringValue(context, arguments[3]);
        char* regexInput1 = getStringValue(context, arguments[4]);
        char* taskContent = getStringValue(context, arguments[5]);


        char *regex;

        if(selectedMode == 0){

        	regex = g_malloc(strlen(regexInput0)+strlen(regexInput1)+strlen("(?<=)(.*)(?=)")+1);
        	sprintf(regex, "(?<=%s)(.*)(?=%s)", regexInput0, regexInput1);

        }else if(selectedMode == 1){

        	regex = g_malloc(strlen(regexInput0)+strlen("(?<=)([ ]*?)([-+]?[0-9]*[.,]?[0-9]+)")+1);
        	sprintf(regex, "(?<=%s)([ ]*?)([-+]?[0-9]*[.,]?[0-9]+)", regexInput0);

        }else if(selectedMode == 2){
        	regex = g_malloc(strlen(regexInput0)+1);
        	strcpy(regex, regexInput0);
        }

		char*  output = getVaraibleFormText(regex, taskContent);
    

        output = replace(output,  " ", "");
        output = replace(output,  "\n", "");   

        JSStringRef v = JSStringCreateWithUTF8CString(output);

        output = replace(output,  ".", ",");

		VariableObject* variableObject = addVariableObjectToList(index, selectedMode, variableName, regexInput0, regexInput1, regex, output);


		variableObject->isVaildValue = isDouble(output);
		if(isDouble(output)){
			printf("jest\n" );

			if(variableObject) variableObject->convertedValue = convertToDouble(output);

		}else{
			printf("nie jest\n");
			if(variableObject) variableObject->convertedValue = 0.0f;
		}
		
        g_free(taskContent);

        return JSValueMakeString(context, v);
    }
    
    return JSValueMakeUndefined(context);
}


/* Class method declarations */
static const JSStaticFunction DataExchangeStaticFunctions[] = {
    { "validateTask", valiidateTaskCb, kJSPropertyAttributeReadOnly },
    { "submitTask", submitTaskCb, kJSPropertyAttributeReadOnly },
    { "findVariable", findVariableCb, kJSPropertyAttributeReadOnly },
    { NULL, NULL, 0 }
};

static const JSClassDefinition DataExchangeDef = {
    0,                     // version
    kJSClassAttributeNone, // attributes
    "DataExchange",        // className
    NULL,                  // parentClass
    NULL,                  // staticValues
    DataExchangeStaticFunctions, // staticFunctions
    DataExchangeInitCb,  // initialize
    DataExchangeDestroyCd,   // finalize
    NULL,                  // hasProperty
    NULL,                  // getProperty
    NULL,                  // setProperty
    NULL,                  // deleteProperty
    NULL,                  // getPropertyNames
    NULL,                  // callAsFunction
    DataExchangeConstructorCb,                  // callAsConstructor
    NULL,                  // hasInstance  
    NULL                   // convertToType
};

/* Callback - JavaScript window object has been cleared */
static void window_object_cleared_cb(WebKitWebView *web_view, WebKitWebFrame *frame, gpointer context, gpointer window_object, gpointer user_data){
    /* Add classes to JavaScriptCore */
    JSClassRef classDef = JSClassCreate(&DataExchangeDef);
    JSObjectRef classObj = JSObjectMake(context, classDef, context);
    JSObjectRef globalObj = JSContextGetGlobalObject(context);
    JSStringRef str = JSStringCreateWithUTF8CString("DataExchange");
    JSObjectSetProperty(context, globalObj, str, classObj, kJSPropertyAttributeNone, NULL);
}

/* Destroy callback */
static void destroy(GtkWidget *widget, gpointer data ){

	//g_mem_profile ();

	gtk_main_quit();
}	


void initAddTaskWindow(int argc, char* argv[]){
    /* Initialize the widget set */
    gtk_init (&argc, &argv);
    
    /* Create the window widgets */
    GtkWidget *main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    
    /* Create the WebKit Web View widget */
    GtkWidget *web_view = webkit_web_view_new ();
    
    /* Connect the window object cleared event with callback */
    g_signal_connect (G_OBJECT (web_view), "window-object-cleared", G_CALLBACK(window_object_cleared_cb), web_view);


    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
    gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);

    /* Connect the destroy window event with destroy function */
    g_signal_connect (G_OBJECT (main_window), "destroy", G_CALLBACK (destroy), NULL);
    

    WebKitWebSettings *s = webkit_web_settings_new();
    g_object_set(G_OBJECT(s),"enable-file-access-from-file-uris", TRUE, NULL);
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(web_view),s);


    /* Open webpage */
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "file:///home/amadeusz/Desktop/form/default/blank.html");

    /* Create the main window */
    gtk_window_set_default_size (GTK_WINDOW (main_window), 575, 900);
    
    /* Show the application window */
    gtk_widget_show_all (main_window);

    /* Enter the main event loop, and wait for user interaction */
    gtk_main ();
}

static void redrawPrompt(void)
{
    static int  numDots;
    const  int  maxDots = 20;
    const  char prompt[] = "Loading";

    // Return and clear with spaces, then return and print prompt.
    printf("\r%*s\r%s", sizeof(prompt) - 1 + maxDots, "", prompt);
    for (int i = 0; i < numDots; i++)
        fputc('.', stdout);
    fflush(stdout);
    if (++numDots > maxDots)
        numDots = 0;
}

char* getValueFromJson(cJSON *json, char* str){
        const cJSON *jsonValue = NULL;
        jsonValue = cJSON_GetObjectItemCaseSensitive(json, str);
        if (cJSON_IsString(jsonValue) && jsonValue->valuestring != NULL){
            return strdup(jsonValue->valuestring);
        }else{
            return strdup("");
        }
}

void freeTask(){
    g_free(task->content);
    g_free(task->contentSlug);
    g_free(task->vriablesRegistered);
    g_free(task->formula);
    g_free(task->unit);
    g_free(task->additionalInformation);
}


void solveTask(){
    printf("\n\nSOLVING TASK\n");
    char* taskContnet = NULL;
    size_t size;
    while(getchar() != '\n');
    if (!(getline(&taskContnet, &size, stdin) == -1)) {
        //printf("%s", taskContnet);
    }

    taskContnet = replace(taskContnet,  "\n", "");

    cJSON *taskContetJson = cJSON_CreateObject();
    char* contentSlug = getContentSlug(taskContnet);

    cJSON_AddStringToObject(taskContetJson, "content", taskContnet);
    cJSON_AddStringToObject(taskContetJson, "contentSlug",contentSlug);

    //g_print("%s\n", cJSON_Print(taskContetJson));



    g_print("Sending api request... Please wait...");
    char* apiResponse = getTask(taskContetJson);


    cJSON *apiResponseJson = cJSON_Parse(apiResponse);
    if (apiResponseJson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }


    const cJSON *status = NULL;
    status = cJSON_GetObjectItemCaseSensitive(apiResponseJson, "status");

    //g_print("%s\n", status->valuestring);
    

    if (cJSON_IsString(status) && (status->valuestring != NULL)){
        //printf("Status \"%s\"\n", status->valuestring);

        task = (Task *)g_malloc(sizeof(Task));

        // const cJSON *content = NULL;
        // content = cJSON_GetObjectItemCaseSensitive(apiResponseJson, "content");
        // if (!(cJSON_IsString(content) && content->valuestring != NULL))


        char* id = getValueFromJson(apiResponseJson, "id");
        task->content = taskContnet;
        task->contentSlug = getValueFromJson(apiResponseJson, "contentSlug");
        task->vriablesRegistered = getValueFromJson(apiResponseJson, "vriablesRegistered");        
        task->formula = getValueFromJson(apiResponseJson, "formula");
        task->unit = getValueFromJson(apiResponseJson, "unit");
        task->additionalInformation = getValueFromJson(apiResponseJson, "additionalInformation");



        char* resultValueChar = getValueFromJson(apiResponseJson, "resultValue");
        double resultValue;
        sscanf(resultValueChar, "%lf", &resultValue);


        task->resultValue = resultValue;
        

        const cJSON *variables = NULL;
        cJSON *variable = NULL;
        variables = cJSON_GetObjectItemCaseSensitive(apiResponseJson, "variables");
        cJSON_ArrayForEach(variable, variables){

            char *indexInTaskChar = getValueFromJson(variable, "indexInTask");
            int indexInTask;
            sscanf(indexInTaskChar, "%d", &indexInTask);

            char *identificationMethodChar = getValueFromJson(variable, "identificationMethod");
            int identificationMethod;
            sscanf(identificationMethodChar, "%d", &identificationMethod);

            char *variableName = getValueFromJson(variable, "variableName");
            char *regexInput0 = getValueFromJson(variable, "regexInput0");
            char *regexInput1 = getValueFromJson(variable, "regexInput1");
            char *generatedRegex = getValueFromJson(variable, "generatedRegex");
            char *valueFound = getValueFromJson(variable, "valueFound");


            char*  output = getVaraibleFormText(generatedRegex, task->content);
        

            output = replace(output,  " ", "");
            output = replace(output,  "\n", "");
            output = replace(output,  ",", ".");

            VariableObject* variableObject = addVariableObjectToList(indexInTask, identificationMethod, variableName, regexInput0, regexInput1, generatedRegex, output);

            variableObject->isVaildValue = isDouble(output);
            if(isDouble(output)){
                if(variableObject) variableObject->convertedValue = convertToDouble(output);
            }else{
                if(variableObject) variableObject->convertedValue = 0.0f;
            }

        }
        //printAllVariableObjects(variableListHead);

        CalculationResult* calculationResult = calculateTask(variableListHead, task->formula);

        if(calculationResult->status == CALCULATED_CORRECTLY){

            task->resultValue = calculationResult->result;
            printf("WYNIK:%f %s", task->resultValue, task->unit);
         }
        g_free(calculationResult);
        freeAllObjects(variableListHead);
        variableListHead = NULL;

        g_free(id);

    }

    g_free(apiResponse);
    g_free(taskContnet);
    g_free(contentSlug);

    //printf("\ngowno434");
    cJSON_Delete(taskContetJson);

    error:

    end:
        cJSON_Delete(apiResponseJson);

}
 
main(int argc, char* argv[]){
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
  }
  return 0;
}
