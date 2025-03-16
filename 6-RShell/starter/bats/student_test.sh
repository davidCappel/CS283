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

