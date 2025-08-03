#!/bin/bash

echo "=== Test Ctrl+C ==="
echo "echo 'sleep 5' | ./minishell"
echo "sleep 5" | ./minishell &
PID=$!
sleep 1
kill -INT $PID
wait $PID
echo "Exit status: $?"

echo -e "\n=== Test Ctrl+D ==="
echo "exit" | ./minishell
echo "Exit status: $?"

echo -e "\n=== Test \$? after Ctrl+C ==="
echo "echo \$?" | ./minishell
