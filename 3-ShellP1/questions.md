1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: `fgets()` is a good choice for this application because it is designed to **read input line by line** . It reads at most one less than the specified size characters from the input stream and stores them into a buffer until it encounters a newline or EOF. If a newline is read, it is stored into the buffer, and a terminating null byte is added. This makes `fgets()` suitable for processing user input in a shell, where commands are typically entered one line at a time. Using `fgets()` helps in creating a "line by line" processor, which aligns well with how shells operate.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: Using `malloc()` allows for **dynamic memory allocation**, which is necessary because the size of the user input is not known at compile time. Allocating a fixed-size array might lead to a buffer overflow if the user enters a command longer than the array size . `malloc()` allocates memory at runtime based on the size defined in `dshlib.h`, providing flexibility and preventing potential buffer overflows.



3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: Trimming leading and trailing spaces is necessary to ensure that the commands and arguments are parsed correctly . If spaces are not trimmed, the shell might misinterpret the command or arguments, leading to **incorrect execution or errors** . For example, if a command is stored as " cmd ", the shell might not recognize it as "cmd" . The `build_cmd_list` function's purpose is to split the input into commands and parse each command by identifying the executable name and arguments. Leading and trailing spaces would interfere with this parsing process .

