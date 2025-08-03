#!/bin/bash

# Test Runner Corrigé pour Minishell vs Bash
# ==========================================

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Fonction pour extraire la sortie de minishell
extract_minishell_output() {
    local output="$1"
    # Supprimer les lignes commençant par "minishell>"
    echo "$output" | grep -v "^minishell>" | sed '/^$/d'
}

# Fonction pour tester une commande
test_command() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"

    echo -e "${BLUE}=== TEST: $test_name ===${NC}"
    echo "Commande: $command"

    # Test avec bash
    bash_output=$(echo "$command" | bash 2>&1)
    bash_exit=$?

    # Test avec minishell
    minishell_full_output=$(echo "$command" | ./minishell 2>&1)
    minishell_exit=$?

    # Extraire la sortie réelle de minishell
    minishell_output=$(extract_minishell_output "$minishell_full_output")

    echo "Bash output: '$bash_output'"
    echo "Bash exit: $bash_exit"
    echo "Minishell output: '$minishell_output'"
    echo "Minishell exit: $minishell_exit"

    if [ "$bash_output" = "$minishell_output" ] && [ "$bash_exit" = "$minishell_exit" ]; then
        echo -e "${GREEN}✓ OK${NC}"
        return 0
    else
        echo -e "${RED}✗ FAIL${NC}"
        if [ "$bash_output" != "$minishell_output" ]; then
            echo "  Différence de sortie"
        fi
        if [ "$bash_exit" != "$minishell_exit" ]; then
            echo "  Différence de code de sortie: bash=$bash_exit, minishell=$minishell_exit"
        fi
        return 1
    fi
}

# Tests de base
echo -e "${YELLOW}=== TESTS DE BASE ===${NC}"

test_command "Echo simple" "echo hello" 0
test_command "Echo avec arguments" "echo hello world" 0
test_command "Echo -n" "echo -n hello" 0
test_command "Echo vide" "echo" 0

test_command "Exit sans arguments" "exit" 0
test_command "Exit avec code" "exit 42" 0
test_command "Exit avec argument non-numérique" "exit abc" 0

test_command "PWD" "pwd" 0
test_command "PWD avec arguments" "pwd /tmp" 0

test_command "ENV" "env" 0

# Tests de variables d'environnement
echo -e "${YELLOW}=== TESTS VARIABLES D'ENVIRONNEMENT ===${NC}"

test_command "Echo variable USER" "echo \$USER" 0
test_command "Echo variable inexistante" "echo \$VAR_INEXISTANT" 0
test_command "Echo variable dans guillemets doubles" "echo \"\$USER\"" 0
test_command "Echo variable dans guillemets simples" "echo '\$USER'" 0

# Tests de guillemets
echo -e "${YELLOW}=== TESTS GUILLEMETS ===${NC}"

test_command "Guillemets doubles simples" "echo \"hello world\"" 0
test_command "Guillemets doubles avec variable" "echo \"hello \$USER\"" 0
test_command "Guillemets simples" "echo 'hello world'" 0
test_command "Guillemets simples avec variable" "echo '\$USER'" 0
test_command "Guillemets imbriqués" "echo \"hello 'world'\"" 0

# Tests de redirections
echo -e "${YELLOW}=== TESTS REDIRECTIONS ===${NC}"

test_command "Redirection sortie simple" "echo hello > /tmp/test.txt" 0
test_command "Redirection entrée simple" "cat < /tmp/test.txt" 0
test_command "Redirection append" "echo world >> /tmp/test.txt" 0

# Tests de pipes
echo -e "${YELLOW}=== TESTS PIPES ===${NC}"

test_command "Pipe simple" "echo hello | cat" 0
test_command "Pipe avec grep" "echo hello world | grep hello" 0
test_command "Pipe multiple" "echo hello world | grep hello | wc -l" 0

# Tests de commandes builtin
echo -e "${YELLOW}=== TESTS BUILTINS ===${NC}"

test_command "CD vers /tmp" "cd /tmp && pwd" 0
test_command "CD vers répertoire inexistant" "cd /chemin/inexistant" 0
test_command "CD sans arguments" "cd" 0

test_command "Export variable" "export TEST_VAR=value" 0
test_command "Export variable existante" "export PATH=/bin:/usr/bin" 0
test_command "Export sans arguments" "export" 0

test_command "Unset variable" "unset TEST_VAR" 0
test_command "Unset variable inexistante" "unset VAR_INEXISTANT" 0

# Tests complexes
echo -e "${YELLOW}=== TESTS COMPLEXES ===${NC}"

test_command "Commande très longue" "echo \"1 2 3 4 5 6 7 8 9 10\"" 0
test_command "Arguments avec caractères spéciaux" "echo \"hello\$USER\\nworld\"" 0
test_command "Heredoc simple" "cat << EOF
hello
world
EOF" 0

# Tests d'erreurs
echo -e "${YELLOW}=== TESTS D'ERREURS ===${NC}"

test_command "Commande inexistante" "nonexistent_command" 0
test_command "Fichier inexistant" "cat /file/does/not/exist" 0
test_command "Permission denied" "cat /etc/shadow" 0

# Tests de signaux (simulation)
echo -e "${YELLOW}=== TESTS SIGNAUX ===${NC}"

echo "Note: Les tests de signaux nécessitent une interaction manuelle"
echo "Ctrl-C, Ctrl-D, Ctrl-\\ doivent être testés manuellement"

# Nettoyage
rm -f /tmp/test.txt

echo -e "${GREEN}=== TESTS TERMINÉS ===${NC}"
