#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

void splitTokens(char **cmd1,char ** cmd2,char *cmd) {
	int i = 0;
	int len = 0;
	//printf("Here1\n");
	while(cmd[i] != '\0' && cmd[i] != '|' ) {	
		if(cmd[i] == ' '){
			i++; continue;
		}
		//printf("%c\n", cmd[i]);
		while(cmd[i] != '\0' && cmd[i] != ' ' && cmd[i] != '|') {
			int j = 0;
			while(cmd[i] != '\0' && cmd[i] !=  ' ') {
				//printf("Inside %c\n", cmd[i]);
				if(cmd[i] == '|') {
					//printf("Here baby exactly\n");
					break;
				}
				cmd1[len][j] = cmd[i];
				i++; j++;
			}
			len++;
		}
	}
	cmd1[len] = NULL;
	if(cmd2 == NULL)
		return;
	//printf("---------------------------------\n");
	int j = 0;
	i++;len = 0;
	while(cmd[i] != '\0'  && cmd[i] != '|' ) {	

		if(cmd[i] == ' ' || cmd[i] == '\n'){
			i++; continue;
		}
		//printf("%c\n", cmd[i]);
		while(cmd[i] != '\0' && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '\n') {
			int j = 0;
			while(cmd[i] != '\0' && cmd[i] !=  ' ' && cmd[i] != '\n') {
				//printf("Inside %c\n", cmd[i]);
				if(cmd[i] == '|') {
					//printf("Here baby exactly\n");
					break;
				}
				cmd2[len][j] = cmd[i];
				i++; j++;
			}
			len++;
		}
	}
	cmd2[len] = NULL;
	j = 0;
}
void createPipes(char *cmd) {
	// printf("In pipes\n");
	 int i = 0,status;
	 char **cmd1 = (char **)malloc(10 * sizeof(char*));
	 for( i = 0;i < 10;i++) {
	 	cmd1[i] = (char *)malloc(50);
	 }
	 char **cmd2 = (char **)malloc(10 * sizeof(char*));
	 for(i = 0;i < 10; i++) {
	 	cmd2[i] = (char *)malloc(50);
	 }
	splitTokens(cmd1,cmd2,cmd);
	int pipefd[2];
 	int childpid,childpid2;
 	int stat= pipe(pipefd);
 	if(childpid=fork()){
 		fflush(stdout);
 		fflush(stdin);
   		close(pipefd[1]);
   		int stat = dup2(pipefd[0],STDIN_FILENO);
   		//printf("\n%d stat\n",stat);
   		int status = execvp(cmd2[0],cmd2);
 	}else{  
   		//child
   		//write
   		fflush(stdout);
 		fflush(stdin);
   		close(pipefd[0]);
   		dup2(pipefd[1],STDOUT_FILENO);
   		int status = execvp(cmd1[0], cmd1);
 	}
}

