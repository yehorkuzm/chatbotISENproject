# Hotel Simulator Chatbot

A text-based hotel simulation game where players work various jobs to earn money and pay rent. Built in C with a chatbot-style interaction system.

## 📋 Features

- **Player Management**: Create and manage multiple player profiles
- **Conversation System**: Natural language chatbot interface
- **Job System**: Work different positions (reception, cleaning, kitchen, maintenance)
- **Economy**: Earn money, pay rent, and manage your balance
- **Persistent Storage**: Player data and conversation history saved to files
- **Admin Mode**: Add/edit/delete interactions and manage players
- **Chat History**: View past conversations and track progress

## 🎮 Gameplay

Players start with 200€ and must find work in the hotel to survive. Available jobs include:
- **Reception** (150€): Work at the front desk
- **Cleaning** (100€): Clean hotel rooms
- **Kitchen** (120€): Help in the restaurant
- **Maintenance** (130€): Fix hotel equipment

Pay 200€ weekly rent or face game over!

## 🛠️ Building the Project

### Prerequisites
- GCC compiler (MinGW on Windows)
- CMake 3.10 or higher

### Build Instructions

**Using CMake (Recommended):**
```bash
# Configure
cmake -S . -B build -G "MinGW Makefiles"

# Build
cmake --build build

# Run
build\src\hotel_simulator.exe
```

**Using GCC directly:**
```bash
gcc -o hotel_simulator src/main.c src/database.c src/utils.c src/player.c src/game.c src/chatbot.c src/admin.c -Iinclude
```

## 📁 Project Structure

```
chatbotISENproject/
├── include/           # Header files
│   ├── player.h
│   ├── database.h
│   ├── chatbot.h
│   ├── game.h
│   ├── admin.h
│   └── utils.h
├── src/              # Source files
│   ├── main.c
│   ├── player.c
│   ├── database.c
│   ├── chatbot.c
│   ├── game.c
│   ├── admin.c
│   └── utils.c
├── db/               # Database files
│   ├── players.txt
│   ├── responses.txt
│   └── history_*.txt
├── build/            # Build output
└── CMakeLists.txt
```

## 🎯 How to Play

### Main Menu
1. **Create a new player** - Register your character
2. **View all players** - See existing players
3. **Start game** - Begin playing
4. **Admin mode** - Manage game content (advanced)
0. **Quit** - Exit the application

### In-Game Commands
- `help` - Show available actions
- `work` or `job` - Find employment
- `reception`, `cleaning`, `kitchen`, `maintenance` - Work specific jobs
- `explore` or `look around` - Explore the hotel
- `balance` or `money` - Check your balance
- `pay rent` - Pay your weekly rent (200€)
- `eat` - Grab a meal (costs 15€)
- `rest` or `sleep` - Take a break
- `quit` or `exit` - Save and exit

## 🔧 Admin Mode

Admin mode allows you to customize the game:

1. **Add new interaction** - Create custom chatbot responses
2. **Edit interaction** - Modify existing interactions
3. **Delete interaction** - Remove interactions
4. **List all interactions** - View all chatbot responses
5. **Delete player** - Remove player from database
6. **View player history** - See conversation logs
7. **Clear player history** - Reset conversation history

### Adding Custom Interactions

Format: `KEYWORDS|RESPONSE|BALANCE_CHANGE`

Example:
```
gamble,casino,bet|You head to the hotel casino. You win 50€ at the blackjack table!|50
```

## 💾 Data Files

### players.txt
Stores player data:
```
playername 200.00
```

### responses.txt
Chatbot interaction database:
```
keyword1,keyword2|Response text here|balance_change
```

### history_playername.txt
Conversation logs for each player:
```
Player: hello
Narrator: Welcome to the hotel!
```
