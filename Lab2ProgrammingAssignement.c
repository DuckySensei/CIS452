// Step 1: Displays the user's prompt`

//Step 2: Recieve and parse the user input
//        - read line pref using fgets()
//        - tokenize the line using strtok() or strsep()

//Step 3: Spawn a child process to execute the command (fork())
//        - probably gonna use execvp() because I know how to use it

//Step 4: Figure out how to get the user CPU time used
//        - Make sure this runs with each individual child processes.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//This #include is for the CPU time usage
#include <sys/resource.h> 

int main() {
    // Step 1:
    char command[64];
    char *args[16];

    while (1) {
	//this is to set up the 'Shell'... the Dyl one. It's the 'real dyl'
        printf("Dyl-Shell> ");
        fgets(command, 64, stdin);
        command[strcspn(command, "\n")] = '\0';
	
	//break if someone types "quit"
        if (strcmp(command, "quit") == 0) {
            break;
        }
	
	//Step 2:
	//make a token of the input command
        int i = 0;
        char *token = strtok(command, " ");
	
	//So if the token isn't empty...
        while (token != NULL) {
	    //save each word as an arguement
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
	//end the arguement list with null.. Always end with null 
        args[i] = NULL;

	//Step 3:
        // Create a child process
        pid_t pid = fork();

        if (pid == 0) { //THIS IS THE CHILD PROCESS!
            execvp(args[0], args);
            perror("exec failed");
            return 1;  // Exit child if exec fails
        } else if (pid > 0) { //THIS IS THE PARENT PROCESS!
            
	    //Child to finish before parent b/c they run concurrently otherwise
            int status;
            wait(&status);

	    //Step 4:
            //This is where I am getting the CPU ussage
            struct rusage usage;
            getrusage(RUSAGE_CHILDREN, &usage);

	    //Show how long it takes :)
            printf("Your CPU time: %ld.%06ld seconds\n",
                   usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
            printf("Unwanted Context Switches: %ld\n", usage.ru_nivcsw);
        }
    }

    //always add this return
    return 0;
}

