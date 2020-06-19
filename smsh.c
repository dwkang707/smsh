#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define MAXCOM 1000 // max number of letters to be supported 
#define numberchars 100 // max number of commands to be supported
static char *token;

int main(int argc, char const *argv[])
{
	int status, fd, files[2];
	char buffers[MAXCOM];
	pid_t retid;

	/* read command line until "end of file" */
	while (read(stdin, buffers, numberchars)) {
		/* parse command line */
		if (/* command line contains & */)
			amp = 1;
		else
			amp = 0;
		/* built-in command cd, for, while ... 등은 새로운 프로세스를 생성하지 않고 실행 */
		
		if (fork() == 0) { // child
			if (/* redirect output */) {
				fd = creat(newfile, fmask);
				close(stdout);
				dup(fd);
				close(fd);
				/* stdout is now redirected */
			}

			if (pipe(files) == 0) {
				pipe(files);
				if (fork() == 0) {
					/* first commponent of command line */
					close(stdout);
					dup(files[1]);
					close(files[1]);
					close(files[0]);
					/* stdout now goes to pipe */
					/* child process does command1 */
					execlp(command1, command1, 0);
				}
				close(stdin);
				dup(files[0]);
				close(files[0]);
				close(files[1]);
				/* stdin now comes from pipe */
			}
			execve(command2, command2, 0);
		}
		/* parent continues over here, wait for child to exit if required */

		if (amp == 0)
			retid = wait(&status);
		else
			continue;
	} // end of while loop

	return 0;
}

	