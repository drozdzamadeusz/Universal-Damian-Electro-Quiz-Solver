#include "TextProcessing.h"

char* getContentSlug(char* content){
	char* contentSlug =  preg_replace("[^a-zA-Z]", "", content);
    toUpperCase(contentSlug);
    return contentSlug;
}

void toUpperCase(char* str) {
   int c = 0;
   
   while (str[c] != '\0') {
      if (str[c] >= 'a' && str[c] <= 'z') {
         str[c] = str[c] - 32;
      }
      c++;
   }
}
char* replace(char* str, char* a, char* b){
    int len  = strlen(str);
    int lena = strlen(a), lenb = strlen(b);
    for (char* p = str; p = strstr(p, a); ++p) {
        if (lena != lenb) // shift end as needed
            memmove(p+lenb, p+lena,
                len - (p - str) + lenb);
        memcpy(p, b, lenb);
    }
    return str;
}



char* getVaraibleFormText(char* RegexStr, char* source){
	bool Debug = true;
	bool Found = false;
	pcre2_code *re;
	PCRE2_SPTR pattern;
	PCRE2_SPTR subject;
	int errornumber;
	int i;
	int rc;
	PCRE2_SIZE erroroffset;
	PCRE2_SIZE *ovector;
	size_t subject_length;
	pcre2_match_data *match_data;

	pattern = (PCRE2_SPTR)RegexStr;// <<<<< This is where you pass your REGEX 
	subject = (PCRE2_SPTR)source;// <<<<< This is where you pass your bufer that will be checked. 
	subject_length = strlen((char *)subject);


	//g_print("\n%s", RegexStr);

	//char *result = NULL;
	char *result = strdup("");
	//char *result = g_malloc(subject_length);
	//result = "";

	  re = pcre2_compile(
	  pattern,               /* the pattern */
	  PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
	  0,                     /* default options */
	  &errornumber,          /* for error number */
	  &erroroffset,          /* for error offset */
	  NULL);                 /* use default compile context */

	/* Compilation failed: print the error message and exit. */
	if (re == NULL)
	  {
	  PCRE2_UCHAR buffer[256];
	  //pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
	 // printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,buffer);
	  return result;
	  }


	match_data = pcre2_match_data_create_from_pattern(re, NULL);

	rc = pcre2_match(
	  re,
	  subject,              /* the subject string */
	  subject_length,       /* the length of the subject */
	  0,                    /* start at offset 0 in the subject */
	  0,                    /* default options */
	  match_data,           /* block for storing the result */
	  NULL);

	if (rc < 0)
	  {
	  switch(rc)
	    {
	    case PCRE2_ERROR_NOMATCH: //printf("No match\n"); //
	    pcre2_match_data_free(match_data);
	    pcre2_code_free(re);
	    Found = 0;
	    return result;
	    //  break;
	    /*
	    Handle other special cases if you like
	    */
	    default: printf("Matching error %d\n", rc); //break;
	    }
	  pcre2_match_data_free(match_data);   /* Release memory used for the match */
	  pcre2_code_free(re);
	  Found = 0;                /* data and the compiled pattern. */
	  return result;
	  }



	PCRE2_SPTR substring_start;
	if (Debug){
	ovector = pcre2_get_ovector_pointer(match_data);
	//printf("Match succeeded at offset %d\n", (int)ovector[0]);

	if (rc == 0)
	  printf("ovector was not big enough for all the captured substrings\n");


	if (ovector[0] > ovector[1])
	  {
	  printf("\\K was used in an assertion to set the match start after its end.\n From end to start the match was: %.*s\n", (int)(ovector[0] - ovector[1]), (char *)(subject + ovector[1]));
	  printf("Run abandoned\n");
	  pcre2_match_data_free(match_data);
	  pcre2_code_free(re);
	  return result;
	}

	  substring_start = subject + ovector[0];
		size_t substring_length = ovector[1] - ovector[0];


		free(result);
        result = g_malloc((int)substring_length*2+10);
		sprintf(result,"%.*s\n",(int)substring_length, (char *)substring_start);


		pcre2_match_data_free(match_data);
		pcre2_code_free(re);


		return result;
	}

	else{
	  if(rc > 0){
	    Found = true;

	    } 
	}
	return result;
}

