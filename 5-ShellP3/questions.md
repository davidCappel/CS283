1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The implementation ensures all child processes complete by using a waitpid() loop after creating all the processes. This is crucial because it prevents the shell from accepting new input until all piped commands have finished executing. If I forgot to call waitpid() on the child processes, several issues would arise. Zombie processes would accumulate in the system, consuming process table entries since the children would terminate but their exit statuses wouldn't be collected.
The shell wouldn't synchronize with the completion of commands, potentially leading to race conditions where new input is processed before previous commands finish.
Over time, the system could run out of process slots, causing new fork() calls to fail.
If the shell terminated while child processes were still running, they'd become orphans and be adopted by the init process, which isn't ideal for resource management.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Closing unused pipe ends after dup2() calls is critical for proper pipe functionality. When handling multiple pipes, each unused pipe end must be closed to avoid several serious problems such as File descriptor leaks, and Hanging processes


3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command must be implemented as a built-in command rather than an external process due to the fundamental nature of process creation in Unix-like systems. This necessity stems from how processes interact with their environment.
