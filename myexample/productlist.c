#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include <string.h>
//#include "productlist.h"
/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 typedef struct {
   int tokindex;
   int objectindex;
 } NameTokenInfo;


NameTokenInfo * jsonNameList(char *jsonstr,jsmntok_t *t, int tokcount,NameTokenInfo *nameTokIndex);
void printNameList(char *jsonstr,jsmntok_t *t,NameTokenInfo  * nameTokIndex);
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

NameTokenInfo* jsonNameList(char *jsonstr,jsmntok_t *t, int tokcount,NameTokenInfo *nameTokenInfo)
{

	int count=0,i,child;
  child=0;

  int objectindex=0;
  int nameindex;

	for (i=0;i<tokcount;i++){
    if(t[i].type==JSMN_OBJECT&&t[i+1].type==JSMN_STRING&&child==0){
      objectindex++;
    }
		if(t[i].type==JSMN_STRING&&t[i].size>0&&child==0){
			 child=t[i+1].size;
			 count++;
			 nameTokenInfo=(NameTokenInfo*)realloc(nameTokenInfo,(count)*sizeof(NameTokenInfo));
       nameTokenInfo[count-1].objectindex=objectindex;
       nameTokenInfo[count-1].tokindex=i;
		}
		 else if(child!=0){
       child+=t[i+1].size;
			 child--;
		 }
	}
	nameTokenInfo=(NameTokenInfo*)realloc(nameTokenInfo,(count+1)*sizeof(NameTokenInfo));
	nameTokenInfo[count].tokindex=-1;

	return nameTokenInfo;
}

void printNameList(char *jsonstr,jsmntok_t *t,NameTokenInfo *nameTokenInfo)
{
			int i=0;
			printf("-------namelist---------\n");
		// 	for(i=0;nameTokIndex[i]!=-1;i++){
		// 		printf("인덱스:%d",i);
		// 		 printf("[%d NAME%d]%.*s\n",nameTokIndex[i],i+1,t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,
		// 		 	 jsonstr + t[(nameTokIndex[i])].start);
		//  // printf("[NAME%d]%.*s\n",i,t[[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[[nameTokIndex[i]].start);
		// }
		for(i=0;nameTokenInfo[i].tokindex!=-1;i++){
			 printf("%d [NAME%d]%.*s\n",nameTokenInfo[i].objectindex,i+1,t[(nameTokenInfo[i]).tokindex].end-t[(nameTokenInfo[i]).tokindex].start,
				 jsonstr + t[(nameTokenInfo[i]).tokindex].start);
		// printf("[NAME%d]%.*s\n",i,t[[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[[nameTokIndex[i]].start);
		}


}

int * objectList(char *jsonstr,jsmntok_t *t,int* nameTokIndex,int t_size,int* objectTokIndex)
{
    int i=0,count=0,child=0;
		char name[300];
		strncpy(name,jsonstr + t[nameTokIndex[0]].start,t[nameTokIndex[0]].end-t[nameTokIndex[0]].start); //맨처음 token 즉 이 예제에서는 name
    name[t[nameTokIndex[0]].end-t[nameTokIndex[0]].start]='\0';
    char value[300];
     printf("-------objectlist---------\n");
		  for(i=0;i<t_size;i++){
		   	if(t[i].type==JSMN_OBJECT&&t[i+1].type==JSMN_STRING&&child==0){
			 	strncpy(value,jsonstr + t[i+1].start,(t[i+1].end-t[i+1].start));
        value[t[i+1].end-t[i+1].start]='\0';
        //printf("%s %s",name,value);
			if(strcmp(name,value)==0){ // token value가 name과 같으면
					count++;
					 child=t[i].size;
					objectTokIndex=(int*)realloc( objectTokIndex,(count+1)*sizeof(int));
          objectTokIndex[count]=i;
					printf("[NAME%d]%.*s\n",count,t[i+2].end-t[i+2].start,
				 jsonstr + t[i+2].start);
		 		}
		 	}
			else if(child!=0){
				child+=t[i+1].size;
				child--;
			}
		 }
		 count++;
 	  objectTokIndex=(int*)realloc(objectTokIndex,(count+1)*sizeof(int));
    objectTokIndex[count]=t_size; //맨 마지막 표시
   return objectTokIndex;
}

