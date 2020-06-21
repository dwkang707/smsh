#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAXCOM 1000 // max number of letters to be supported
#define grater_than	1
#define right_shift	2
#define grater_than_pipe 3
#define less_than	4
#define PIPE		5
#define SEMICOLON	6

int pipec = 0; // pipe 갯수
int semic = 0; // semicolon 갯수
int redirc = 0; // redirection 갯수
int redirkind = 0; // redirection 종류

void history(FILE *historyLog);
int isredirect(char f);
int amp_process(char *cl, int length);
int parse_command(char *buf, int len, char pars[MAXCOM][MAXCOM]);

int main()
{
	int fd, files[2], amp, n = 1; // n: history line number
	char buffers[MAXCOM];
	FILE *historyLog = fopen("historyLog.txt", "a+");

	// read command line until "end of file"
	while (1) {
		fprintf(stdout, "2017097229_shell> ");

		// parse command line
		fgets(buffers, sizeof(buffers), stdin);
		if (strcmp(buffers, "") == 0) // shell이 null로 입력되면 다시 입력 받는다.
			continue;

		fprintf(historyLog, "%d  %s\n", n, buffers); // shell을 입력하면 기록한다.
		n++;
		rewind(historyLog); // 개방된 파일에서 파일 포인터의 위치를 0으로 설정한다.
		
		char command[MAXCOM][MAXCOM] = {};
		amp = amp_process(buffers, strlen(buffers));
		int commandn = parse_command(buffers, strlen(buffers), command);
		
		// built-in command cd, history, exit ... 등은 새로운 프로세스를 생성하지 않고 실행
		if (strncmp(buffers, "exit", 4) == 0) // 사용자가 exit을 입력하면 smsh를 종료한다.
			exit(0);

		if(strcmp(buffers, "history") == 0) {
			history(historyLog);
			continue;
		}

		char **command1 = (char **)malloc(sizeof(char *) * MAXCOM);
		char *ptr = strtok(command[0], " ");
		for(int i = 0; ptr != NULL; i++) {
			command1[i] = ptr;
			ptr = strtok(NULL, " ");
		}

		if (strcmp(command1[0], "cd") == 0) {
			if (chdir(command1[1]))
				perror("No such file or directory");
		}
		else {
			char **command2 = (char **)malloc(sizeof(char *) * MAXCOM);
			ptr = strtok(command[2], " ");
			for(int i = 0; ptr != NULL; i++) {
				command2[i] = ptr;
				ptr = strtok(NULL, " ");
			}
			
			if (fork() == 0) {
				if (redirkind){
					if(redirkind == 1){ // <
						fd = open(command2[0], O_RDONLY, 0) ;
						dup2(fd, STDIN_FILENO);
						close(fd);
						execvp(command1[0], command1);
					}

					if(redirkind == 2){ // >
						fd = open(command2[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
						dup2(fd, STDOUT_FILENO);
						close(fd);
						execvp(command1[0], command1);
					/* stdout is now redirected */
					}
					
					if(redirkind == 3){ // >>
						fd = open(command2[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
						dup2(fd, STDOUT_FILENO);
						close(fd);
						execvp(command1[0], command1);
					}

					if(redirkind == 4) {// >|
						fd = open(command2[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
						dup2(fd, STDOUT_FILENO);
						close(fd);
						execvp(command1[0], command1);
					}
				}

				if (pipec) {
					pipe(files);
					if (fork() == 0) {
						/* first component of command line */
						dup2(files[1], STDOUT_FILENO);
						close(files[1]);
						close(files[0]); 
						/* stdout now goes to pipe */
						/* child process does command1 */
						execvp(command1[0], command1);
					}
					/* 2nd command of command line */
					dup2(files[0], STDIN_FILENO);
					close(files[0]);
					close(files[1]);
					/* stdin now comes from pipe */
					execvp(command2[0], command2);
				}
				execvp(command1[0], command1);
			}
			if (amp == 0)
				wait(NULL);	
		}
	} // end of while loop
	fclose(historyLog);
	return 0;
}

void history(FILE *historyLog) {
	char c;
	rewind(historyLog);
	int i = 1;
	while((c = fgetc(historyLog)) != EOF) {
        printf("%c", c);
    }
}

int isredirect(char f){
	if(f == grater_than || f == grater_than_pipe || f == right_shift || f == less_than)
		return 1;
	return 0;
}

int amp_process(char *command, int length){
	for (int i = 0; i < length; i++) {
		if (command[i] == '&') {
			command[i] = '\0';
			return 1;
		}
	}
	return 0;
}

int parse_command(char *buf, int len, char pars[MAXCOM][MAXCOM]) {
	int comidx = 0, com = 0;
	for (int i = 0; i < len; i++) {
		if (buf[i] == ';') {
			pars[++comidx][0] = SEMICOLON;
			com = 0;
			comidx++;
			semic++;
		}
		else if (buf[i] == '<') {
			pars[++comidx][0] = less_than;
			com = 0;
			comidx++;
			redirc++;
			redirkind = 1;
		}
		else if (buf[i] == '|') {
			pars[++comidx][0] = PIPE;
			com = 0;
			comidx++;
			pipec++;
		}
		else if (buf[i] == '>') {
			if (buf[i+1] == '>') {
				pars[++comidx][0] = right_shift;
				i++;
				redirkind = 3;
			}
			else if (buf[i+1] == '|') {
				pars[++comidx][0] = grater_than_pipe;
				i++;
				redirkind = 4;
			}
			else {
				pars[++comidx][0] = grater_than;
				redirkind = 2;
			}
			com = 0;
			comidx++;
			redirc++;
		}
		else {
			if (com == 0 && buf[i] == ' ')
				continue;
			if (buf[i] == ' ' && (buf[i+1] == ';' || buf[i+1] == '<' || buf[i+1] == '|' || buf[i+1] == '>'))
				continue;			
			if(i == len-1 && buf[i] == ' ')
				continue;
			pars[comidx][com++] = buf[i];
		}
	}
	return comidx;
}