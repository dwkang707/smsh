#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define MAXCOM 1000 // max number of letters to be supported 
#define numberchars 100 // byte
#define MAXLIST 100 // max number of commands to be supported

static char *token;

void changedir(char buffers);
void history(FILE *historyLog);
int main()
{
	int status, fd, files[2], amp, i = 1;
	char buffers[MAXCOM], *parsed[MAXLIST], *cwd;
	pid_t retid;
	FILE *historyLog = fopen("historyLog.txt", "a+");

	// read command line until "end of file"
	while (read(STDIN_FILENO, buffers, numberchars)) {
		fprintf(stdout, "%s> ", getcwd(NULL, MAXCOM));

		// parse command line
		gets(buffers);
		if (strcmp(buffers, "") == 0) // shell이 null로 입력되면 다시 입력 받는다.
			continue;

		fprintf(historyLog, "%s  %s\n", char(i), buffers); // shell을 입력하면 기록한다.
		i++;
		rewind(historyLog); // 개방된 파일에서 파일 포인터의 위치를 0으로 설정한다.
		
		if (strcmp(buffers, "exit") == 0) // 사용자가 exit을 입력하면 smsh를 종료한다.
			exit(0);

		if (buffers[strlen(buffers) - 1] == '&') // command line contains &
			amp = 1;
		else
			amp = 0;

/*
		if (strncmp("cd", buffers, 2) == 0) {
			cwd = getcwd(NULL, numberchars);
			printf("Current directory: %s\n", cwd);
			changedir(buffers);
			continue;
		}
*/
		
		
		/*
		// built-in command cd, for, while ... 등은 새로운 프로세스를 생성하지 않고 실행
		
		if (fork() == 0) { // child
			if () {// redirect output
				fd = creat(newfile, fmask);
				close(stdout);
				dup(fd);
				close(fd);
				// stdout is now redirected
			}

			if (pipe(files) == 0) {
				pipe(files);
				if (fork() == 0) {
					// first commponent of command line
					close(stdout);
					dup(files[1]);
					close(files[1]);
					close(files[0]);
					// stdout now goes to pipe
					// child process does command1
					execlp(command1, command1, 0);
				}
				close(stdin);
				dup(files[0]);
				close(files[0]);
				close(files[1]);
				// stdin now comes from pipe
			}
			execve(command2, command2, 0);
		}
		// parent continues over here, wait for child to exit if required

		if (amp == 0)
			retid = wait(&status);
		else
			continue;
			*/
	} // end of while loop
	fclose(historyLog);
	return 0;
}

/*
void changedir(char buffers) {
  //char *home;
  //home = getenv("HOME");
	char *parsed[MAXLIST], *cwd;

	for (int i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(buffers, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    }
    cwd = getcwd(NULL, numberchars);
    chdir(parsed[1]);
    return;
}
*/
void history(FILE *historyLog) {
	char c;
	rewind(historyLog);
	int i = 1;
	while((c = fgetc(historyLog)) != EOF) {
        printf("%c", c);
    }
}