void do_redirect2(char** cmd, char* file) {
  int fds[2]; 
  int count; 
  int fd;     
  char i;    
  pid_t pid;  
  pipe(fds);

  while(cmd[i] != NULL) {
  	//printf("%s\n",cmd[i]);
  	i++;
  }
  //printf("%s\n", file);
  //File Descriptors/pipe and redirecting char variables (i)
  //fd is used with the open command, basically stores the 
  //Child 1
  if (fork() == 0) {
    //Open the file with read/write commands, 0_CREAT creates the file if it does not exist
    fd = open(file, O_RDWR| O_TRUNC | O_CREAT, 0777);
    dup2(fds[0], 0);
    //Close STDOUT
    close(fds[1]);
    //Read from STDOUT
    while ((count = read(0, &i, 1)) > 0)
    write(fd, &i, 1); // Write to file.
  //Child 2
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);

    //Close STDIN
    close(fds[0]);
    //Output contents to the given file.
    execvp(cmd[0], cmd);
    perror("execvp failed");

  //Parent
  } else {
    waitpid(pid, NULL, 0);
    close(fds[0]);
    close(fds[1]);
  }
}
void do_redirect(char** cmd, char* file) {
  int fds[2]; 
  int count; 
  int fd;     
  char i;    
  pid_t pid;  
  pipe(fds);

  while(cmd[i] != NULL) {
  	//printf("%s\n",cmd[i]);
  	i++;
  }
  //printf("%s\n", file);
  //File Descriptors/pipe and redirecting char variables (i)
  //fd is used with the open command, basically stores the 
  //Child 1
  if (fork() == 0) {
    //Open the file with read/write commands, 0_CREAT creates the file if it does not exist
    fd = open(file, O_RDWR| O_APPEND | O_CREAT, 0777);
    dup2(fds[0], 0);
    //Close STDOUT
    close(fds[1]);
    //Read from STDOUT
    while ((count = read(0, &i, 1)) > 0)
    write(fd, &i, 1); // Write to file.
  //Child 2
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);

    //Close STDIN
    close(fds[0]);
    //Output contents to the given file.
    execvp(cmd[0], cmd);
    perror("execvp failed");

  //Parent
  } else {
    waitpid(pid, NULL, 0);
    close(fds[0]);
    close(fds[1]);
  }
}
void redirection2(char *cmd) {
	char *arg1 = malloc(50);
	int i = 0;
	while(cmd[i] != '\0' && cmd[i] != '>') {
		arg1[i] = cmd[i];
		i++;
	}
	arg1[i] = '\0';
	char **cmd1 = (char **)malloc(10 * sizeof(char*));
	int j = 0;
	 for( j = 0;j < 10;j++) {
	 	cmd1[j] = (char *)malloc(50);
	 }
	 char **cmd2 = NULL;
	 splitTokens(cmd1,cmd2,arg1);
	 i++;
	 char *file = malloc(50);
	 j = 0;
	 while(cmd[i] != '\0' && cmd[i] != '\n') {
	 	file[j] = cmd[i];
	 	j++; i++;
	 }
	 file[j] = '\0';
	 do_redirect2(cmd1,file);
}

