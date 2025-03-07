#!/usr/bin/env bats
# File: student_tests.sh
#
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF
ls
EOF
    # Assertions
    [ "$status" -eq 0 ]
}
@test "Empty input does nothing" {
    run ./dsh <<< "\n"
    [ "$status" -eq 0 ]
}

# Test multiple spaces between command arguments
@test "Multiple spaces are handled correctly" {
    run ./dsh <<< "echo     Hello   World"
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello World"* ]]
}

# Test built-in exit command
@test "Exit command terminates shell" {
    run ./dsh <<< "exit"
    [ "$status" -eq 99 ]
}

# Test executing a command with arguments
@test "Execute ls -l command" {
    run ./dsh <<< "ls -l"
    [ "$status" -eq 0 ]
    [[ "$output" == *"total"* ]]
}

# Test forking a process with sleep
@test "Fork and wait for process" {
    run ./dsh <<< "sleep 1"
    [ "$status" -eq 0 ]
}

@test "Pipe with large data: cat dshlib.c | grep include" {
    run ./dsh <<< "cat dshlib.c | grep include"
    [ "$status" -eq 0 ]
    [[ "$output" == *"#include"* ]]
}

@test "Pipe with invalid command: ls | nonexistentcommand" {
    run ./dsh <<< "ls | nonexistentcommand"
    [ "$status" -eq 0 ]
    [[ "$output" == *"Execution failed"* ]] || [[ "$output" == *"not found"* ]]
}

@test "Triple pipe: ls | grep .c | wc -l" {
    FILE=$(mktemp)
    ./dsh <<< "ls | grep .c | wc -l" > $FILE

    OUTPUT=$(cat $FILE)
    
    
    EXPECTED=$(ls | grep .c | wc -l)
    
    
    [[ "$OUTPUT" == *"$EXPECTED"* ]]
    
    
    rm $FILE
}

@test "It handles quoted spaces" {
    run "./dsh" <<EOF                
   echo " hello     world     " 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output=" hello     world     dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}