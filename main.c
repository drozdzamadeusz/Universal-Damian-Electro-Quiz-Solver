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


#include "tinyexpr.h"

#include "TaskVariable.h"
#include "TextProcessing.h"
#include "MathEquationsParser.h"
#include "ApiService.h"
#include "Memd/mem.h"


/* Class initialize */

typedef struct Task{
    char* content;
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


        VariableObject* currentObj = varibleListHead;

        if(!currentObj || taskVriablesRegistered == "" || taskFormula == "") return JSValueMakeUndefined(context);


        task->content = taskContent;
        task->vriablesRegistered = taskVriablesRegistered;
        task->formula = taskFormula;
        task->unit = taskUnit;
        task->additionalInformation = taskAdditionalInformation;

        size_t formulaSize = strlen(taskFormula)+10;
        short int numberOfVaiables = 0;
        while(currentObj){
            if(!currentObj -> isVaildValue) return JSValueMakeUndefined(context);
            formulaSize += 10;
            numberOfVaiables++;
            currentObj = currentObj->nextVariable;  
        }

        currentObj = varibleListHead;

        te_variable vars[numberOfVaiables];

        for(int i=0;currentObj;i++){
            te_variable uwu = {currentObj->variableName, &(currentObj->convertedValue)};
            vars[i] = uwu;

            if(currentObj->nextVariable)
                currentObj = currentObj->nextVariable; 
            else
                break;
        }

        g_print("JAVASCRIP KURWO: taskFormula: %svariableName: %s valueFound: %s",taskFormula, currentObj->variableName, currentObj->valueFound);


        g_print("GANG ALBANII: %f", currentObj->convertedValue);


        int err;
        te_expr *expr = te_compile(taskFormula, vars, numberOfVaiables, &err);


        if (expr) {
            double r = te_eval(expr);
            task->resultValue = r;
            printf("KALKULATORED:%f\n", r);

            char* result = g_malloc(sizeof(r)+1000);
            sprintf(result,"%f",r);

            JSStringRef v = JSStringCreateWithUTF8CString(result);


            g_print("%s\n", cJSON_Print(variablesObjectsToJsonArray(varibleListHead)));

            g_free(result);
            te_free(expr);

            return JSValueMakeString(context, v);
        
        }else{
            printf("Parse error at %d\n", err);
        }
   // }
    return JSValueMakeUndefined(context);
}



static JSValueRef submitTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){

    cJSON* json = variablesObjectsToJsonArray(varibleListHead);

    
   // memcpy(resultValue,&(task->resultValue),sizeof(task->resultValue));


    //char resultValue[sizeof(task->resultValue)];
    //sprintf(resultValue, "%lf", task->resultValue);

    cJSON_AddStringToObject(json, "content", task->content);
    cJSON_AddStringToObject(json, "vriablesRegistered", task->vriablesRegistered);
    cJSON_AddStringToObject(json, "formula", task->formula);
    cJSON_AddStringToObject(json, "unit", task->unit);
    cJSON_AddStringToObject(json, "additionalInformation", task->additionalInformation);
    //cJSON_AddStringToObject(json, "resultValue", resultValue);
    

    g_print("%s\n", cJSON_Print(json));
    

    sendTaskToApi();

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

		VariableObject* VariableObject = addVariableObjectToList(index, selectedMode, variableName, regexInput0, regexInput1, regex, output);


		VariableObject->isVaildValue = isDouble(output);
		if(isDouble(output)){
			printf("jest\n" );

			if(VariableObject) VariableObject->convertedValue = convertToDouble(output);

		}else{
			printf("nie jest\n");
			if(VariableObject) VariableObject->convertedValue = 0.0f;
		}
		
        g_free(taskContent);

		/*
        g_free(variableName);
        g_free(regexInput0);
        g_free(regexInput1);
        g_free(regex);
        g_free(output);*/

		//printf("AAA:%s\n", varibleListHead->variableName);


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
    


int main (int argc, char* argv[]){

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
    
    /* The user lost interest */
    return 0;
}
