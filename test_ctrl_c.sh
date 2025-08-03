#!/bin/bash

echo "=== Test Ctrl+C ==="
echo "sleep 10" | ./minishell &
PID=$!
sleep 2
kill -INT $PID
wait $PID
echo "Exit status: $?"

echo -e "\n=== Test \$? after Ctrl+C ==="
echo "echo \$?" | ./minishell
