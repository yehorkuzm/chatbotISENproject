#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/chatbot.h"
#include "../include/database.h"

static void save_message_to_history(const char *player_name, const char *sender, const char *message) {
    char history_path[256];
    snprintf(history_path, sizeof(history_path), "db/history_%s.txt", player_name);

    FILE *file = fopen(history_path, "a");
    if (!file) {
        printf("Warning: Could not save message to history.\n");
        return;
    }

    fprintf(file, "%s: %s\n", sender, message);
    fclose(file);
}

static void display_commands(void) {
    printf("\n=== AVAILABLE COMMANDS ===\n");
    printf("  Jobs:   reception, cleaning, kitchen, maintenance\n");
    printf("  Money:  balance, pay rent\n");
    printf("  Other:  explore, rest, eat, help\n");
    printf("  Toggle: hide / show (commands visibility)\n");
    printf("  Exit:   quit\n");
    printf("===========================\n");
}

static void display_conversation_history(const char *player_name) {
    char history_path[256];
    snprintf(history_path, sizeof(history_path), "db/history_%s.txt", player_name);

    FILE *file = fopen(history_path, "r");
    if (!file) {
        // No history exists, show welcome message
        printf("\n=== WELCOME TO HOTEL GRAND ISEN ===\n");
        printf("Narrator: Welcome, %s! You've just arrived at the Hotel Grand ISEN.\n", player_name);
        printf("Narrator: You're living in room 204, but your savings are running low.\n");
        printf("Narrator: You need to find work in the hotel to pay your rent and survive.\n");
        printf("Narrator: Talk to me about what you want to do. Type 'quit' to exit the game.\n");
        display_commands();
        printf("\n");

        // Save welcome message to history
        save_message_to_history(player_name, "Narrator", "Welcome! You've just arrived at the Hotel Grand ISEN.");
        save_message_to_history(player_name, "Narrator", "You're living in room 204, but your savings are running low.");
        save_message_to_history(player_name, "Narrator", "You need to find work in the hotel to pay your rent and survive.");
        save_message_to_history(player_name, "Narrator", "Talk to me about what you want to do. Type 'quit' to exit the game.");
        return;
    }

    printf("\n=== CONVERSATION HISTORY ===\n");
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("=== END OF HISTORY ===\n\n");
    printf("Narrator: Welcome back, %s!\n", player_name);
    fclose(file);
}

void start_game() {
    char player_name[50];
    printf("\nEnter your player name to start the game: ");
    fgets(player_name, sizeof(player_name), stdin);
    player_name[strcspn(player_name, "\n")] = '\0';

    Player player;

    // Check if player exists in database
    if (player_exists(player_name)) {
        if (load_player_from_db(player_name, &player)) {
            printf("Welcome back, %s! Your balance is %.2f EUR\n", player.name, player.balance);
        } else {
            printf("Error loading player data.\n");
            return;
        }
    } else {
        printf("Player '%s' not found in database.\n", player_name);
        printf("Please create a player first from the main menu.\n");
        return;
    }

    display_conversation_history(player.name);

    // Always show commands at game start
    display_commands();
    printf("\n");

    // Get responses file path from database module
    char responses_path[256];
    get_responses_path(responses_path, sizeof(responses_path));

    Chatbot bot;
    if (!init_chatbot(&bot, responses_path)) {
        printf("Error: Failed to initialize chatbot. Make sure %s exists.\n", responses_path);
        return;
    }

    char input[256];
    int game_running = 1;
    int show_commands = 1;  // Commands visible by default

    while (game_running) {
        printf("%s: ", player.name);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("Narrator: Goodbye, %s! Come back soon.\n", player.name);
            save_message_to_history(player.name, player.name, input);
            save_message_to_history(player.name, "Narrator", "Goodbye! Come back soon.");
            game_running = 0;
        } else if (strcmp(input, "hide") == 0 || strcmp(input, "hide commands") == 0) {
            show_commands = 0;
            printf("Narrator: Commands hidden. Type 'show' or 'help' to see them again.\n");
            save_message_to_history(player.name, player.name, input);
            save_message_to_history(player.name, "Narrator", "Commands hidden.");
        } else if (strcmp(input, "show") == 0 || strcmp(input, "show commands") == 0) {
            show_commands = 1;
            printf("Narrator: Commands will now be shown after each action.\n");
            display_commands();
            save_message_to_history(player.name, player.name, input);
            save_message_to_history(player.name, "Narrator", "Commands shown.");
        } else {
            save_message_to_history(player.name, player.name, input);
            process_input(&bot, &player, input);
            
            // Show commands after each action if enabled
            if (show_commands) {
                display_commands();
            }
        }

        if (player.balance <= 0) {
            printf("\nNarrator: Oh no! You've run out of money and can't afford your room anymore.\n");
            printf("Narrator: GAME OVER.\n");
            save_message_to_history(player.name, "Narrator", "Oh no! You've run out of money and can't afford your room anymore. GAME OVER.");
            game_running = 0;
        }
    }

    cleanup_chatbot(&bot);

    // Save player's final balance to database
    update_player_in_db(&player);
    printf("Progress saved.\n");
}
