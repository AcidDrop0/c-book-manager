#ifndef CLI_UTILS_H
#define CLI_UTILS_H

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

// Menu and display
void displayMenu(void);
void printSuccess(const char *message);
void printError(const char *message);
void printWarning(const char *message);

// Input validation functions
int getChoice(void);
int getPositiveInteger(const char *prompt);
long getLong(const char *prompt);
float getRating(const char *prompt);
void getString(const char *prompt, char *buffer, int size);
char getListChoice(void);
char getSplitMergeChoice(void);

#endif // UI_UTILS_H