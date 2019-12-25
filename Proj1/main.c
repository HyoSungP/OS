#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// #include<dirent.h>

#include <sys/stat.h>
#include <fcntl.h>



int main()
{
	char buf[4096];
	//char path[40];
	while (fgets(buf, 4096, stdin)) {
		if (strncmp(buf, "pwd", 3) == 0) {
			fflush(stdin);	
			pid_t pid = fork();//프로세스 번호(pid)를 저장하는 타입(t)
			int status;
			
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char working_directory_name[4096];
				
				getcwd(working_directory_name, 4096);//현재디렉토리얻어오는함수
				printf("%s\n", working_directory_name);
				
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);  //한번확인 
			}	
		}
		else if(strncmp(buf, "ls > youwant",4)==0){ //ls>기능구현
			pid_t pid = fork();
			int status;
			
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				int fd;
				char str[1024];
				char *lss = strtok(buf,">");
				char *filename = strtok(NULL," ");
				strncpy(str,filename,strlen(filename)-1); 
				fd = open(str,O_CREAT | O_RDWR, 0755);
				
				dup2(fd, 1);
				close(fd);
				execl("/bin/ls", "/bin/ls",NULL);
				
				exit(EXIT_SUCCESS);
				
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);   
			}	
		}
		else if(strncmp(buf, "grep wantword < wantfile",4)==0){ 
			fflush(stdin);			
			pid_t pid = fork();
			int status;
			fflush(stdout);
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char str[1024];
				
				char *grepp = strtok(buf," ");
				char *word = strtok(NULL," ");
				char *fileredirection = strtok(NULL," "); 
				char *filename = strtok(NULL,"\n");
				
				
			execl("/bin/grep", "/bin/grep",word,filename,NULL);
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);   
			}	
		}
		else if(strncmp(buf, "/bin/ls", 7)==0){ //ls 입력
			pid_t pid = fork();
			int status;
			
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char  working_directory_name[4096];
				
				getcwd(working_directory_name, 4096);

				execl("/bin/ls", "/bin/ls",NULL);	
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status); 

			}
			 
		}
		//course_sched를 execl로 실행한다.
		else if(strncmp(buf,  "~/workspace/PA0/course_sched", 12)==0){ 
			fflush(stdin);		 	
			pid_t pid = fork();//프로세스 번호(pid)를 저장하는 타입(t)
			fflush(stdout)	;		
			int status;

			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char working_directory_name[4096];
				getcwd(working_directory_name, 4096);//현재디렉토리얻어오는함수
				char *path = strtok(working_directory_name, "/"); 
				char *path1 = strtok(NULL, "/");
				//시작부분/와 그뒷부분들 붙이기
				char papath[50] = "/";
				strcat(papath, path);
				strcat(papath,"/");
				strcat(papath,path1);
				strcat(papath,"/workspace/PA0");
				chdir(papath);
				execl("course_sched","course_sched",NULL);
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);
			}	
		 }
		else if(strncmp(buf,  "cd ~/workspace/PA0", 18)==0){
			
			char working_directory_name[4096];		
			getcwd(working_directory_name, 4096);
			//현재경로에서 추출
			char *path = strtok(working_directory_name, "/"); 
			char *path1 = strtok(NULL, "/");
			//시작부분/와 그뒷부분들 붙이기
			char papath[50] = "/";
			strcat(papath, path);
			strcat(papath,"/");
			strcat(papath,path1);
			strcat(papath,"/workspace/PA0");
			chdir(papath);
			
			}
		else if(strncmp(buf,"make test", 9)==0){ //ls 입력
			pid_t pid = fork();
			int status;

			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char working_directory_name[4096];
				getcwd(working_directory_name, 4096);//현재디렉토리얻어오는함수
				char *path = strtok(working_directory_name, "/"); 
				char *path1 = strtok(NULL, "/");
				//시작부분/와 그뒷부분들 붙이기
				char papath[50] = "/";
				strcat(papath, path);
				strcat(papath,"/");
				strcat(papath,path1);
				strcat(papath,"/workspace/PA0");
				chdir(papath); 
				execl("/usr/bin/make","make","test", NULL);	
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status); // 한번 확인
			}
			 
		}
		else if(strncmp(buf,"make run",8)==0){
			fflush(stdin);			
			pid_t pid =fork();
			int status;
			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char working_directory_name[4096];
				getcwd(working_directory_name, 4096);//현재디렉토리얻어오는함수
				char *path = strtok(working_directory_name, "/"); 
				char *path1 = strtok(NULL, "/");
				//시작부분/와 그뒷부분들 붙이기
				char papath[50] = "/";
				strcat(papath, path);
				strcat(papath,"/");
				strcat(papath,path1);
				strcat(papath,"/workspace/PA0");
				chdir(papath);
				execl("/usr/bin/make","make", "run", NULL);
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);
			}
		}
		else if(strncmp(buf, "make",4)==0){
			fflush(stdin);			
			pid_t pid = fork();//프로세스 번호(pid)를 저장하는 타입(t)
			int status;

			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char working_directory_name[4096];
				getcwd(working_directory_name, 4096);//현재디렉토리얻어오는함수
				char *path = strtok(working_directory_name, "/"); 
				char *path1 = strtok(NULL, "/");
				//시작부분/와 그뒷부분들 붙이기
				char papath[50] = "/";
				strcat(papath, path);
				strcat(papath,"/");
				strcat(papath,path1);
				strcat(papath,"/workspace/PA0");
				chdir(papath);
				execl("/usr/bin/make","/usr/bin/make", NULL);
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status);
			}
		}
		else if(strncmp(buf, "echo $PATH",10)==0){
			char *ptr;
			ptr = getenv("PATH");
			printf("%s\n",ptr);
			//exit(0);
		}

		else if(strncmp(buf, "echo \"you want\" > youwant",4)==0){//echo기능
			fflush(stdin);			
			pid_t pid = fork();
			int status;

			if (pid == -1) {
				fprintf(stderr, "Error occured during process creation\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) {
				char str[1024];
				char *echoo = strtok(buf," ");
				char *text = strtok(NULL,">");
				char *filename = strtok(NULL," ");
				strncpy(str,filename,strlen(filename)-1); //null remove	
				int fp;
				int writen;
				fp = open(str , O_CREAT | O_RDWR, 0755);//파일생성 및 읽고쓰기
				writen = write(fp, text, strlen(text));
				close(fp);
				exit(EXIT_SUCCESS);
			} else {
				//waitpid(pid, &status, WNOHANG);
				pid = wait(&status); 
			}	

		}
		else if(strncmp(buf,"ls",2)==0){
			pid_t pid =fork();
			int status;
			if(pid ==-1){
				fprintf(stderr, "Erro occured during prodess creation\n");
				exit(EXIT_FAILURE);
			}else if(pid ==0){
				char working_directory_name[4096];
				getcwd(working_directory_name,4096);
				execl("/bin/ls","/bin/ls",NULL);
				exit(EXIT_SUCCESS);
			}else{
				pid = wait(&status);
				}	
			}
		else if(strncmp(buf,"ready-to-score ./2019-1-PA0/",28)==0){
			char working_directory_name[4096];
			getcwd(working_directory_name,4096);
			char *path = strtok(working_directory_name,"/");
			char *path1 =strtok(NULL,"/");
			char papath[50] ="/";
			strcat(papath,path);
			strcat(papath,"/");
			strcat(papath,path1);
			strcat(papath,"/workspace/PA1");
			//printf("%s\n",papath);
			chdir(papath);
			pid_t pid =fork();
			int status;
			if(pid ==-1){
				fprintf(stderr, "Erro occured during prodess creation\n");
				exit(EXIT_FAILURE);
			}else if(pid ==0){
				strcat(papath,"/scripts/ready-to-score.py");				
			
				execl("/usr/bin/python3","python3",papath, "./2019-1-PA0/",NULL);
				//execl("/usr/bin/python3","python3","/home/parkhyo/workspace/PA1/scripts/ready-to-score.py", "./2019-1-PA0/",NULL);
				exit(EXIT_SUCCESS);
			}else{
				pid = wait(&status);
				}		
			}	
			else if(strncmp(buf,"auto-grade-pa0 ./2019-1-PA0/",28)==0){
			char working_directory_name[4096];
			getcwd(working_directory_name,4096);
			char *path = strtok(working_directory_name,"/");
			char *path1 =strtok(NULL,"/");
			char papath[50] ="/";
			strcat(papath,path);
			strcat(papath,"/");
			strcat(papath,path1);
			strcat(papath,"/workspace/PA1");
			//printf("%s\n",papath);
			chdir(papath);
			pid_t pid =fork();
			int status;
			if(pid ==-1){
				fprintf(stderr, "Erro occured during prodess creation\n");
				exit(EXIT_FAILURE);
			}else if(pid ==0){
				strcat(papath,"/scripts/auto-grade-pa0.py");				
			
				execl("/usr/bin/python3","python3",papath, "./2019-1-PA0/",NULL);
				//execl("/usr/bin/python3","python3","/home/parkhyo/workspace/PA1/scripts/ready-to-score.py", "./2019-1-PA0/",NULL);
				exit(EXIT_SUCCESS);
			}else{
				pid = wait(&status);
				}		
			}
			else if(strncmp(buf,"report-grade ./2019-1-PA0/",26)==0){
			char working_directory_name[4096];
			getcwd(working_directory_name,4096);
			char *path = strtok(working_directory_name,"/");
			char *path1 =strtok(NULL,"/");
			char papath[50] ="/";
			strcat(papath,path);
			strcat(papath,"/");
			strcat(papath,path1);
			strcat(papath,"/workspace/PA1");
			//printf("%s\n",papath);
			chdir(papath);
			pid_t pid =fork();
			int status;
			if(pid ==-1){
				fprintf(stderr, "Erro occured during prodess creation\n");
				exit(EXIT_FAILURE);
			}else if(pid ==0){
				strcat(papath,"/scripts/report-grade.py");				
			
				execl("/usr/bin/python3","python3",papath, "./2019-1-PA0/",NULL);
				//execl("/usr/bin/python3","python3","/home/parkhyo/workspace/PA1/scripts/ready-to-score.py", "./2019-1-PA0/",NULL);
				exit(EXIT_SUCCESS);
			}else{
				pid = wait(&status);
				}		
			}
		 else {
			printf("I don't know what you said: %s", buf);
		}
		
	}

	return 0;
}//main 괄호
