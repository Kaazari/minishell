#!/bin/bash

# Tests Complets et Complexes pour Minishell
# ==========================================

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Fonction pour extraire la sortie de minishell
extract_minishell_output() {
    local output="$1"
    echo "$output" | grep -v "^minishell>" | sed '/^$/d'
}

# Fonction de test
test_command() {
    local test_name="$1"
    local command="$2"

    echo -e "${BLUE}=== TEST: $test_name ===${NC}"
    echo "Commande: $command"

    # Test avec bash
    bash_output=$(echo "$command" | bash 2>&1)
    bash_exit=$?

    # Test avec minishell
    minishell_full_output=$(echo "$command" | ./minishell 2>&1)
    minishell_exit=$?
    minishell_output=$(extract_minishell_output "$minishell_full_output")

    echo "Bash: '$bash_output' (exit: $bash_exit)"
    echo "Minishell: '$minishell_output' (exit: $minishell_exit)"

    if [ "$bash_output" = "$minishell_output" ] && [ "$bash_exit" = "$minishell_exit" ]; then
        echo -e "${GREEN}✓ OK${NC}"
        return 0
    else
        echo -e "${RED}✗ FAIL${NC}"
        return 1
    fi
}

# Tests de base
echo -e "${YELLOW}=== TESTS DE BASE ===${NC}"

test_command "Echo simple" "echo hello"
test_command "Echo avec espaces" "echo hello world"
test_command "Echo -n" "echo -n hello"
test_command "Echo vide" "echo"
test_command "Echo avec plusieurs -n" "echo -n -n hello"

# Tests d'arguments
echo -e "${YELLOW}=== TESTS D'ARGUMENTS ===${NC}"

test_command "Arguments simples" "ls -la"
test_command "Arguments avec guillemets" "echo 'hello world'"
test_command "Arguments avec variables" "echo $USER"
test_command "Arguments avec espaces" "echo 'hello    world'"

# Tests d'echo complexes
echo -e "${YELLOW}=== TESTS ECHO COMPLEXES ===${NC}"

test_command "Echo avec backslash" "echo 'hello\\nworld'"
test_command "Echo avec tab" "echo 'hello\\tworld'"
test_command "Echo avec retour chariot" "echo 'hello\\rworld'"
test_command "Echo avec plusieurs -n" "echo -n -n hello"
test_command "Echo avec -n et arguments" "echo -n hello world"

# Tests d'exit
echo -e "${YELLOW}=== TESTS EXIT ===${NC}"

test_command "Exit sans arguments" "exit"
test_command "Exit avec code 0" "exit 0"
test_command "Exit avec code 42" "exit 42"
test_command "Exit avec code 255" "exit 255"
test_command "Exit avec code 256" "exit 256"
test_command "Exit avec code négatif" "exit -1"
test_command "Exit avec argument non-numérique" "exit abc"
test_command "Exit avec plusieurs arguments" "exit 1 2 3"

# Tests de variables d'environnement
echo -e "${YELLOW}=== TESTS VARIABLES D'ENVIRONNEMENT ===${NC}"

test_command "Variable USER" "echo $USER"
test_command "Variable inexistante" "echo $VAR_INEXISTANT"
test_command "Variable vide" "echo $EMPTY_VAR"
test_command "Variable avec underscore" "echo $USER_NAME"
test_command "Variable avec chiffres" "echo $USER123"
test_command "Variable dans guillemets doubles" "echo \"$USER\""
test_command "Variable dans guillemets simples" "echo '$USER'"
test_command "Variable avec texte" "echo hello$USERworld"
test_command "Variable avec texte dans guillemets" "echo \"hello$USERworld\""
test_command "Variable avec texte dans simples" "echo 'hello$USERworld'"

# Tests de guillemets complexes
echo -e "${YELLOW}=== TESTS GUILLEMETS COMPLEXES ===${NC}"

test_command "Guillemets doubles simples" "echo \"hello world\""
test_command "Guillemets doubles avec variable" "echo \"hello $USER\""
test_command "Guillemets simples" "echo 'hello world'"
test_command "Guillemets simples avec variable" "echo '$USER'"
test_command "Guillemets imbriqués doubles" "echo \"hello 'world'\""
test_command "Guillemets imbriqués simples" "echo 'hello \"world\"'"
test_command "Guillemets non fermés doubles" "echo \"hello"
test_command "Guillemets non fermés simples" "echo 'hello"
test_command "Guillemets vides doubles" "echo \"\""
test_command "Guillemets vides simples" "echo ''"
test_command "Guillemets avec espaces" "echo \"   hello   world   \""
test_command "Guillemets avec tab" "echo \"hello\\tworld\""

# Tests de redirections
echo -e "${YELLOW}=== TESTS REDIRECTIONS ===${NC}"

test_command "Redirection sortie simple" "echo hello > /tmp/test.txt"
test_command "Redirection entrée simple" "cat < /tmp/test.txt"
test_command "Redirection append" "echo world >> /tmp/test.txt"
test_command "Redirection sortie avec chemin relatif" "echo hello > ./test.txt"
test_command "Redirection entrée avec chemin relatif" "cat < ./test.txt"
test_command "Redirection vers fichier inexistant" "echo hello > /tmp/nonexistent/test.txt"
test_command "Redirection depuis fichier inexistant" "cat < /tmp/nonexistent/test.txt"
test_command "Redirection multiple sortie" "echo hello > /tmp/test1.txt > /tmp/test2.txt"
test_command "Redirection multiple entrée" "cat < /tmp/test1.txt < /tmp/test2.txt"

# Tests de heredoc
echo -e "${YELLOW}=== TESTS HEREDOC ===${NC}"

