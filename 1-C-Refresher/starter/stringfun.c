#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int i = 0;
    int j = 0;
    int str_len = 0;
    int last_char_was_space = 1;

     while (user_str[i] != '\0') {
        if (j >= len) {
            return -1;
        }
        if (user_str[i] == ' ' || user_str[i] == '\t') {
            if (!last_char_was_space) {
                buff[j++] = ' ';
                last_char_was_space = 1;
            }
        } else {
            buff[j++] = user_str[i];
            str_len++;
            last_char_was_space = 0;
        }
        i++;
    }
    while (j < len) {
        buff[j++] = '.';
    }
    return str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int i = 0;
    int in_word = 0; 

    if (buff == NULL) {
        return -1; 
    }
    while(i < len && i < str_len){ 
        if (*(buff + i) == ' ') {
            in_word = 0; 
        } else if (in_word == 0) {
            word_count++; 
            in_word = 1;  
        }
        i++;
    }
    
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //  This code is fine becasuse if no command line argument is profided by the user, thatn the program will just exit after it calls usage.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      this will just be to chedk if tht user gives a string after one fo the operation indicaters, and if not it exits
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL){
        printf("Error with memory allocation\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r': {
            //reverse string
            char *temp_buff;
            temp_buff = (char*)malloc(user_str_len);
            if (temp_buff == NULL){
              printf("Error allocating memory\n");
              exit(2);
            }
            for (int i = 0; i < user_str_len; i++){
                *(temp_buff+i) = *(buff + user_str_len - 1 - i);
            }

            printf("Reversed String: ");
            for(int i = 0; i < user_str_len; i++){
                putchar(*(temp_buff + i));
            }
            putchar('\n');
            free(temp_buff);
            break;
        }
     case 'w': {
        int word_count = 0;
        int start_index = -1;
        int i = 0;

        printf("Word Print\n");
        printf("----------\n");

        for (i = 0; i <= user_str_len; i++) {
            if (i == user_str_len || *(buff + i) == ' ') {
                if (start_index != -1) {
                    word_count++;
                    printf("%d. ", word_count);
                    
                    for (int j = start_index; j < i; j++) {
                        putchar(*(buff + j));
                    }
                    
                    printf(" (%d)\n", i - start_index);
                    
                    start_index = -1;
                }
            }
            else if (start_index == -1) {
                start_index = i;
            }
        }

        printf("Number of words returned: %d\n", word_count);
        break;
    }
        case 'x': {
            if (argc != 5){
                usage(argv[0]);
                exit(1);
            }
            printf("Not Implemented!\n");
            exit(3);
            break;
        }

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
// Giving both the buffer and the length helps so that both the main funtiona nd helpers are aware of the size of the pointer. The helpers could be used in some cases where the buffer size is not specified so giving both mitigates this.