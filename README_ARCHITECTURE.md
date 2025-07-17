# Minishell - Architecture Propre

## 📁 Structure du Projet

```
minishell/
├── Makefile                    # Makefile principal avec organisation modulaire
├── minishell                   # Exécutable généré
├── include/                    # Headers
│   └── minishell.h             # Header principal avec tous les prototypes
├── src/                        # Code source organisé par modules
│   ├── core/                   # Noyau du shell
│   │   ├── minishell.c         # Point d'entrée principal
│   │   └── shell_utils.c       # Utilitaires du shell (boucle principale)
│   ├── parsing/                # Module de parsing et tokenisation
│   │   ├── tokenize.c          # Fonctions de base de tokenisation
│   │   ├── tokenize_utils.c    # Utilitaires de tokenisation
│   │   └── tokenize_commands.c # Parsing des commandes et pipes
│   ├── execution/              # Module d'exécution
│   │   ├── externals.c         # Exécution de commandes externes
│   │   ├── redirections.c      # Gestion des redirections
│   │   └── redirections_utils.c # Utilitaires de redirections
│   ├── builtins/               # Commandes intégrées
│   │   └── builtin.c           # Toutes les commandes built-in
│   ├── utils/                  # Utilitaires généraux
│   │   ├── utils.c             # Utilitaires de chemins et environnement
│   │   └── utils2.c            # Utilitaires supplémentaires
│   └── memory/                 # Gestion mémoire
│       └── cleaner.c           # Fonctions de nettoyage mémoire
├── libft/                      # Bibliothèque libft (non modifiée)
└── pipex/                      # Module pipex (non modifié)
```

## 🎯 Principes Architecturaux

### 1. **Séparation des Responsabilités**

- **Core** : Gestion du shell principal et signaux
- **Parsing** : Analyse lexicale et syntaxique
- **Execution** : Exécution de commandes et redirections
- **Builtins** : Commandes intégrées du shell
- **Utils** : Fonctions utilitaires (chemins, environnement)
- **Memory** : Gestion mémoire et nettoyage

### 2. **Header Unique**

- Un seul header principal `include/minishell.h`
- Tous les prototypes organisés par modules
- Includes système et de projet centralisés

### 3. **Makefile Modulaire**

- Sources organisées par variables selon les modules
- Compilation claire et maintenir
- Dependencies automatiques

## 🔧 Avantages de cette Architecture

### ✅ **Maintenabilité**

- Code organisé logiquement par fonction
- Facile à comprendre et naviguer
- Modifications isolées par module

### ✅ **Scalabilité**

- Ajout facile de nouveaux modules
- Extension simple des fonctionnalités
- Structure prête pour des projets plus complexes

### ✅ **Lisibilité**

- Fichiers de taille raisonnable (< 200 lignes)
- Responsabilités claires
- Nommage cohérent

### ✅ **Conformité**

- Respect de la norminette
- Headers normalisés
- Organisation professionnelle

## 🛠️ Compilation

```bash
make clean  # Nettoyer les fichiers objets
make        # Compiler le projet
make re     # Reconstruire complètement
```

## 📝 Normes Respectées

- ✅ Maximum 25 lignes par fonction
- ✅ Maximum 5 fonctions par fichier
- ✅ Headers norminette conformes
- ✅ Gestion mémoire optimisée
- ✅ Structure modulaire propre

Cette architecture garantit un code propre, maintenable et conforme aux standards de développement professionnel.
