
# TESTS CHECKLIST - MINISHELL vs BASH
# ======================================

## LÉGENDE
# ✅ OK = Fonctionne comme bash
# ❌ FAIL = Ne fonctionne pas du tout
# ⚠️ DIFF = Fonctionne mais différent de bash
# 🔍 TODO = À tester manuellement

## 1. COMPILATION
- ✅ make -n fonctionne
- ✅ Compilation sans erreurs
- ✅ Pas de re-linking inutile

## 2. COMMANDE SIMPLE ET VARIABLES GLOBALES
- ✅ Commande simple avec PATH absolu (/bin/ls)
- ✅ Discussion variables globales avec évaluateur
- ✅ Commande vide

## 3. ARGUMENTS
- ✅ Commande avec arguments simples
- ✅ Commande avec arguments et guillemets simples
- ✅ Commande avec arguments et guillemets doubles
- ✅ Tests multiples avec différentes commandes
- ✅ Pas de crash

## 4. ECHO
- ✅ echo sans arguments
- ✅ echo avec arguments
- ✅ echo -n
- ✅ echo avec guillemets
- ✅ echo avec variables d'environnement
- ✅ Tests multiples

## 5. EXIT
- ❌ exit sans arguments (CRASH - segmentation fault)
- ❌ exit avec argument numérique (CRASH - segmentation fault)
- ❌ exit avec argument non-numérique (CRASH - segmentation fault)
- ❌ exit avec plusieurs arguments (CRASH - segmentation fault)
- ❌ Tests multiples (CRASH - segmentation fault)

## 6. VALEUR DE RETOUR D'UN PROCESSUS ($?)
- 🔍 $? après commande réussie (à tester manuellement)
- 🔍 $? après commande échouée (à tester manuellement)
- 🔍 $? $? (double) (à tester manuellement)
- 🔍 Comparaison avec bash (à tester manuellement)

## 7. SIGNAUX
- 🔍 Ctrl-C sur prompt vide (à tester manuellement)
- 🔍 Ctrl-\ sur prompt vide (à tester manuellement)
- 🔍 Ctrl-D sur prompt vide (à tester manuellement)
- 🔍 Ctrl-C après saisie (à tester manuellement)
- 🔍 Ctrl-D après saisie (à tester manuellement)
- 🔍 Ctrl-\ après saisie (à tester manuellement)
- 🔍 Ctrl-C pendant commande bloquante (à tester manuellement)
- 🔍 Ctrl-\ pendant commande bloquante (à tester manuellement)
- 🔍 Ctrl-D pendant commande bloquante (à tester manuellement)

## 8. GUILLEMETS DOUBLES
- ✅ echo "hello world"
- ❌ echo "hello $USER" (variables non interprétées)
- ❌ echo "hello 'world'" (guillemets imbriqués non gérés)
- ❌ Variables d'environnement interprétées
- ✅ Espaces préservés

