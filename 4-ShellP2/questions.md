1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use fork() before execvp() to create a new child process where the command execution takes place. This ensures that the shell itself does not get replaced by the new program. fork() allows the shell to continue running independently while managing the execution of external commands. Without fork(), calling execvp() directly would replace the shell process entirely, making it impossible to return to the shell after executing a command.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, it usually indicates that the system is out of process resources. This could be due to exceeding the system's process limit or running out of memory. In our implementation, we handle this scenario by checking the return value of fork(). If it returns -1, we print an error message using perror("fork") and do not attempt to execute the command. This prevents undefined behavior from occurring due to an invalid process state.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for the specified command in the directories listed in the PATH environment variable. The system looks through each directory in PATH, checking for an executable file that matches the given command. If execvp() cannot find the command in any of the PATH directories, it returns an error, indicating that the command was not found.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait() function ensures that the parent process (the shell) waits for the child process to finish before continuing execution. Without wait(), the shell could immediately return to the prompt without knowing whether the child process completed successfully or failed. Additionally, if wait() is not called, the child process becomes what is known as a "zombie process," where it just takes all of the system resources unnecessarily until it is cleaned up by the operating system.
5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _WEXITSTATUS(status) extracts the exit status of the child process from the status value returned by waitpid(). This provides information on whether the executed command succeeded or failed. It is crucial for debugging, error handling, and implementing features such as checking return codes for conditional execution in scripts.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  Our implementation correctly handles quoted arguments by treating text inside double quotes (" ") as a single argument, even if it contains spaces. This ensures that commands like echo "Hello World" are parsed as echo and "Hello World" instead of echo, Hello, World. Without this handling, commands with spaces inside quotes would be incorrectly split, leading to incorrect execution behavior.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I really just tried to be as through as possible and not overthink the assignment. 
