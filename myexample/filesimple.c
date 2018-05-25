#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include <string.h>

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

void jsonNameList(char *jsonstr,jsmntok_t *t, int tokcount,int *nameTokIndex);
void printNameList(char *jsonstr,jsmntok_t *t,int * nameTokIndex);
char *readJSONFile() {

	FILE *fp =fopen("data.json","r");

	char * json_string=(char*)malloc(sizeof(char));

  char str[255];

	 if(fp==NULL){
			 printf("파일 오픈 실패 !\n");
	  	}

		int len = 0;
		while(1){
			fgets(str, 255, fp);
			len += strlen(str);
			if(feof(fp)) break;
			json_string=(char*)realloc(json_string, len+1);
			strcat(json_string, str);
		}

   fclose(fp);
	 return json_string;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}




void selectNameList(char *jsonstr,jsmntok_t *t, int *nameTokIndex)
{
	  int index;
    while(1){
			printf("Select Name's no(exit:0)>>");
			scanf("%d",&index);
			if(index==0) break;
			printf("[NAME%d]%.*s\n",index,t[(nameTokIndex[index-1])].end-t[(nameTokIndex[index-1])].start,
				 jsonstr + t[(nameTokIndex[index-1])].start);

     int ptr=nameTokIndex[index-1]+1;
     if(t[(nameTokIndex[index-1])+1].size>0){
		   ptr++;
			 while(1){
				 printf("%.*s\n",t[ptr].end-t[ptr].start,
	 						jsonstr + t[ptr].start);
				 if(t[ptr+1].size>0)break;
				 ptr++;
			 }
		 }
		 else{
			 printf("%.*s\n",t[(nameTokIndex[index-1])+1].end-t[(nameTokIndex[index-1])+1].start,
						 jsonstr + t[(nameTokIndex[index-1])+1].start);
		 }
		}


}

void jsonNameList(char *jsonstr,jsmntok_t *t, int tokcount,int *nameTokIndex)
{

	int count=0,i;

	for (i=1;i<tokcount;i++){
		 if(t[i].type==3&&t[i].size>0){
			 count++;
			 nameTokIndex=(int*)realloc(nameTokIndex,(count)*sizeof(int));
			 nameTokIndex[count-1]=i;
		 //	printf("[NAME%d]%.*s\n",count,t[i].end-t[i].start,jsonstr + t[i].start);
		//	printf("%d ", nameTokIndex[0]);
		 }
	}
	nameTokIndex[count]=-1;
}

void printNameList(char *jsonstr,jsmntok_t *t,int *nameTokIndex)
{
			int i=0;
			for(i=0;nameTokIndex[i]!=-1;i++){
				printf("[NAME%d]%.*s\n",i+1,t[(nameTokIndex[i])].end-t[(nameTokIndex[i])].start,
					 jsonstr + t[(nameTokIndex[i])].start);
			  printf("",t[(nameTokIndex[i])].end-t[(nameTokIndex[i])].start,
 						 jsonstr + t[(nameTokIndex[i])].start);
			//printf("%d ", *(nameTokIndex+1));
		}
}

void objectList(char *jsonstr,jsmntok_t *t,int* nameTokIndex,int t_size)
{
    int i=0,count=0;
		char name[10];
		strncpy(name,jsonstr + t[nameTokIndex[0]].start,t[nameTokIndex[0]].end-t[nameTokIndex[0]].start);
		char value[255];

		 for(i=0;i<t_size;i++){
		 	if(t[i].type==JSMN_OBJECT&&t[i+1].type==JSMN_STRING){
				//strncpy(value,jsonstr + t[nameTokIndex[0]].start,t[nameTokIndex[0]].end-t[nameTokIndex[0]].start);
				strncpy(value,jsonstr + t[i+1].start,t[i+1].end-t[i+1].start);
		 		if(strcmp(name,value)==0){
					count++;
		 			printf("[NAME%d]%.*s\n",count,t[i+2].end-t[i+2].start,
		 				 jsonstr + t[i+2].start);
		 		}
		 	}
		 }
}

int main() {
	int i,j;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	static char *JSON_STRING;

  JSON_STRING=readJSONFile();
  //printf("%s",JSON_STRING);

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1) {
		printf("Object expected\n");
		return 1;
	}


  int* nameTokIndex;
  nameTokIndex=(int*)malloc(sizeof(int));
  jsonNameList(JSON_STRING,t,r,nameTokIndex);
  printNameList(JSON_STRING,t,nameTokIndex);
  selectNameList(JSON_STRING,t,nameTokIndex);
  objectList(JSON_STRING,t,nameTokIndex,sizeof(t)/sizeof(t[0]));

	// /* Loop over all keys of the root object */
	// for (i = 1; i < r; i++) {
	// 	if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
	// 		/* We may use strndup() to fetch string value */
	// 		printf("-name: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
	// 		/* We may additionally check if the value is either "true" or "false" */
	// 		printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
	// 		/* We may want to do strtol() here to get numeric value */
	// 		printf("- description: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	} else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {
	// 		/* We may want to do strtol() here to get numeric value */
	// 		printf("- examples: %.*s\n", t[i+1].end-t[i+1].start,
	// 				JSON_STRING + t[i+1].start);
	// 		i++;
	// 	}else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
	// 		int j;
	// 		printf("- Groups:\n");
	// 		if (t[i+1].type != JSMN_ARRAY) {
	// 			continue; /* We expect groups to be an array of strings */
	// 		}
	// 		for (j = 0; j < t[i+1].size; j++) {
	// 			jsmntok_t *g = &t[i+j+2];
	// 			printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
	// 		}
	// 		i += t[i+1].size + 1;
	// 	} else {
	// 		//printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
	// 			//	JSON_STRING + t[i].start);
	// 	}
	// }
	return EXIT_SUCCESS;
}