## 9. GUILLEMETS SIMPLES
- ✅ echo 'hello world'
- ✅ echo '$USER' (pas d'interprétation)
- ✅ echo 'hello "world"'
- ✅ Rien interprété dans les simples

## 10. ENV
- ✅ env affiche les variables
- ✅ env avec arguments (doit échouer)

## 11. EXPORT
- ✅ export VAR=value
- ✅ export VAR="value with spaces"
- ✅ export VAR (sans valeur)
- ✅ export VAR= (valeur vide)
- ✅ export VAR existant
- ⚠️ export sans arguments (ordre des variables différent)

## 12. UNSET
- ✅ unset VAR
- ✅ unset VAR inexistant
- ✅ unset plusieurs variables
- ✅ unset sans arguments

## 13. CD
- ✅ cd /tmp
- ✅ cd ..
- ✅ cd .
- ✅ cd ~
- ✅ cd ~/Desktop
- ⚠️ cd chemin inexistant (message d'erreur différent)
- ✅ cd sans arguments

## 14. PWD
- ✅ pwd
- ✅ pwd avec arguments (doit échouer)

## 15. CHEMIN RELATIF
- ✅ ./minishell
- ✅ ../minishell
- ✅ ../../minishell
- ✅ ls ./src
- ✅ ls ../src

## 16. PATH D'ENVIRONNEMENT
- ✅ ls (sans PATH)
- ✅ cat (sans PATH)
- ✅ PATH=/bin:/usr/bin
- ✅ PATH multiple
- ✅ PATH vide

## 17. REDIRECTION
- ✅ echo hello > file.txt
- ✅ cat < file.txt
- ✅ echo hello >> file.txt
- ✅ cat << EOF
- ✅ Multiple redirections
- ✅ Redirections invalides

## 18. PIPES
- ✅ ls | grep
- ✅ ls | grep | wc -l
- ✅ cat | grep
- ✅ echo hello | cat
- ✅ Pipes avec redirections
- ✅ Pipes invalides

## 19. VARIABLES D'ENVIRONNEMENT
- ✅ echo $USER
- ❌ echo "$USER" (variables non interprétées dans guillemets doubles)
- ✅ echo '$USER'
- ✅ echo $VAR_INEXISTANT
- ✅ echo $VAR_INEXISTANT$USER

## 20. TESTS COMPLEXES ET TRICKY
- ✅ Commandes très longues
- ⚠️ Arguments avec caractères spéciaux (\n mal interprété)
- ❌ Nested quotes (guillemets imbriqués non gérés)
- ✅ Heredoc complexe
- ✅ Pipes multiples
- ✅ Redirections multiples
- ✅ Variables dans heredoc
- 🔍 Signaux dans pipes (à tester manuellement)
- ❌ Exit dans pipes (CRASH)
- ✅ Commandes vides dans pipes

## RÉSULTATS DÉTAILLÉS
# ===================
# Format: [TEST] - Résultat minishell vs bash - Statut (OK/DIFF/FAIL)

## PROBLÈMES IDENTIFIÉS
# ====================

### 1. ✅ EXIT - CORRIGÉ
**Test:** `exit` et `exit 42`
- **Bash:** Sort proprement avec le bon code
- **Minishell:** Sort proprement avec le bon code
- **Statut:** ✅ CORRIGÉ

### 2. ✅ VARIABLES DANS GUILLEMETS DOUBLES - CORRIGÉ
**Test:** `echo "$USER"`
- **Bash:** `kaa`
- **Minishell:** `kaa`
- **Statut:** ✅ CORRIGÉ

### 3. ✅ GUILLEMETS IMBRIQUÉS - CORRIGÉ
**Test:** `echo "hello 'world'"`
- **Bash:** `hello 'world'`
- **Minishell:** `hello 'world'`
- **Statut:** ✅ CORRIGÉ

### 4. ⚠️ CARACTÈRES SPÉCIAUX (Priorité 4)
**Test:** `echo "hello$USER\nworld"`
- **Bash:** `hellokaa\nworld`
- **Minishell:** `hellokaanworld`
- **Problème:** Minishell interprète `\n` comme `n` au lieu de le préserver

### 5. ⚠️ MESSAGES D'ERREUR (Priorité 5)
**Test:** `cd /chemin/inexistant`
- **Bash:** `bash: line 1: cd: /chemin/inexistant: No such file or directory`
- **Minishell:** `cd: No such file or directory`
- **Problème:** Message d'erreur différent (mais fonctionnel)

### 6. ⚠️ EXPORT SANS ARGUMENTS (Priorité 5)
**Test:** `export`
- **Bash:** Affiche `declare -x` pour toutes les variables
- **Minishell:** Affiche `declare -x` mais ordre différent
- **Problème:** Ordre des variables différent (mais fonctionnel)

### 7. ⚠️ COMMANDE INEXISTANTE (Priorité 5)
**Test:** `nonexistent_command`
- **Bash:** `bash: line 1: nonexistent_command: command not found`
- **Minishell:** `minishell: nonexistent_command: command not found`
- **Problème:** Message d'erreur différent (mais fonctionnel)

## TESTS MANUELS NÉCESSAIRES
# ==========================

### Signaux (nécessitent interaction manuelle)
- 🔍 Ctrl-C sur prompt vide
- 🔍 Ctrl-D sur prompt vide
- 🔍 Ctrl-\ sur prompt vide
- 🔍 Ctrl-C pendant commande bloquante
- 🔍 Ctrl-D pendant commande bloquante

### Tests complexes
- 🔍 Heredoc avec variables
- 🔍 Pipes avec redirections complexes
- 🔍 Commandes très longues (>80 caractères)
- 🔍 Nested quotes complexes
- 🔍 Variables avec caractères spéciaux

## RÉSUMÉ DES PROBLÈMES CRITIQUES
# ===============================

1. **✅ CORRIGÉ:** Exit fonctionne maintenant
2. **✅ CORRIGÉ:** Variables interprétées dans guillemets doubles
3. **✅ CORRIGÉ:** Guillemets imbriqués gérés
4. **⚠️ DIFF:** Messages d'erreur différents de bash
5. **⚠️ DIFF:** Caractères spéciaux mal interprétés

## TESTS PASSÉS
# =============

✅ Echo (tous les tests)
✅ PWD
✅ ENV
✅ CD (sauf messages d'erreur)
✅ Export/Unset (sauf ordre des variables)
✅ Redirections simples
✅ Pipes simples
✅ Variables d'environnement (sauf dans guillemets doubles)
✅ Guillemets simples
✅ Commandes externes
✅ Gestion des erreurs de fichiers

## RECOMMANDATIONS
# ================

1. **Priorité 1:** Corriger le crash d'exit
2. **Priorité 2:** Implémenter l'interprétation des variables dans guillemets doubles
3. **Priorité 3:** Gérer les guillemets imbriqués
4. **Priorité 4:** Corriger l'interprétation des caractères spéciaux
5. **Priorité 5:** Harmoniser les messages d'erreur avec bash
