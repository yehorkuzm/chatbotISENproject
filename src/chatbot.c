#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/chatbot.h"

static void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int init_chatbot(Chatbot *bot, const char *response_file) {
    bot->response_count = 0;

    FILE *file = fopen(response_file, "r");
    if (!file) {
        printf("Error: Could not open response file: %s\n", response_file);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && bot->response_count < MAX_RESPONSES) {
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        BotResponse *resp = &bot->responses[bot->response_count];
        resp->keyword_count = 0;
        resp->balance_change = 0.0;
        resp->response[0] = '\0';

        // Find the first pipe separator
        char *pipe1 = strchr(line, '|');
        if (!pipe1) continue;

        // Parse keywords (everything before first pipe)
        *pipe1 = '\0';
        char *keywords_str = line;
        char *keyword = strtok(keywords_str, ",");
        while (keyword && resp->keyword_count < MAX_KEYWORDS) {
            // Trim whitespace
            while (*keyword == ' ') keyword++;
            char *end = keyword + strlen(keyword) - 1;
            while (end > keyword && *end == ' ') end--;
            *(end + 1) = '\0';

            if (strlen(keyword) > 0) {
                strncpy(resp->keywords[resp->keyword_count], keyword, 49);
                resp->keywords[resp->keyword_count][49] = '\0';
                to_lowercase(resp->keywords[resp->keyword_count]);
                resp->keyword_count++;
            }

            keyword = strtok(NULL, ",");
        }

        // Find the second pipe separator
        char *pipe2 = strchr(pipe1 + 1, '|');

        if (pipe2) {
            // Parse response text (between first and second pipe)
            *pipe2 = '\0';
            char *response_text = pipe1 + 1;

            // Trim leading whitespace
            while (*response_text == ' ') response_text++;

            strncpy(resp->response, response_text, MAX_LINE_LENGTH - 1);
            resp->response[MAX_LINE_LENGTH - 1] = '\0';

            // Parse balance change (after second pipe)
            char *balance_str = pipe2 + 1;
            while (*balance_str == ' ') balance_str++;
            resp->balance_change = atof(balance_str);
        } else {
            // No balance change, just response text
            char *response_text = pipe1 + 1;
            while (*response_text == ' ') response_text++;

            strncpy(resp->response, response_text, MAX_LINE_LENGTH - 1);
            resp->response[MAX_LINE_LENGTH - 1] = '\0';
        }

        // Replace \n with actual newlines in response
        char temp[MAX_LINE_LENGTH];
        int j = 0;
        for (int i = 0; resp->response[i] && j < MAX_LINE_LENGTH - 1; i++) {
            if (resp->response[i] == '\\' && resp->response[i + 1] == 'n') {
                temp[j++] = '\n';
                i++;
            } else {
                temp[j++] = resp->response[i];
            }
        }
        temp[j] = '\0';
        strcpy(resp->response, temp);

        bot->response_count++;
    }

    fclose(file);
    printf("Chatbot initialized with %d responses.\n", bot->response_count);
    return 1;
}

int matches_keywords(const char *text, const BotResponse *response) {
    char text_lower[MAX_LINE_LENGTH];
    strncpy(text_lower, text, MAX_LINE_LENGTH - 1);
    text_lower[MAX_LINE_LENGTH - 1] = '\0';
    to_lowercase(text_lower);

    for (int i = 0; i < response->keyword_count; i++) {
        if (strstr(text_lower, response->keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

static void save_bot_response(const char *player_name, const char *response) {
    char history_path[256];
    snprintf(history_path, sizeof(history_path), "db/history_%s.txt", player_name);

    FILE *file = fopen(history_path, "a");
    if (file) {
        fprintf(file, "Narrator: %s\n", response);
        fclose(file);
    }
}

void process_input(Chatbot *bot, Player *player, const char *input) {
    int found = 0;

    // Check for balance command specially
    char input_lower[MAX_LINE_LENGTH];
    strncpy(input_lower, input, MAX_LINE_LENGTH - 1);
    input_lower[MAX_LINE_LENGTH - 1] = '\0';
    to_lowercase(input_lower);

    if (strstr(input_lower, "balance") || strstr(input_lower, "money") ||
        strstr(input_lower, "cash") || strstr(input_lower, "funds")) {
        char balance_msg[256];
        snprintf(balance_msg, sizeof(balance_msg), "Your current balance is: %.2f EUR", player->balance);
        printf("  Narrator: %s\n", balance_msg);
        save_bot_response(player->name, balance_msg);
        return;
    }

    for (int i = 0; i < bot->response_count; i++) {
        if (matches_keywords(input, &bot->responses[i])) {
            printf("  Narrator: %s\n", bot->responses[i].response);
            save_bot_response(player->name, bot->responses[i].response);

            if (bot->responses[i].balance_change != 0) {
                player->balance += bot->responses[i].balance_change;
                char balance_msg[256];
                snprintf(balance_msg, sizeof(balance_msg), "Your new balance: %.2f EUR", player->balance);
                printf("\n  Narrator: %s\n", balance_msg);
                save_bot_response(player->name, balance_msg);
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        const char *default_msg = "I'm not sure what you mean. Try asking about 'work', 'explore', or type 'help'.";
        printf("  Narrator: %s\n", default_msg);
        save_bot_response(player->name, default_msg);
    }
}

void cleanup_chatbot(Chatbot *bot) {
    bot->response_count = 0;
}
