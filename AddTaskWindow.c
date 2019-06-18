#include "AddTaskWindow.h"


char* getStringValue(JSContextRef context, const JSValueRef jsValue){
    JSStringRef valueJSString = JSValueToStringCopy(context, jsValue, NULL);
    char* value = g_new(char, JSStringGetMaximumUTF8CStringSize(valueJSString));
    JSStringGetUTF8CString(valueJSString, value, JSStringGetMaximumUTF8CStringSize(valueJSString));

	JSStringRelease(valueJSString);

	return value;
}


/* Class initialize */
static void DataExchangeInitCb(JSContextRef ctx, JSObjectRef object){
/* Inits DataExchange */
    initTask();
}


/* Class constructor */
static JSObjectRef DataExchangeConstructorCb(JSContextRef context, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){
    return constructor;
}


/* Class finalize */
static void DataExchangeDestroyCd(JSObjectRef object){
}


/* DataExchange.getSubmittedTaskCb method callback implementation */
static JSValueRef validateTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){
    /* At least, one argument must be received */
    //if (argumentCount == 1 && JSValueIsString(context, arguments[0])) {
        /* Converts JSValue to char */
    if (argumentCount == 5) {

        char* taskContent = getStringValue(context, arguments[0]);
        char* taskVriablesRegistered = getStringValue(context, arguments[1]);
        char* taskFormula = getStringValue(context, arguments[2]);
        char* taskUnit = getStringValue(context, arguments[3]);
        char* taskAdditionalInformation = getStringValue(context, arguments[4]);


        if(!variableListHead || taskVriablesRegistered == "" || taskFormula == "") return JSValueMakeUndefined(context);


        task->content = taskContent;
        task->vriablesRegistered = taskVriablesRegistered;
        task->formula = taskFormula;
        task->unit = taskUnit;
        task->additionalInformation = taskAdditionalInformation;



        CalculationResult* calculationResult = calculateTask(variableListHead, taskFormula);


        if(calculationResult->status == CALCULATED_CORRECTLY){

            task->resultValue = calculationResult->result;

            char* result = g_malloc(sizeof(task->resultValue)+1000);
            sprintf(result,"%f",task->resultValue);

            JSStringRef v = JSStringCreateWithUTF8CString(result);

            //g_print("%s\n", cJSON_Print(variablesObjectsToJsonArray(variableListHead)));

            g_free(result);
            g_free(calculationResult);
            return JSValueMakeString(context, v);
        }
        g_free(calculationResult);
    }
    return JSValueMakeUndefined(context);
}



static JSValueRef submitTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception){


    if(!(variableListHead)) return JSValueMakeUndefined(context);


    cJSON* json = taskToJson();

    sendTaskToApi(json);

    cJSON_Delete(json);

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
			if(variableObject){
                variableObject->convertedValue = convertToDouble(output);
            }
		}else{
			if(variableObject) variableObject->convertedValue = 0.0f;
		}

        g_free(taskContent);

        return JSValueMakeString(context, v);
    }
    
    return JSValueMakeUndefined(context);
}

/* Callback - JavaScript window object has been cleared */
static void JsWindowObjectClearedCb(WebKitWebView *webView, WebKitWebFrame *frame, gpointer context, gpointer windowObject, gpointer userData){
    /* Add classes to JavaScriptCore */
    JSClassRef classDef = JSClassCreate(&DataExchangeDef);
    JSObjectRef classObj = JSObjectMake(context, classDef, context);
    JSObjectRef globalObj = JSContextGetGlobalObject(context);
    JSStringRef str = JSStringCreateWithUTF8CString("DataExchange");
    JSObjectSetProperty(context, globalObj, str, classObj, kJSPropertyAttributeNone, NULL);
}

/* Destroy callback */
static void destroy(GtkWidget *widget, gpointer data){
	gtk_main_quit();
    freeTask();
}	


void initAddTaskWindow(int argc, char* argv[]){
    /* Initialize the widget set */
    gtk_init (&argc, &argv);
    
    /* Create the window widgets */
    GtkWidget *mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    
    /* Create the WebKit Web View widget */
    GtkWidget *webView = webkit_web_view_new ();
    
    /* Connect the window object cleared event with callback */
    g_signal_connect (G_OBJECT (webView), "window-object-cleared", G_CALLBACK(JsWindowObjectClearedCb), webView);


    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER_ALWAYS);

    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add (GTK_CONTAINER (scrolled_window), webView);
    gtk_container_add (GTK_CONTAINER (mainWindow), scrolled_window);

    /* Connect the destroy window event with destroy function */
    g_signal_connect (G_OBJECT (mainWindow), "destroy", G_CALLBACK (destroy), NULL);
    
	/* Allow access to local resources */
    WebKitWebSettings *webViewSettings = webkit_web_settings_new();
    g_object_set(G_OBJECT(webViewSettings),"enable-file-access-from-file-uris", TRUE, NULL);
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(webView),webViewSettings);


    gtk_window_set_title(mainWindow, "Add a task");

    char programLocation[200], webSourceLocation[300];
    getcwd(programLocation, 200);
    snprintf(webSourceLocation, 300, "file://%s%s%s", programLocation, WEB_SOURCE_DIRECTORY, WEB_SOURCE_NAME);

    /* Open webpage */
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (webView), webSourceLocation);

    /* Create the main window */
    gtk_window_set_default_size (GTK_WINDOW (mainWindow), 575, 900);
    
    /* Show the application window */
    gtk_widget_show_all (mainWindow);

    /* Enter the main event loop, and wait for user interaction */
    gtk_main();
}