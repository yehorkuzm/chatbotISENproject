#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/admin.h"
#include "../include/utils.h"

#define RESPONSES_FILE "db/responses.txt"
#define PLAYERS_FILE "db/players.txt"
#define MAX_LINE 1024

void admin_menu() {
    int choice;
    
    do {
        printf("\n=== ADMIN MODE ===\n");
        printf("1. Add new interaction\n");
        printf("2. Edit interaction\n");
        printf("3. Delete interaction\n");
        printf("4. List all interactions\n");
        printf("5. Delete player\n");
        printf("0. Back to main menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_interaction();
                break;
            case 2:
                edit_interaction();
                break;
            case 3:
                delete_interaction();
                break;
            case 4:
                list_interactions();
                break;
            case 5:
                delete_player_admin();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

void list_interactions() {
    FILE *file = fopen(RESPONSES_FILE, "r");
    if (!file) {
        printf("Error opening responses file.\n");
        return;
    }

    char line[MAX_LINE];
    int index = 1;
    
    printf("\n=== ALL INTERACTIONS ===\n");
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        char *keywords = strtok(line, "|");
        char *response = strtok(NULL, "|");
        char *balance = strtok(NULL, "|");
        
        if (keywords && response && balance) {
            printf("\n[%d] Keywords: %s\n", index++, keywords);
            printf("    Response: %.50s%s\n", response, strlen(response) > 50 ? "..." : "");
            printf("    Balance change: %s", balance);
        }
    }
    
    fclose(file);
    printf("\n");
}

void add_interaction() {
    char keywords[256], response[512];
    int balance_change;
    
    printf("\n=== ADD NEW INTERACTION ===\n");
    printf("Enter keywords (comma-separated): ");
    fgets(keywords, sizeof(keywords), stdin);
    keywords[strcspn(keywords, "\n")] = '\0';
    
    printf("Enter response: ");
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = '\0';
    
    printf("Enter balance change (can be negative): ");
    scanf("%d", &balance_change);
    clear_input_buffer();
    
    FILE *file = fopen(RESPONSES_FILE, "a");
    if (!file) {
        printf("Error opening responses file.\n");
        return;
    }
    
    fprintf(file, "\n%s|%s|%d\n", keywords, response, balance_change);
    fclose(file);
    
    printf("Interaction added successfully!\n");
}

void edit_interaction() {
    list_interactions();
    
    printf("\nEnter interaction number to edit: ");
    int target_index;
    scanf("%d", &target_index);
    clear_input_buffer();
    
    FILE *file = fopen(RESPONSES_FILE, "r");
    if (!file) {
        printf("Error opening responses file.\n");
        return;
    }
    
    char lines[100][MAX_LINE];
    int line_count = 0;
    int current_index = 0;
    int target_line = -1;
    
    while (fgets(lines[line_count], MAX_LINE, file)) {
        if (lines[line_count][0] != '#' && lines[line_count][0] != '\n') {
            current_index++;
            if (current_index == target_index) {
                target_line = line_count;
            }
        }
        line_count++;
    }
    fclose(file);
    
    if (target_line == -1) {
        printf("Invalid interaction number.\n");
        return;
    }
    
    char keywords[256], response[512];
    int balance_change;
    
    printf("Enter new keywords: ");
    fgets(keywords, sizeof(keywords), stdin);
    keywords[strcspn(keywords, "\n")] = '\0';
    
    printf("Enter new response: ");
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = '\0';
    
    printf("Enter new balance change: ");
    scanf("%d", &balance_change);
    clear_input_buffer();
    
    snprintf(lines[target_line], MAX_LINE, "%s|%s|%d\n", keywords, response, balance_change);
    
    file = fopen(RESPONSES_FILE, "w");
    for (int i = 0; i < line_count; i++) {
        fputs(lines[i], file);
    }
    fclose(file);
    
    printf("Interaction updated successfully!\n");
}

void delete_interaction() {
    list_interactions();
    
    printf("\nEnter interaction number to delete: ");
    int target_index;
    scanf("%d", &target_index);
    clear_input_buffer();
    
    FILE *file = fopen(RESPONSES_FILE, "r");
    if (!file) {
        printf("Error opening responses file.\n");
        return;
    }
    
    char lines[100][MAX_LINE];
    int line_count = 0;
    int current_index = 0;
    int target_line = -1;
    
    while (fgets(lines[line_count], MAX_LINE, file)) {
        if (lines[line_count][0] != '#' && lines[line_count][0] != '\n') {
            current_index++;
            if (current_index == target_index) {
                target_line = line_count;
            }
        }
        line_count++;
    }
    fclose(file);
    
    if (target_line == -1) {
        printf("Invalid interaction number.\n");
        return;
    }
    
    file = fopen(RESPONSES_FILE, "w");
    for (int i = 0; i < line_count; i++) {
        if (i != target_line) {
            fputs(lines[i], file);
        }
    }
    fclose(file);
    
    printf("Interaction deleted successfully!\n");
}

void delete_player_admin() {
    FILE *file = fopen(PLAYERS_FILE, "r");
    if (!file) {
        printf("Error opening players file.\n");
        return;
    }
    
    char lines[100][256];
    int line_count = 0;
    
    printf("\n=== ALL PLAYERS ===\n");
    while (fgets(lines[line_count], 256, file)) {
        char name[50];
        double balance;
        sscanf(lines[line_count], "%s %lf", name, &balance);
        printf("[%d] %s - %.2f €\n", line_count + 1, name, balance);
        line_count++;
    }
    fclose(file);
    
    if (line_count == 0) {
        printf("No players found.\n");
        return;
    }
    
    printf("\nEnter player number to delete: ");
    int target;
    scanf("%d", &target);
    clear_input_buffer();
    
    if (target < 1 || target > line_count) {
        printf("Invalid player number.\n");
        return;
    }
    
    file = fopen(PLAYERS_FILE, "w");
    for (int i = 0; i < line_count; i++) {
        if (i != target - 1) {
            fputs(lines[i], file);
        }
    }
    fclose(file);
    
    printf("Player deleted successfully!\n");
}