test_command "Heredoc simple" "cat << EOF
hello
world
EOF"
test_command "Heredoc avec variable" "cat << EOF
hello $USER
world
EOF"
test_command "Heredoc avec guillemets" "cat << EOF
hello 'world'
EOF"
test_command "Heredoc avec delimiter variable" "cat << $USER
hello
$USER"
test_command "Heredoc vide" "cat << EOF
EOF"
test_command "Heredoc avec espaces" "cat << EOF
   hello
   world
EOF"

# Tests de pipes
echo -e "${YELLOW}=== TESTS PIPES ===${NC}"

test_command "Pipe simple" "echo hello | cat"
test_command "Pipe avec grep" "echo hello world | grep hello"
test_command "Pipe multiple" "echo hello world | grep hello | wc -l"
test_command "Pipe avec commande inexistante" "echo hello | nonexistent_command"
test_command "Pipe avec redirection" "echo hello | cat > /tmp/test.txt"
test_command "Pipe avec heredoc" "cat << EOF | grep hello
hello world
goodbye world
EOF"
test_command "Pipe vide" "echo | cat"
test_command "Pipe avec commande vide" "echo hello | ''"

# Tests de commandes builtin
echo -e "${YELLOW}=== TESTS BUILTINS ===${NC}"

test_command "CD vers /tmp" "cd /tmp && pwd"
test_command "CD vers répertoire inexistant" "cd /chemin/inexistant"
test_command "CD sans arguments" "cd"
test_command "CD vers ~" "cd ~ && pwd"
test_command "CD vers ~/Desktop" "cd ~/Desktop && pwd"
test_command "CD avec .." "cd .. && pwd"
test_command "CD avec ." "cd . && pwd"
test_command "CD avec chemin relatif" "cd ./src && pwd"

test_command "PWD" "pwd"
test_command "PWD avec arguments" "pwd /tmp"

test_command "ENV" "env"
test_command "ENV avec arguments" "env /tmp"

test_command "Export variable" "export TEST_VAR=value"
test_command "Export variable avec espaces" "export TEST_VAR=\"value with spaces\""
test_command "Export variable vide" "export TEST_VAR="
test_command "Export variable existante" "export PATH=/bin:/usr/bin"
test_command "Export sans arguments" "export"
test_command "Export avec syntaxe invalide" "export =value"
test_command "Export avec nom invalide" "export 123VAR=value"

test_command "Unset variable" "unset TEST_VAR"
test_command "Unset variable inexistante" "unset VAR_INEXISTANT"
test_command "Unset plusieurs variables" "unset VAR1 VAR2 VAR3"
test_command "Unset sans arguments" "unset"

# Tests complexes et edge cases
echo -e "${YELLOW}=== TESTS COMPLEXES ET EDGE CASES ===${NC}"

test_command "Commande très longue" "echo \"$(seq 1 50 | tr '\n' ' ')\""
test_command "Arguments avec caractères spéciaux" "echo \"hello\$USER\\n\\t\\rworld\""
test_command "Variables dans heredoc" "cat << EOF
\$USER
\$PATH
\$HOME
EOF"
test_command "Pipes multiples complexes" "echo 'hello world' | grep hello | wc -l | cat"
test_command "Redirections multiples" "echo hello > /tmp/test1.txt && echo world > /tmp/test2.txt && cat /tmp/test1.txt /tmp/test2.txt"
test_command "Commandes vides dans pipes" "echo hello | | cat"
test_command "Pipes avec commandes vides" "echo | | | cat"
test_command "Redirections avec pipes" "echo hello | cat > /tmp/test.txt && cat /tmp/test.txt"
test_command "Heredoc avec pipes" "cat << EOF | grep hello
hello world
goodbye world
hello again
EOF"
test_command "Variables avec caractères spéciaux" "echo \"\$USER\\n\$PATH\\t\$HOME\""
test_command "Guillemets avec backslash" "echo \"hello\\\"world\""
test_command "Guillemets avec backslash dans simples" "echo 'hello\\'world'"

# Tests d'erreurs
echo -e "${YELLOW}=== TESTS D'ERREURS ===${NC}"

test_command "Commande inexistante" "nonexistent_command"
test_command "Fichier inexistant" "cat /file/does/not/exist"
test_command "Permission denied" "cat /etc/shadow"
test_command "Répertoire inexistant" "ls /nonexistent/directory"
test_command "Commande avec arguments invalides" "ls --invalid-option"
test_command "Pipe avec commande qui échoue" "echo hello | cat /nonexistent/file"
test_command "Redirection vers répertoire" "echo hello > /tmp"
test_command "Redirection depuis répertoire" "cat < /tmp"

# Tests de signaux (simulation)
echo -e "${YELLOW}=== TESTS SIGNAUX ===${NC}"

echo "Note: Tests de signaux nécessitent interaction manuelle"
echo "Ctrl-C, Ctrl-D, Ctrl-\\ doivent être testés manuellement"

# Tests de syntaxe
echo -e "${YELLOW}=== TESTS SYNTAXE ===${NC}"

test_command "Guillemets non fermés doubles" "echo \"hello"
test_command "Guillemets non fermés simples" "echo 'hello"
test_command "Pipe sans commande" "| cat"
test_command "Redirection sans commande" "> /tmp/test.txt"
test_command "Redirection sans fichier" "echo hello >"
test_command "Heredoc sans delimiter" "cat <<"
test_command "Heredoc avec delimiter vide" "cat << \"\""

# Nettoyage
rm -f /tmp/test.txt /tmp/test1.txt /tmp/test2.txt ./test.txt

echo -e "${GREEN}=== TESTS TERMINÉS ===${NC}"
