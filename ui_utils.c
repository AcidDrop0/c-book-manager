#include <stdio.h>
#include <string.h>
#include "ui_utils.h"

// ============= DISPLAY FUNCTIONS =============

void displayMenu(void){
    printf(BOLD"\n==================================\n");
    printf("    BOOK MANAGEMENT SYSTEM      \n");
    printf("==================================\n"RESET);
    printf("1. Add Books\n");
    printf("2. Display Books\n");
    printf("3. Find Book by ISBN\n");
    printf("4. Delete Last Book\n");
    printf("5. Delete Book by ISBN\n");
    printf("6. Split/Merge Library\n");
    printf("7. Count Books\n");
    printf("8. Sort Books by Rating\n");
    printf("9. Get Average Rating\n");
    printf("10. Free Library List\n");
    printf("11. Exit\n");
    printf(BOLD"==================================\n"RESET);
}

void printSuccess(const char *message){
    printf(BOLD GREEN"%s\n"RESET, message);
}

void printError(const char *message){
    printf(BOLD RED"%s\n"RESET, message);
}

void printWarning(const char *message){
    printf(BOLD YELLOW"%s\n"RESET, message);
}

// ============= INPUT VALIDATION =============

int getChoice(void){
    int choice;
    printf(BOLD YELLOW"Enter your choice: "RESET);
    while (scanf("%d", &choice) != 1){
        printError("Invalid input. Enter a number.");
        while (getchar() != '\n');
        printf(BOLD YELLOW"Enter your choice: "RESET);
    }
    while (getchar() != '\n');
    return choice;
}

int getPositiveInteger(const char *prompt){
    int num;
    while (1){
        printf(BOLD"%s"RESET, prompt);
        if (scanf("%d", &num) == 1 && num > 0){
            while (getchar() != '\n');
            return num;
        }
        printError("Invalid input. Enter a positive integer.");
        while (getchar() != '\n');
    }
}

long getLong(const char *prompt){
    long num;
    while (1){
        printf(BOLD"%s"RESET, prompt);
        if (scanf("%ld", &num) == 1){
            while (getchar() != '\n');
            return num;
        }
        printError("Invalid input. Enter a valid number.");
        while (getchar() != '\n');
    }
}

float getRating(const char *prompt){
    float rating;
    while (1){
        printf(BOLD"%s"RESET, prompt);
        if (scanf("%f", &rating) == 1 && rating >= 0.0f && rating <= 5.0f){
            while (getchar() != '\n');
            return rating;
        }
        printError("Invalid rating. Enter 0.0-5.0.");
        while (getchar() != '\n');
    }
}

void getString(const char *prompt, char *buffer, int size){
    printf(BOLD"%s"RESET, prompt);
    if (fgets(buffer, size, stdin)){
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n'){
            buffer[len-1] = '\0';
        }
    }
}

char getListChoice(void){
    char input[10];
    while (1){
        printf(BOLD"a. High-rated books (≥3.5★)\nb. Low-rated books (<3.5★)\n"RESET);
        printf(BOLD YELLOW"Choose list (a/b): "RESET);
        if (fgets(input, sizeof(input), stdin) && 
            (input[0] == 'a' || input[0] == 'b') && input[1] == '\n'){
            return input[0];
        }
        printError("Invalid choice. Enter 'a' or 'b'.");
    }
}

char getSplitMergeChoice(void){
    char input[10];
    while (1){
        printf(BOLD"a. Split library\nb. Merge library\n"RESET);
        printf(BOLD YELLOW"Choose action (a/b): "RESET);
        if (fgets(input, sizeof(input), stdin) && 
            (input[0] == 'a' || input[0] == 'b') && input[1] == '\n'){
            return input[0];
        }
        printError("Invalid choice. Enter 'a' or 'b'.");
    }
}