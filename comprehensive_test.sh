#!/bin/bash

# Test complet du minishell
# Vérifie toutes les fonctionnalités principales

echo "=== TESTS COMPLETS MINISHELL ==="
echo

# Test 1: Commandes simples
echo "1. Test commandes simples:"
echo "ls" | ./minishell
echo

# Test 2: Variables d'environnement
echo "2. Test variables d'environnement:"
echo 'echo $USER' | ./minishell
echo 'echo "$USER"' | ./minishell
echo 'echo '\''$USER'\''' | ./minishell
echo

# Test 3: Builtins
echo "3. Test builtins:"
echo "pwd" | ./minishell
echo "echo hello world" | ./minishell
echo "echo -n hello world" | ./minishell
echo

# Test 4: Export/Unset
echo "4. Test export/unset:"
echo 'export TEST_VAR="test value"' | ./minishell
echo 'echo $TEST_VAR' | ./minishell
echo 'unset TEST_VAR' | ./minishell
echo 'echo $TEST_VAR' | ./minishell
echo

# Test 5: CD
echo "5. Test cd:"
echo "pwd" | ./minishell
echo "cd /tmp && pwd" | ./minishell
echo "cd - && pwd" | ./minishell
echo

# Test 6: Pipes
echo "6. Test pipes:"
echo "ls | grep .c | wc -l" | ./minishell
echo "echo hello | cat" | ./minishell
echo

# Test 7: Redirections
echo "7. Test redirections:"
echo "echo hello > test_file.txt" | ./minishell
echo "cat test_file.txt" | ./minishell
echo "rm test_file.txt" | ./minishell
echo

# Test 8: Heredoc
echo "8. Test heredoc:"
echo 'cat << EOF
hello world
$USER
EOF' | ./minishell
echo

# Test 9: Opérateurs logiques
echo "9. Test opérateurs logiques:"
echo "ls && echo 'success'" | ./minishell
echo "nonexistent_command || echo 'fallback'" | ./minishell
echo

# Test 10: Exit
echo "10. Test exit:"
echo "echo 'testing exit'" | ./minishell
echo "exit 42" | ./minishell
echo "Exit code: $?"
echo

# Test 11: Commandes complexes
echo "11. Test commandes complexes:"
echo "ls | grep .c | wc -l && echo 'found .c files'" | ./minishell
echo

# Test 12: Gestion d'erreurs
echo "12. Test gestion d'erreurs:"
echo "cd /chemin/inexistant" | ./minishell
echo "nonexistent_command" | ./minishell
echo

echo "=== TESTS TERMINÉS ==="
