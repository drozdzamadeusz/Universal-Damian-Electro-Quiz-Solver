#include "MathEquationsParser.h"


bool isDouble(const char *str)
{
    int len;
    double doubleValue = 0.0;
    if (sscanf(str, "%lf %n", &doubleValue, &len) == 1 && len == (int)strlen(str)){
    	//printf("%lf\n", doubleValue);
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

                g_print("\nVaraible match succeeded: %s - %.4lf", currentObj->variableName, currentObj->convertedValue);

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
                //printf("KALKULATORED:%f\n", r);

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