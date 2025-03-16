#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

// old code
int parse_command_line(char *cmd_line, command_list_t *cmd_list) {
    int cmd_count = 0;
    char *cmd_start = cmd_line;
    char *pipe_pos;
    
    
    cmd_list->num = 0;
    
    
    while (cmd_start != NULL && *cmd_start != '\0') {
        
        while (*cmd_start == ' ') {
            cmd_start++;
        }
        
        if (*cmd_start == '\0') {
            break;
        }
        
        bool in_quotes = false;
        pipe_pos = NULL;
        for (char *p = cmd_start; *p != '\0'; p++) {
            if (*p == '"') {
                in_quotes = !in_quotes;
            } else if (*p == PIPE_CHAR && !in_quotes) {
                pipe_pos = p;
                break;
            }
        }
        
        cmd_list->commands[cmd_count]._cmd_buffer = NULL;
        cmd_list->commands[cmd_count].argc = 0;
        
        if (pipe_pos != NULL) {
            
            *pipe_pos = '\0';
        }
        
        
        char *temp_cmd = strdup(cmd_start);
        if (!temp_cmd) {
            return ERR_MEMORY;
        }
        
        
        int arg_index = 0;
        char *p = temp_cmd;
        char *arg_start = p;
        in_quotes = false;
        
        while (*p != '\0' && arg_index < CMD_ARGV_MAX - 1) {
            if (*p == '"') {
                
                in_quotes = !in_quotes;
                
                memmove(p, p + 1, strlen(p));
                
                continue;
            } else if ((*p == ' ' && !in_quotes) || *p == '\0') {
                
                if (p != arg_start) {  
                    *p = '\0';  
                    cmd_list->commands[cmd_count].argv[arg_index++] = strdup(arg_start);
                    arg_start = p + 1;
                } else {
                    
                    arg_start++;
                }
            }
            
            p++;
            
            
            if (*p == '\0' && arg_start < p && !in_quotes) {
                
                cmd_list->commands[cmd_count].argv[arg_index++] = strdup(arg_start);
            }
        }
        
        
        cmd_list->commands[cmd_count].argc = arg_index;
        cmd_list->commands[cmd_count].argv[arg_index] = NULL;

        
        cmd_list->commands[cmd_count]._cmd_buffer = strdup(cmd_start);
        
        
        free(temp_cmd);
        
        
        if (pipe_pos != NULL) {
            *pipe_pos = PIPE_CHAR;
            cmd_start = pipe_pos + 1;
        } else {
            cmd_start = NULL;
        }
        
        cmd_count++;
        
        
        if (cmd_count >= CMD_MAX) {
            break;
        }
    }
    
    cmd_list->num = cmd_count;
    
    if (cmd_count == 0) {
        return WARN_NO_CMDS;
    } else if (cmd_count > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }
    
    return OK;
}


int execute_pipeline(command_list_t *cmd_list) {
    int i;
    int pipes[CMD_MAX-1][2]; 
    pid_t pids[CMD_MAX];     
    
    for (i = 0; i < cmd_list->num - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }
    
    for (i = 0; i < cmd_list->num; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            return ERR_EXEC_CMD;
        } else if (pids[i] == 0) {
            
            
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            if (i < cmd_list->num - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            
            for (int j = 0; j < cmd_list->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            execvp(cmd_list->commands[i].argv[0], cmd_list->commands[i].argv);
            
            
            perror("Your Execution failed");
            exit(1);
        }
    }
    
    
    for (i = 0; i < cmd_list->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    
    for (i = 0; i < cmd_list->num; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
    
    return OK;
}


int is_builtin_command(cmd_buff_t *cmd) {
    if (cmd->argc > 0) {
        if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
            return 1;
        } else if (strcmp(cmd->argv[0], "cd") == 0) {
            return 1;
        }
    }
    return 0;
}


int execute_builtin(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        printf("exiting...\n");
        exit(EXIT_SC);
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc > 1) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
            }
        }
        return 0;
    }
    return -1;
}


void free_command_list(command_list_t *cmd_list) {
    for (int i = 0; i < cmd_list->num; i++) {
        
        if (cmd_list->commands[i]._cmd_buffer) {
            free(cmd_list->commands[i]._cmd_buffer);
        }
        
       
        for (int j = 0; j < cmd_list->commands[i].argc; j++) {
            if (cmd_list->commands[i].argv[j]) {
                free(cmd_list->commands[i].argv[j]);
            }
        }
    }
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    // This function should parse the cmd_line into clist->commands array
    int rc = parse_command_line(cmd_line, clist);  // Assuming you already have `parse_command_line()` implemented
    return rc;
}

int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX]; 
    int rc = 0; 
    command_list_t cmd_list;

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        
        
        rc = parse_command_line(cmd_buff, &cmd_list);
        
        if (rc == WARN_NO_CMDS) {
            printf("%s\n", CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            free_command_list(&cmd_list);
            continue;
        } else if (rc == ERR_MEMORY) {
            perror("Memory allocation failed");
            free_command_list(&cmd_list);
            continue;
        }
        
        
        if (cmd_list.num == 1 && is_builtin_command(&cmd_list.commands[0])) {
            execute_builtin(&cmd_list.commands[0]);
        } else {
            
            rc = execute_pipeline(&cmd_list);
        }
        
        
        free_command_list(&cmd_list);
    }

    return rc;
}