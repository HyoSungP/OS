
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 4096
struct CourseEntry {
  char name[MAX_LENGTH]; // 과목의 이름
  struct CourseEntry* prerequisites; // 선수 과목들
  int n_prerequisites; // 선수 과목의 수
  float difficulty; // 과목의 난이도
};


int main(int argc, char** argv)
{
  char Sub_File[2000]; //파일 불러온것 저장하는 변수
  char *str; //Subs_File을 쪼갠 것을 저장하는 변수
  struct CourseEntry course_entry[10];//나중 소팅할때 배열
  struct CourseEntry Before_course[10][10];//선수강과목넣을 배열구조체
  FILE * db =fopen("./in/database.csv","r");
  FILE * et = fopen(argv[1],"r");
  //FILE * et = fopen("./in/everytime2.csv", "r");
  int i=0;
  int g=0;

  //database
  if(db == NULL){
    puts("FILE OPEN FAIL!");
    return -1;
  }
  else{
    while(1){
      str = fgets(Sub_File, MAX_LENGTH,db);
      if(str != NULL){
        char *Name = strtok(str, ",");
        char *num = strtok(NULL, ",");

        strcpy(course_entry[i].name,Name);//과목이름 배열구조체
        course_entry[i].n_prerequisites = atoi(num);//num스트링을 int로 변환

        for(int j=0;j<course_entry[i].n_prerequisites;j++){
          //만약 선수강과목이있다면 그 선수강과목 수만큼..
          char * Bf  =strtok(NULL, ",");
          if(Bf[strlen(Bf)-1] == '\n'){Bf[strlen(Bf)-1] = '\0';}
          if(Bf[0] == ' '){
            for(int n=0;n<strlen(Bf);n++){Bf[n] =Bf[n+1];}
          }
        
        for(int m=0; m<i;m++){
          if(!strcmp(Bf,course_entry[m].name)){
            Before_course[i][j] = course_entry[m];
          }
        }
        course_entry[i].prerequisites = Before_course[i];
        }
        i++;
      }
    else{break;}
    }
  }//database

  if(et== NULL){
    puts("FILE OPEN FAIL!");
    return -1;
  }
  else{
    while(1){

      str = fgets(Sub_File,MAX_LENGTH, et);
      if(str != NULL){
        char *Name = strtok(str, ",");
        char *score = strtok(NULL, ",");
        for(int k=0;k<6;k++){
        if( !strcmp(course_entry[g].name ,Name)){
        course_entry[g].difficulty = atof(score);
       }
       else {
        course_entry[g].difficulty = 5.0000;
        g++;
          }
        }
       g++;
      }
    else{break;}
      }
  }//난이도 구조체에 넣기.
  
  for(int i=0; i<10;i++){
    if(course_entry[i].n_prerequisites == 0){//선수강과목 없는 과목만 ..
      for(int k=i+1; k<6;k++){
      //선수과목이없는것들중에서 난이도 비교후 정렬
        if(course_entry[k].n_prerequisites ==0){
          if(course_entry[i].difficulty < course_entry[k].difficulty){
              struct CourseEntry temp; 
              temp = course_entry[i];
              course_entry[i] = course_entry[k];
              course_entry[k] = temp;
            }
        else if(course_entry[i].difficulty == course_entry[k].difficulty){
          int len = strlen(course_entry[i].name);
            if(strcmp(course_entry[i].name,course_entry[k].name) > 0){
              struct CourseEntry temp2; 
              temp2 = course_entry[k];
              course_entry[k] = course_entry[i];
              course_entry[i] = temp2;
              }
            }
          }
        }
      }
    else if(course_entry[i].n_prerequisites != 0){ //선수과목이 있을경우
     for(int g=0;g<6;g++){ 
      if(course_entry[i].prerequisites[0].name == course_entry[g].name){
        struct CourseEntry temp1; 
        temp1 = course_entry[g];
        course_entry[g] = course_entry[i];
        course_entry[i] = temp1;

      }
     }
    }
  }// 가장 큰범위 for문

  //테스트코드
   for(int e =0; e<6; e++){ 
    printf("%s\n", course_entry[e].name);
  }
  //  printf("선수과목수:%d\n", course_entry[e].n_prerequisites);
  //  for(int t=0; t<course_entry[e].n_prerequisites; t++){
  //  printf("선수과목:%s\n", course_entry[e].prerequisites[t].name);  
  //   }
  //   printf("난이도: %3lf\n", course_entry[e].difficulty);
  // }//테스트   

  return 0;
}//main괄호
