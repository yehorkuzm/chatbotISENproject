# Documentation d'Installation et d'Utilisation
## Hotel Simulator Chatbot

### 1. PRÉREQUIS

#### Logiciels Nécessaires
- **Compilateur GCC** (MinGW pour Windows)
  - Version minimale: GCC 7.0 ou supérieur
  - Télécharger: https://www.mingw-w64.org/
- **CMake** 
  - Version minimale: 3.10
  - Télécharger: https://cmake.org/download/
- **Git** (optionnel, pour le contrôle de version)

#### Configuration Système
- Système d'exploitation: Windows 10/11, Linux, macOS
- Espace disque: 50 MB minimum
- Mémoire RAM: 512 MB minimum

### 2. INSTALLATION

#### Méthode 1: Installation via CMake (Recommandée)

```bash
# 1. Cloner ou télécharger le projet
git clone <repository-url>
cd chatbotISENproject

# 2. Configure
cmake -S . -B build -G "MinGW Makefiles"

# 3. Build
cmake --build build

# 4. Run
build\src\hotel_simulator.exe
```

#### Méthode 2: Compilation Manuelle

```bash
# Depuis la racine du projet
gcc -o hotel_simulator src/main.c src/database.c src/utils.c src/player.c src/game.c src/chatbot.c src/admin.c -Iinclude

# Exécuter
hotel_simulator
```

#### Vérification de l'Installation

```bash
# Tester l'exécutable
build\src\hotel_simulator.exe

# Devrait afficher:
# === HOTEL SIMULATOR ===
# 1. Create a new player
# ...
```

### 3. STRUCTURE DES FICHIERS

```
chatbotISENproject/
├── db/                      # Base de données
│   ├── players.txt          # Liste des joueurs
│   ├── responses.txt        # Réponses du chatbot
│   └── history_*.txt        # Historiques des conversations
├── include/                 # Fichiers d'en-tête (.h)
│   ├── admin.h
│   ├── chatbot.h
│   ├── database.h
│   ├── game.h
│   ├── player.h
│   └── utils.h
├── src/                     # Code source (.c)
│   ├── admin.c
│   ├── chatbot.c
│   ├── database.c
│   ├── game.c
│   ├── main.c
│   ├── player.c
│   └── utils.c
├── build/                   # Fichiers de compilation
├── CMakeLists.txt          # Configuration CMake
└── README.md               # Documentation
```

### 4. GUIDE D'UTILISATION

#### 4.1 Premier Démarrage

1. **Lancer l'application**
   ```bash
   build\src\hotel_simulator.exe
   ```

2. **Créer un joueur**
   - Choisir option `1`
   - Entrer un nom unique
   - Le joueur commence avec 200€

3. **Démarrer le jeu**
   - Choisir option `3`
   - Entrer le nom du joueur créé

#### 4.2 Commandes de Jeu

| Commande | Description | Exemple |
|----------|-------------|---------|
| `help` | Affiche l'aide | `help` |
| `work`, `job` | Chercher du travail | `I need work` |
| `reception` | Travail réception (+150€) | `work at reception` |
| `cleaning` | Travail ménage (+100€) | `cleaning rooms` |
| `kitchen` | Travail cuisine (+120€) | `help in kitchen` |
| `maintenance` | Travail maintenance (+130€) | `fix something` |
| `balance` | Voir votre argent | `check balance` |
| `pay rent` | Payer le loyer (-200€) | `pay rent` |
| `explore` | Explorer l'hôtel | `look around` |
| `eat` | Manger (-15€) | `I'm hungry` |
| `rest` | Se reposer | `sleep` |
| `quit` | Quitter et sauvegarder | `quit` |

#### 4.3 Mode Administrateur

Accéder au mode admin depuis le menu principal (option 4):

```
=== ADMIN MODE ===
1. Add new interaction      # Ajouter une réponse
2. Edit interaction         # Modifier une réponse
3. Delete interaction       # Supprimer une réponse
4. List all interactions    # Lister toutes les réponses
5. Delete player           # Supprimer un joueur
6. View player history     # Voir l'historique
7. Clear player history    # Effacer l'historique
0. Back to main menu       # Retour
```

**Exemple: Ajouter une Interaction**
```
Choice: 1
Enter keywords (comma-separated): casino,gamble,bet
Enter response: You visit the casino. You win 50€!
Enter balance change: 50
```

#### 4.4 Gestion des Données

**Sauvegardes Automatiques:**
- L'état du joueur est sauvegardé après chaque action
- L'historique des conversations est enregistré en temps réel

**Fichiers de Données:**
- `db/players.txt`: Base de joueurs
  ```
  john 350.00
  marie 180.00
  ```
- `db/responses.txt`: Interactions du chatbot
  ```
  work,job|Let me find work for you...|0
  reception|You work at reception!|150
  ```
- `db/history_john.txt`: Historique de conversation
  ```
  john: I need work
  Narrator: Let me check available positions...
  ```

### 5. RÉSOLUTION DE PROBLÈMES

#### Problème: "Player not found"
**Solution:** Créer d'abord un joueur via l'option 1 du menu principal

#### Problème: "Error opening responses file"
**Solution:** 
```bash
# Vérifier que le fichier existe
ls db/responses.txt

# Créer le dossier db si nécessaire
mkdir db
```

#### Problème: Compilation échoue
**Solution:**
```bash
# Nettoyer et recompiler
rm -rf build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

#### Problème: "Balance is 0, GAME OVER"
**Solution:** Recommencer avec un nouveau joueur ou modifier le solde dans `db/players.txt`

