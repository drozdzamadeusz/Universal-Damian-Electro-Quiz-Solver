#include "MathEquationsParser.h"


bool isDouble(const char *str)
{
    int len;
    double doubleValue = 0.0;
    if (sscanf(str, "%lf %n", &doubleValue, &len) == 1 && len == (int)strlen(str)){
    	printf("%lf\n", doubleValue);
        return true;
    }
    return false;
}

double convertToDouble(const char *str)
{
    int len;
    double doubleValue = 0.0;
    if (sscanf(str, "%lf %n", &doubleValue, &len) == 1 && len == (int)strlen(str))
        return doubleValue;
    else
        return 0;
}