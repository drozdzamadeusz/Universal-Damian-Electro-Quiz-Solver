#ifndef ADD_TASK_WINDOW_H
#define ADD_TASK_WINDOW_H

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
#include <unistd.h>

#include "tinyexpr.h"

#include "Task.h"
#include "TaskVariable.h"
#include "TextProcessing.h"
#include "MathEquationsParser.h"
#include "ApiService.h"
#include "preg_replace.h"
#include "JsonHelper.h"
#include "AddTaskWindow.h"


#define WEB_SOURCE_DIRECTORY "/forntend_forms/default/"
#define WEB_SOURCE_NAME "add_task_form.html"


/* Get string from JS value */
char* getStringValue(JSContextRef context, const JSValueRef jsValue);

/* Class initialize */
static void DataExchangeInitCb(JSContextRef ctx, JSObjectRef object);

/* Class constructor */
static JSObjectRef DataExchangeConstructorCb(JSContextRef context, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception);

/* Class finalize */
static void DataExchangeDestroyCd(JSObjectRef object);

/* DataExchange.validateTask method callback implementation */
static JSValueRef validateTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception);

/* DataExchange.submitTask method callback implementation */
static JSValueRef submitTaskCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception);

/* DataExchange.findVariable method callback implementation */
static JSValueRef findVariableCb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception);

/* Callback - JavaScript window object has been cleared */
static void JsWindowObjectClearedCb(WebKitWebView *webView, WebKitWebFrame *frame, gpointer context, gpointer windowObject, gpointer userData);

/* Destroy callback */
static void destroy(GtkWidget *widget, gpointer data);

/* Initiate the task adding window */
void initAddTaskWindow(int argc, char* argv[]);


/* Class method declarations */
static const JSStaticFunction DataExchangeStaticFunctions[] = {
    { "validateTask", validateTaskCb, kJSPropertyAttributeReadOnly },
    { "submitTask", submitTaskCb, kJSPropertyAttributeReadOnly },
    { "findVariable", findVariableCb, kJSPropertyAttributeReadOnly },
    { NULL, NULL, 0 }
};

static const JSClassDefinition DataExchangeDef = {
    0,                              // version
    kJSClassAttributeNone,          // attributes
    "DataExchange",                 // className
    NULL,                           // parentClass
    NULL,                           // staticValues
    DataExchangeStaticFunctions,    // staticFunctions
    DataExchangeInitCb,             // initialize
    DataExchangeDestroyCd,          // finalize
    NULL,                           // hasProperty
    NULL,                           // getProperty
    NULL,                           // setProperty
    NULL,                           // deleteProperty
    NULL,                           // getPropertyNames
    NULL,                           // callAsFunction
    DataExchangeConstructorCb,      // callAsConstructor
    NULL,                           // hasInstance
    NULL                            // convertToType
};

#endif