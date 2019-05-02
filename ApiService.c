
#include "ApiService.h"


void sendTaskToApi(cJSON* task){
    char *url;
    char data[1024], response[4096];
    int  i, ret, size;

    HTTP_INFO hi2;


    // Init http session. verify: check the server CA cert.
    http_init(&hi2, TRUE);

    //cJSON* json = cJSON_CreateObject();
   // cJSON_AddStringToObject(json, "task", cJSON_Print(task));

    //g_print("%s\n", cJSON_Print(json));

    url = "https://e146.eu/api/push-task/";
    sprintf(data, cJSON_Print(task));

    ret = http_post(&hi2, url, data, response, sizeof(response));

    printf("return code: %d \n", ret);
    printf("return body: %s \n", response);


error:

    http_close(&hi2);

}
