
#include "ApiService.h"


void sendTaskToApi(cJSON* task){

    char *url;
    char data[65536], response[16384];
    int  i, ret, size;

    HTTP_INFO hi1, hi2;


    // Init http session. verify: check the server CA cert.
    http_init(&hi1, FALSE);
    http_init(&hi2, TRUE);


    //cJSON* json = cJSON_CreateObject();
   // cJSON_AddStringToObject(json, "task", cJSON_Print(task));

    //g_print("%s\n", cJSON_Print(json));

    url = "https://e146.eu/api/push-task/";

    sprintf(data, cJSON_Print(task));


    ret = http_post(&hi2, url, data, response, sizeof(response));

    printf("return code: %d \n", ret);
    printf("return body: %s \n", response);





/*
    if(http_open(&hi2, url) == 0)
    {
        size = sprintf(data, "{\"message\":\"Hello, https_client!\"}");

        if(http_write(&hi2, data, size) != size)
        {
            http_strerror(data, 1024);
            printf("socket error: %s \n", data);

            goto error;
        }

        ret = http_read_chunked(&hi2, response, sizeof(response));

        printf("return code: %d \n", ret);
        printf("return body: %s \n", response);

    }
    else
    {
        http_strerror(data, 1024);
        printf("socket error: %s \n", data);
    }

*/





    /*char *url;
    char data[16384], response[16384];
    int  i, ret, size;

    HTTP_INFO hi2;


    // Init http session. verify: check the server CA cert.
    http_init(&hi2, TRUE);

    //cJSON* json = cJSON_CreateObject();
   // cJSON_AddStringToObject(json, "task", cJSON_Print(task));

    //g_print("%s\n", cJSON_Print(json));

    url = "https://e146.eu/api/push-task/";
    //strcpy(data, cJSON_Print(task));

    ret = http_post(&hi2, url, cJSON_Print(task), response, sizeof(response));

    printf("return code: %d \n", ret);
    printf("return body: %s \n", response);
*/

error:
    http_close(&hi2);
}


char* getTask(cJSON* task){
    char *url;
    char data[1024], *response;
    int  i, ret, size;

    response = g_malloc(4096);

    HTTP_INFO hi2;


    // Init http session. verify: check the server CA cert.
    http_init(&hi2, TRUE);

    url = "https://e146.eu/api/get-task/";
    sprintf(data, cJSON_Print(task));

    ret = http_post(&hi2, url, data, response, 4096);

    //printf("return code: %d \n", ret);
    printf("return body: %s \n\n", response);

    http_close(&hi2);
    return response;
}
