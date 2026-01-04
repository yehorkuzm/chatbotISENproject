# Architecture Logicielle
## Hotel Simulator Chatbot

### 1. VUE D'ENSEMBLE

Le projet utilise une architecture modulaire en C, séparant les responsabilités en modules distincts pour faciliter la maintenance et l'évolution.

```
┌─────────────────────────────────────────────────┐
│                   main.c                        │
│            (Point d'entrée, Menu)               │
└─────────────────┬───────────────────────────────┘
                  │
        ┌─────────┼─────────┐
        │         │         │
        ▼         ▼         ▼
   ┌────────┐ ┌──────┐ ┌───────┐
   │ Player │ │ Game │ │ Admin │
   └────┬───┘ └───┬──┘ └───┬───┘
        │         │        │
        │    ┌────▼────┐   │
        │    │ Chatbot │   │
        │    └────┬────┘   │
        │         │        │
        └─────────┼────────┘
                  ▼
            ┌──────────┐
            │ Database │
            └─────┬────┘
                  │
            ┌─────▼─────┐
            │   Utils   │
            └───────────┘
```

### 2. MODULES ET COMPOSANTS

#### 2.1 Module Main (main.c)

**Rôle:** Point d'entrée de l'application, gestion du menu principal

**Fonctions Principales:**
```c
int main()
```
- Affiche le menu principal
- Gère les choix utilisateur (1-4, 0)
- Coordonne les appels aux autres modules

**Dépendances:**
- player.h, database.h, game.h, admin.h, utils.h

**Flux d'Exécution:**
```
Démarrage → Affichage Menu → Choix Utilisateur
    ↓
    ├─ 1: Créer joueur → database
    ├─ 2: Liste joueurs → database
    ├─ 3: Démarrer jeu → game
    ├─ 4: Mode admin → admin
    └─ 0: Quitter
```

---

#### 2.2 Module Player (player.c/h)

**Rôle:** Gestion des données et opérations sur les joueurs

**Structures:**
```c
typedef struct {
    char name[MAX_NAME_LENGTH];      // Nom du joueur
    double balance;                   // Solde actuel
    ChatEntry history[MAX_HISTORY];   // Historique conversations
    int history_count;                // Nombre d'entrées
} Player;

typedef struct {
    char message[MAX_MESSAGE_LENGTH];    // Message joueur
    char response[MAX_MESSAGE_LENGTH];   // Réponse chatbot
    double balance_before;               // Solde avant
    double balance_after;                // Solde après
} ChatEntry;
```

**Fonctions Principales:**

| Fonction | Description | Paramètres | Retour |
|----------|-------------|------------|--------|
| `create_player()` | Crée un nouveau joueur | name, balance | Player |
| `add_to_history()` | Ajoute entrée historique | player*, message, response, balances | void |
| `display_history()` | Affiche l'historique | player* | void |
| `save_history_to_file()` | Sauvegarde historique | player* | int (success) |
| `load_history_from_file()` | Charge historique | player* | int (success) |
| `clear_history()` | Efface l'historique | player* | void |

**Exemple d'Utilisation:**
```c
// Créer un joueur
Player p = create_player("John", 200.0);

// Ajouter à l'historique
add_to_history(&p, "hello", "Welcome!", 200.0, 200.0);

// Sauvegarder
save_history_to_file(&p);
```

---

#### 2.3 Module Database (database.c/h)

**Rôle:** Persistance des données (joueurs, réponses)

**Constantes:**
```c
#define PLAYER_DB_DIR "db"
#define PLAYER_DB_FILE "players.txt"
#define RESPONSES_FILE "responses.txt"
```

**Fonctions Principales:**

| Fonction | Description | Complexité | Fichier |
|----------|-------------|------------|---------|
| `save_player_to_db()` | Sauvegarde joueur | O(1) | players.txt |
| `load_player_from_db()` | Charge joueur | O(n) | players.txt |
| `player_exists()` | Vérifie existence | O(n) | players.txt |
| `list_all_players()` | Liste tous joueurs | O(n) | players.txt |
| `get_responses_path()` | Chemin responses | O(1) | - |

**Format des Fichiers:**

**players.txt:**
```
nom_joueur solde
john 350.00
marie 180.00
```

**responses.txt:**
```
keywords|response|balance_change
work,job|Looking for work...|0
reception|You work reception!|150
```

**Algorithme de Recherche:**
```c
// O(n) - recherche linéaire
int player_exists(const char *name) {
    FILE *file = fopen(DB_FILE, "r");
    Player temp;
    while (fscanf(file, "%s %lf", temp.name, &temp.balance) == 2) {
        if (strcmp(temp.name, name) == 0)
            return 1;  // Trouvé
    }
    return 0;  // Non trouvé
}
```

---

#### 2.4 Module Chatbot (chatbot.c/h)

**Rôle:** Traitement du langage naturel et réponses

**Structures:**
```c
typedef struct {
    char keywords[MAX_KEYWORDS][50];  // Mots-clés
    int keyword_count;                // Nombre de mots-clés
    char response[MAX_LINE_LENGTH];   // Réponse à donner
    double balance_change;            // Changement de solde
} BotResponse;

typedef struct {
    BotResponse responses[MAX_RESPONSES];
    int response_count;
} Chatbot;
```

**Fonctions Principales:**

| Fonction | Description | Algorithme |
|----------|-------------|------------|
| `init_chatbot()` | Initialise le chatbot | Parse responses.txt |
| `process_input()` | Traite entrée utilisateur | Recherche mots-clés |
| `matches_keywords()` | Vérifie correspondance | String matching |
| `cleanup_chatbot()` | Libère ressources | Réinitialisation |
| `save_bot_response()` | Sauvegarde réponse | Écriture fichier |

