#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include <string.h>
#include "myproduct.h"
/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */


char *readJSONFile() {

  char filename[30];
 printf("원하는 파일 명 입력:");
  scanf("%s",filename);
  strcat(filename,".json");
	FILE *fp =fopen(filename,"r");

	char * json_string=(char*)malloc(sizeof(char));

  char str[255];

	 if(fp==NULL){
			 printf("%s 파일이 존재하지 않습니다\n",filename);
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


void printProduct(product_t * p[], int pcount)
{
  printf("*********************************************************\n");
  printf("%10s %10s %10s %10s %10s \n","번호","제품명","제조사","가격","개수");
  printf("*********************************************************\n");

  char * company;

  int i=0;
  for(i=0;i<pcount;i++){
        if(p[i]->company == 0 ) company="오리온";
        else if(p[i]->company == 1 ) company="롯데";
        else if(p[i]->company == 2 ) company="농심";
        else if(p[i]->company == 3 ) company="크라운";

        printf("%7d %11s  %7s  %7d %6d\n",i+1,p[i]->name,company,p[i]->price,p[i]->count);
  }

}

int makeProduct(const char * jsonstr, jsmntok_t *t, int tokcount, product_t * p[])
{
  int i=0;
  char name[20]="";
  char company[30]="";
  company_t com;
  int price;
  int total;

  int count=0,child;
  child=0;

  int objectindex=0;
  int nameindex;

	for (i=0;i<tokcount;i++){
    if(t[i].type==JSMN_OBJECT&&t[i+1].type==JSMN_STRING&&child==0){
      objectindex++;
      p[objectindex-1]=(product_t *)malloc(sizeof(product_t));
    }
		if(t[i].type==JSMN_STRING&&t[i].size>0&&child==0){
			 child=t[i+1].size;
			 count++;

       char value[20]="";
       strncpy(value,jsonstr+t[i].start,t[i].end-t[i].start);
       value[t[i].end-t[i].start]='\0';
       if(strcmp(value,"name")==0){
         strncpy(name,jsonstr+t[i+1].start,t[i+1].end-t[i+1].start);
         name[t[i+1].end-t[i+1].start]='\0';
       }
       else if(strcmp(value,"company")==0){
         strncpy(company,jsonstr+t[i+1].start,t[i+1].end-t[i+1].start);
         if(strcmp(company,"오리온")==0) com=ORION;
         else if(strcmp(company,"롯데")==0) com=LOTTE;
         else if(strcmp(company,"농심")==0) com=NONGSIM;
         else if(strcmp(company,"크라운")==0) com=CROWN;
       else if(strcmp(value,"price")==0){
       }
         char temp[30]="";
         strncpy(temp,jsonstr+t[i+1].start,t[i+1].end-t[i+1].start);
         temp[t[i+1].end-t[i+1].start]='\0';
         price=atoi(temp);
       }
       else if(strcmp(value,"count")==0){
         char temp[30]="";
         strncpy(temp,jsonstr+t[i+1].start,t[i+1].end-t[i+1].start);
         temp[t[i+1].end-t[i+1].start]='\0';
         count=atoi(temp);

         p[objectindex-1]->company=com;
         strcpy( p[objectindex-1]->name,name);
         p[objectindex-1]->price=price;
         p[objectindex-1]->count=count;
       }
		}
		 else if(child!=0){
       child+=t[i+1].size;
			 child--;
		 }
	}

  return objectindex;
}

int main() {
	int i,j;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	static char *JSON_STRING;

  JSON_STRING=readJSONFile();


	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object or array */
	if (r < 1) {
		printf("Object expected\n");
		return 1;
	}

  int pcount;

  pcount=makeProduct(JSON_STRING,t,r,snacklist);
  printProduct(snacklist,pcount);

	return EXIT_SUCCESS;
}
