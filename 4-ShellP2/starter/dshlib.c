#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX]; 
    int rc = 0; 
    cmd_buff_t cmd; 

    // TODO IMPLEMENT MAIN LOOP
    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        cmd.argc = 0;
        cmd._cmd_buffer = strdup(cmd_buff); 
        if (!cmd._cmd_buffer) {
            perror("Memory allocation failed");
            return ERR_MEMORY;
        }

        char *token = strtok(cmd._cmd_buffer, " ");
        while (token != NULL && cmd.argc < CMD_ARGV_MAX) {
            cmd.argv[cmd.argc++] = token;
            token = strtok(NULL, " ");
        }
        cmd.argv[cmd.argc] = NULL; 

        if (cmd.argc == 0) {
            printf("%s\n", CMD_WARN_NO_CMD);
            free(cmd._cmd_buffer);
            continue;
        }

        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        if (strcmp(cmd.argv[0], "exit") == 0) {
            free(cmd._cmd_buffer);
            exit(0);
        } else if (strcmp(cmd.argv[0], "cd") == 0) {
            if (cmd.argc > 1) {
                if (chdir(cmd.argv[1]) != 0) {
                    perror("cd");
                }
            }
            free(cmd._cmd_buffer);
            continue;
        }

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        pid_t pid = fork();
        if (pid == 0) { 
            execvp(cmd.argv[0], cmd.argv);
            perror("Your Execution failed"); 
            exit(1);
        } else if (pid > 0) { 
            int status;
            waitpid(pid, &status, 0); 
        } else {
            perror("fork");
        }

        free(cmd._cmd_buffer); 
    }

    return rc;
}

//Cd implementation
// void exec_cd(char *path) {
//     // If no argument is provided, do nothing (as specified in the ReadMe)
//     if (path == NULL) {
//         return;
//     }

//     // Try changing the directory
//     if (chdir(path) != 0) {
//         perror("cd"); // Print error message if chdir fails
//     }
// }