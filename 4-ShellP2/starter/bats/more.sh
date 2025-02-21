#!/usr/bin/env bats

# Test handling of empty input
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
    [ "$status" -eq 0 ]
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