void redirection3(char *cmd) {
	char *arg1 = malloc(50);
	int i = 0;
	while(cmd[i] != '\0' && cmd[i] != '>') {
		arg1[i] = cmd[i];
		i++;
	}
	arg1[i] = '\0';
	char **cmd1 = (char **)malloc(10 * sizeof(char*));
	int j = 0;
	 for( j = 0;j < 10;j++) {
	 	cmd1[j] = (char *)malloc(50);
	 }
	 char **cmd2 = NULL;
	 splitTokens(cmd1,cmd2,arg1);
	 i++; i++;
	 char *file = malloc(50);
	 j = 0;
	 while(cmd[i] != '\0' && cmd[i] != '\n') {
	 	file[j] = cmd[i];
	 	j++; i++;
	 }
	 file[j] = '\0';
	 do_redirect(cmd1,file);
}
void runCommand(char * args) {
//	printf("The input string is %s\n", args);
	/* Split the Command into tokens
		for example :
		ls -l should be split as ls, l

		Here we assume that we have a single command to execute
	*/
	int i = 0;
	//printf("Now Here\n");
	int num_tokens = 0;
	char **tokens = (char **)malloc(10 * sizeof(char*));
	for( i = 0;i < 10;i++) {
		tokens[i] = (char *)malloc(50);
	}	
	i = 0;
	while(args[i] != '\0') {
		if(args[i] == ' ' || args[i] == '\n'){
			i++; 
			continue;
		}			
			int index = 0;
			while(args[i] != '\0' && args[i] != '\n' && args[i] != ' ') {
				tokens[num_tokens][index] = args[i];
				index++; i++;
			}
			tokens[num_tokens][index] = '\0';
			num_tokens++;	
	}
	tokens[num_tokens] = NULL;
	int index = 0;
	for(index = 0; index < num_tokens; index++) {
		//printf("%s ",tokens[index]);
	}
	char * env = getenv("PATH");
	char * env_tok = strtok(env, ":");
	char **p_path= (char **)malloc(15 * sizeof(char*));
	for(i = 0;i < 15;i++) {
		p_path[i] = (char *)malloc(50);
	}
	i = 0;
	int len = 0;
	while(env_tok != NULL) {
		int l = 0;
		while(env_tok[l] != '\0') {
			p_path[i][l] = env_tok[l];
			l++;
		}
		p_path[i][l] = '\0';
		env_tok = strtok(NULL,":");
		len++;
		i++;
	}
	int num = len;
	pid_t pid;
	pid = fork();

	if(pid < 0){
		printf("fork failed");
		return;
	}
	else if (pid == 0) {
		//printf("IN Child\n");
		for(i = 0;i < num;i++) {
			int len = 0;
			char exec_path[100];
			int j = 0;
			while(p_path[i][j] != '\0') {
				exec_path[j] = p_path[i][j];
				len++;
				j++;
			}
			exec_path[j] = '/';
			len++;
			j++;
			int k = 0;
			while(tokens[0][k] != '\0'){
				if(tokens[0][k] == ' '){
					k++; continue;
				}
				exec_path[j] = tokens[0][k];	
				k++; j++; len++;
			}
			exec_path[j] = '\0';
			int z = 0;
			char *x = malloc(len);
			for( z = 0; z< len;z++) {
				x[z] = exec_path[z]; 
			}
   			z = 0;
			int zz = 0;
			for(zz = 0; zz < num_tokens;zz++) {
   			//	printf("\n%s", tokens[zz]);
   			}
   			//printf("%s\n", x);
			int status2 = execv(x,tokens);
			//execvp(tokens[0], cmd);
			//printf("\n%d\n", status2);
		}
	}
	else {
		wait(NULL);
		//printf("Child Complete\n");
	}
}
char *getNextToken(char * cmd) {
	int i = 0;
	int pos = -1;
	int type = -1;
	char *rem = malloc(50);

	//printf("\nIn Here\n");
	while(cmd[i] != '\0') {
			if(cmd[i] == '|') {
				createPipes(cmd);
				type = 0;
				pos = i;
				rem = strchr(cmd,'|');
				break;
			}
			else if(cmd[i] == '>' && cmd[i+1] != '>') {
				redirection2(cmd);
				type = 1;
				pos = i;
				rem = strchr(cmd,'>');
				break;
			}
			else if(cmd[i] == '<' && cmd[i+1] != '<') {
				type = 2;
				pos = i;
				rem = strchr(cmd,'<');
				break;
			}
			else if(cmd[i] == '>' && cmd[i+1] == '>') {
				redirection3(cmd);
				type = 3;
				pos = i;
				rem = strchr(cmd,'>');
				break;
			}
			else if(cmd[i] == '<' && cmd[i+1] == '<') {
				type = 4;
				pos = i;
				rem = strchr(cmd,'<');
				break;
			}
			i++;
		}
		char *args = malloc(50);
		char *addons = malloc(50);
		if(pos != -1) {
			i = 0;
			for(i = 0;i < pos;i++) {
				args[i] = cmd[i];
			}
			args[pos] = '\0';
			i = 1;
			int j = 0;
			while(rem[i] != '\0') {
				cmd[j] = rem[i];
				j++; i++;
			}
			cmd[j] = '\0';
		}
		else{
			i = 0;
			while(cmd[i] != '\0'){
				args[i] = cmd[i];
				i++;
			}
			args[i] = '\0';
			cmd[0] = '\0';
			runCommand(args);
		}
		return args;
}
int main() {
	
	printf("\n>$ ");

	while(1) {
		//fflush(stdout);
		char * cmd = malloc(50);
		if(fgets(cmd,20,stdin) != NULL){
			//fflush(NULL);
			printf("%s\n",cmd);
			printf("\n>$ ");

		//	fflush(NULL);
			int i = 0;	
			char * nextTok = malloc(50);
			nextTok = getNextToken(cmd);
			//break;
		}
		
		//fflush(stdout);
	}
}