**Algorithme de Correspondance:**
```c
// Recherche de mots-clés (case-insensitive)
int matches_keywords(const char *text, const BotResponse *response) {
    char text_lower[MAX_LINE_LENGTH];
    to_lowercase(strcpy(text_lower, text));
    
    for (int i = 0; i < response->keyword_count; i++) {
        if (strstr(text_lower, response->keywords[i]) != NULL)
            return 1;  // Correspondance trouvée
    }
    return 0;
}
```

**Flux de Traitement:**
```
Input → to_lowercase() → matches_keywords() → Response
                              ↓
                        balance_change
                              ↓
                        save_to_history
```

---

#### 2.5 Module Game (game.c/h)

**Rôle:** Gestion de la boucle de jeu principale

**Fonctions Principales:**

| Fonction | Description |
|----------|-------------|
| `start_game()` | Boucle principale du jeu |
| `save_message_to_history()` | Enregistre message |
| `display_conversation_history()` | Affiche historique |

**Boucle de Jeu:**
```c
void start_game() {
    // 1. Charger joueur
    load_player_from_db(name, &player);
    
    // 2. Afficher historique ou bienvenue
    display_conversation_history(player.name);
    
    // 3. Initialiser chatbot
    init_chatbot(&bot, responses_path);
    
    // 4. Boucle principale
    while (game_running) {
        // Lire input
        fgets(input, sizeof(input), stdin);
        
        // Traiter
        if (strcmp(input, "quit") == 0)
            break;
        
        process_input(&bot, &player, input);
        
        // Vérifier game over
        if (player.balance <= 0)
            game_running = 0;
    }
    
    // 5. Sauvegarder et nettoyer
    save_player_to_db(&player);
    cleanup_chatbot(&bot);
}
```

**États du Jeu:**
```
┌─────────┐
│  INIT   │ → Charger joueur
└────┬────┘
     ↓
┌─────────┐
│ PLAYING │ ← Boucle principale
└────┬────┘
     ↓
┌─────────┐
│GAME OVER│ → Balance = 0
└─────────┘
```

---

#### 2.6 Module Admin (admin.c/h)

**Rôle:** Gestion administrative du système

**Fonctions Principales:**

| Fonction | Description | Action |
|----------|-------------|--------|
| `admin_menu()` | Menu administrateur | Interface |
| `add_interaction()` | Ajouter réponse | Append responses.txt |
| `edit_interaction()` | Modifier réponse | Update responses.txt |
| `delete_interaction()` | Supprimer réponse | Remove from file |
| `list_interactions()` | Liste réponses | Read responses.txt |
| `delete_player_admin()` | Supprimer joueur | Remove from players.txt |


**Algorithme de Modification:**
```c
void edit_interaction() {
    // 1. Lire toutes les lignes en mémoire
    char lines[100][MAX_LINE];
    int line_count = read_all_lines(file, lines);
    
    // 2. Modifier la ligne ciblée
    lines[target_line] = new_content;
    
    // 3. Réécrire tout le fichier
    write_all_lines(file, lines, line_count);
}
```

---

#### 2.7 Module Utils (utils.c/h)

**Rôle:** Fonctions utilitaires transversales

**Fonctions:**

| Fonction | Description | Usage |
|----------|-------------|-------|
| `clear_input_buffer()` | Vide buffer stdin | Après scanf() |

**Exemple:**
```c
// Nettoyer buffer après scanf
scanf("%d", &choice);
clear_input_buffer();  // Évite problèmes avec fgets()
```

---

### 3. FLUX DE DONNÉES

#### 3.1 Création de Joueur
```
User Input (main.c)
    ↓
create_player() (player.c)
    ↓
save_player_to_db() (database.c)
    ↓
db/players.txt
```

#### 3.2 Interaction de Jeu
```
User Input (game.c)
    ↓
process_input() (chatbot.c)
    ↓
matches_keywords() (chatbot.c)
    ↓
update balance (player.c)
    ↓
save_bot_response() (chatbot.c)
    ↓
db/history_*.txt
```

#### 3.3 Sauvegarde
```
Player Data (memory)
    ↓
save_player_to_db() (database.c)
    ↓
db/players.txt

save_history_to_file() (player.c)
    ↓
db/history_*.txt
```

### 4. DIAGRAMMES

#### 4.1 Diagramme de Classes
```
┌──────────────┐
│    Player    │
├──────────────┤
│ - name       │
│ - balance    │
│ - history[]  │
├──────────────┤
│ + create()   │
│ + save()     │
│ + load()     │
└──────────────┘
       △
       │ uses
       │
┌──────────────┐
│   Chatbot    │
├──────────────┤
│ - responses[]│
├──────────────┤
│ + init()     │
│ + process()  │
└──────────────┘
```

#### 4.2 Diagramme de Séquence (Interaction)
```
User    Game    Chatbot    Player    Database
 │       │         │         │          │
 ├─input>│         │         │          │
 │       ├────────>│         │          │
 │       │    process()      │          │
 │       │         ├──match─>│          │
 │       │         │         │          │
 │       │         ├─update─>│          │
 │       │         │    balance         │
 │       │         │         ├─save────>│
 │       │         │         │          │
 │<─response───────┤         │          │
```

### 5. CONVENTIONS DE CODE

#### Nommage
- **Fonctions:** `snake_case` (ex: `create_player()`)
- **Variables:** `snake_case` (ex: `player_name`)
- **Constantes:** `UPPER_CASE` (ex: `MAX_PLAYERS`)
- **Structures:** `PascalCase` (ex: `Player`)

#### Organisation
- Fichiers .h: Déclarations uniquement
- Fichiers .c: Implémentations
- Fonctions statiques: Préfixer avec `static`
