#ifndef CHATBOT_H
#define CHATBOT_H

#include "player.h"

#define MAX_RESPONSES 100
#define MAX_KEYWORDS 50
#define MAX_LINE_LENGTH 512

typedef struct {
    char keywords[MAX_KEYWORDS][50];
    int keyword_count;
    char response[MAX_LINE_LENGTH];
    double balance_change;
} BotResponse;

typedef struct {
    BotResponse responses[MAX_RESPONSES];
    int response_count;
} Chatbot;

// Initialize chatbot and load responses from file
int init_chatbot(Chatbot *bot, const char *response_file);

// Process player input and generate response
void process_input(Chatbot *bot, Player *player, const char *input);

// Check if text contains any of the keywords
int matches_keywords(const char *text, const BotResponse *response);

// Free chatbot resources
void cleanup_chatbot(Chatbot *bot);

#endif