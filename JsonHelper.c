#include "JsonHelper.h"


char* getValueFromJson(cJSON *json, char* str){
        const cJSON *jsonValue = NULL;
        jsonValue = cJSON_GetObjectItemCaseSensitive(json, str);
        if (cJSON_IsString(jsonValue) && jsonValue->valuestring != NULL){
            return strdup(jsonValue->valuestring);
        }else{
            return strdup("");
        }
}
