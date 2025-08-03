#!/bin/bash

# Test de stress mémoire pour minishell
# Vérifie les fuites mémoire dans des scénarios complexes

echo "=== TEST DE STRESS MÉMOIRE ==="
echo

# Test 1: Pipes multiples
echo "1. Test pipes multiples:"
echo "ls | grep .c | wc -l | cat" | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_pipes.txt ./minishell
grep "definitely lost" stress_pipes.txt
echo

# Test 2: Redirections complexes
echo "2. Test redirections complexes:"
echo "echo hello > test1.txt && cat test1.txt > test2.txt && cat test2.txt && rm test1.txt test2.txt" | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_redir.txt ./minishell
grep "definitely lost" stress_redir.txt
echo

# Test 3: Opérateurs logiques
echo "3. Test opérateurs logiques:"
echo "ls && echo success || echo failure" | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_logic.txt ./minishell
grep "definitely lost" stress_logic.txt
echo

# Test 4: Variables d'environnement
echo "4. Test variables d'environnement:"
echo 'export VAR1="test1" && export VAR2="test2" && echo $VAR1 $VAR2 && unset VAR1 VAR2' | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_env.txt ./minishell
grep "definitely lost" stress_env.txt
echo

# Test 5: Heredoc complexe
echo "5. Test heredoc complexe:"
echo 'cat << EOF
line1
line2
$USER
EOF' | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_heredoc.txt ./minishell
grep "definitely lost" stress_heredoc.txt
echo

# Test 6: Commandes très longues
echo "6. Test commandes très longues:"
echo 'ls | grep .c | wc -l && echo "found files" && pwd && echo $USER && ls | head -5' | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_long.txt ./minishell
grep "definitely lost" stress_long.txt
echo

# Test 7: Gestion d'erreurs
echo "7. Test gestion d'erreurs:"
echo 'cd /chemin/inexistant || echo "cd failed" && nonexistent_command || echo "command failed"' | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_errors.txt ./minishell
grep "definitely lost" stress_errors.txt
echo

# Test 8: Exit avec code
echo "8. Test exit avec code:"
echo "exit 123" | valgrind --leak-check=full --show-leak-kinds=definite --log-file=stress_exit.txt ./minishell
grep "definitely lost" stress_exit.txt
echo

echo "=== RÉSUMÉ DES FUITES MÉMOIRE ==="
echo "Pipes multiples:"
grep "definitely lost" stress_pipes.txt
echo "Redirections:"
grep "definitely lost" stress_redir.txt
echo "Opérateurs logiques:"
grep "definitely lost" stress_logic.txt
echo "Variables d'environnement:"
grep "definitely lost" stress_env.txt
echo "Heredoc:"
grep "definitely lost" stress_heredoc.txt
echo "Commandes longues:"
grep "definitely lost" stress_long.txt
echo "Gestion d'erreurs:"
grep "definitely lost" stress_errors.txt
echo "Exit:"
grep "definitely lost" stress_exit.txt

echo "=== TEST TERMINÉ ==="