void printObjectList(char *jsonstr,jsmntok_t *t,int* nameTokIndex,int t_size,int* objectTokIndex)
{
	int objnum=0,i=0,num=0,j=0,count=0;
	int same=0;
	char name[300];
	strncpy(name,jsonstr + t[nameTokIndex[0]].start,t[nameTokIndex[0]].end-t[nameTokIndex[0]].start);
  char value[300];
//	printf("?");
  //objectList(jsonstr,t,nameTokIndex,sizeof(t)/sizeof(t[0]));

 while(1){

	printf("원하는 번호 입력(Eixt:0)");
	count=0;
	scanf("%d",&num);
	 if(num==0) break;
  printf(" %.*s",t[objectTokIndex[num]+1].end-t[objectTokIndex[num]+1].start,jsonstr + t[objectTokIndex[num]+1].start);
  printf(" : %.*s\n",t[objectTokIndex[num]+2].end-t[objectTokIndex[num]+2].start,jsonstr + t[objectTokIndex[num]+2].start);
  int first=0;
 //
 //
	for(i=0;nameTokIndex[i]!=-1;i++){
		if(nameTokIndex[i]>objectTokIndex[num]&&nameTokIndex[i]<objectTokIndex[num+1]){
      if(first==0) {
				first=1;
				continue;
			}
			printf("     [%.*s] ",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,
					 jsonstr + t[nameTokIndex[i]].start);
		 		  printf("     %.*s\n",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start, jsonstr + t[nameTokIndex[i]+1].start);
		}
   }


 }

}

void printlst(char *jsonstr,jsmntok_t *t,int* nameTokIndex,int t_size,int* objectTokIndex)
{
  printf("*********************************************************\n");
  printf("%10s %10s %10s %10s %10s %10s\n","번호","제품명","제조사","가격","개수","총가격");
    printf("*********************************************************\n");
  int i=0,j=0,first=0;
  int num;
  char name[30]="";
  char company[30]="";
  int price;
  int count;
  int total;


  for(i=1;objectTokIndex[i]!=t_size;i++){
   num=i;
     //printf("hi");
    // first=0;
    for(j=0;nameTokIndex[j]!=-1;j++){
      if(nameTokIndex[j]>objectTokIndex[i]&&nameTokIndex[j]<objectTokIndex[i+1]){
        // if(first==0) {
        //   first=1;
        //   continue;
        // }
        char value[30]="";
        strncpy(value,jsonstr+t[nameTokIndex[j]].start,t[nameTokIndex[j]].end-t[nameTokIndex[j]].start);
        value[t[nameTokIndex[j]].end-t[nameTokIndex[j]].start]='\0';
        if(strcmp(value,"name")==0){
          strncpy(name,jsonstr+t[nameTokIndex[j]+1].start,t[nameTokIndex[j]+1].end-t[nameTokIndex[j]+1].start);
        }
        else if(strcmp(value,"company")==0){
          strncpy(company,jsonstr+t[nameTokIndex[j]+1].start,t[nameTokIndex[j]+1].end-t[nameTokIndex[j]+1].start);
        }
        else if(strcmp(value,"price")==0){
          char temp[30]="";
          strncpy(temp,jsonstr+t[nameTokIndex[j]+1].start,t[nameTokIndex[j]+1].end-t[nameTokIndex[j]+1].start);
          temp[t[nameTokIndex[j]+1].end-t[nameTokIndex[j]+1].start]='\0';
          price=atoi(temp);
        }
        else if(strcmp(value,"count")==0){
          char temp[30]="";
          strncpy(temp,jsonstr+t[nameTokIndex[j]+1].start,t[nameTokIndex[j]+1].end-t[nameTokIndex[j]+1].start);
          count=atoi(temp);
        }
      }
     }
        total=price*count;
        printf("%7d %11s  %7s  %7d %6d %9d\n",num,name,company,price,count,total);
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

	/* Assume the top-level element is an object or array */
	if (r < 1) {
		printf("Object expected\n");
		return 1;
	}


  int* nameTokIndex;
	int* objectTokIndex;
  NameTokenInfo *nameTokenInfo;

  nameTokIndex=(int*)malloc(sizeof(int));
  objectTokIndex=(int*)malloc(sizeof(int));

  nameTokenInfo=(NameTokenInfo*)malloc(sizeof(NameTokenInfo));
  nameTokenInfo=jsonNameList(JSON_STRING,t,r,nameTokenInfo);
  printNameList(JSON_STRING,t,nameTokenInfo);

	return EXIT_SUCCESS;
}
