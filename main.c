#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "book.h"
#include "ui_utils.h"


// ============= COMMAND HANDLER PROTOTYPES =============

static void handleAddBooks(Library *lib);
static void handleDisplay(Library *lib);
static void handleFind(Library *lib);
static void handleDeleteLast(Library *lib);
static void handleDeleteByISBN(Library *lib);
static void handleSplitMerge(Library *lib);
static void handleCount(Library *lib);
static void handleSort(Library *lib);
static void handleAverage(Library *lib);
static void handleFreeList(Library *lib);


// ============= MAIN FUNCTION =============

int main(void){
    Library *lib = createLibrary();
    if (!lib){
        printError("Failed to initialize library. Exiting.");
        return EXIT_FAILURE;
    }

    printSuccess("Book Management System initialized!");

    while (1){
        sleep(1);
        displayMenu();
        int choice = getChoice();

        switch (choice){
            case 1:  handleAddBooks(lib); break;
            case 2:  handleDisplay(lib); break;
            case 3:  handleFind(lib); break;
            case 4:  handleDeleteLast(lib); break;
            case 5:  handleDeleteByISBN(lib); break;
            case 6:  handleSplitMerge(lib); break;
            case 7:  handleCount(lib); break;
            case 8:  handleSort(lib); break;
            case 9:  handleAverage(lib); break;
            case 10: handleFreeList(lib); break;
            case 11:
                printWarning("Cleaning up and exiting...");
                destroyLibrary(lib);
                return EXIT_SUCCESS;
            default:
                printError("Invalid choice. Try again.");
        }
    }
}


// ============= COMMAND HANDLERS =============

static void handleAddBooks(Library *lib){
    if (lib->is_split){
        printError("Cannot add books while library is split. Merge first.");
        return;
    }

    int num = getPositiveInteger("Number of books to add: ");
    
    for (int i = 1; i <= num; i++){
        char title[MAXNAME], author[MAXNAME];
        printf(BOLD"\n--- Book %d ---\n"RESET, i);
        
        getString("Title: ", title, MAXNAME);
        getString("Author: ", author, MAXNAME);
        long isbn = getLong("ISBN: ");
        float rating = getRating("Rating (0.0-5.0): ");
        
        if (addBook(lib, title, author, isbn, rating)){
            printSuccess("Book added successfully!");
        }
        else{
            printWarning("Retrying this book...");
            i--;
        }
    }
}

static void handleDisplay(Library *lib){
    if (lib->is_split){
        char choice = getListChoice();
        if (choice == 'a'){
            displayBooks(lib->high_rated, "High-Rated Books");
        }
        else{
            displayBooks(lib->low_rated, "Low-Rated Books");
        }
    }
    else{
        displayBooks(lib->main_list, "All Books");
    }
}

static void handleFind(Library *lib){
    Book *list = lib->is_split ? 
        getCurrentList(lib, getListChoice()) : lib->main_list;
    
    if (!list){
        printError("Selected list is empty.");
        return;
    }
    
    long isbn = getLong("Enter ISBN to search: ");
    findBook(list, isbn);
}

static void handleDeleteLast(Library *lib){
    deleteLastAddedBook(lib);
}

static void handleDeleteByISBN(Library *lib){
    long isbn = getLong("Enter ISBN to delete: ");
    
    if (lib->is_split){
        char choice = getListChoice();
        deleteBookByISBN(getCurrentListPtr(lib, choice), isbn);
    }
    else{
        deleteBookByISBN(&lib->main_list, isbn);
    }
}

static void handleSplitMerge(Library *lib){
    char choice = getSplitMergeChoice();
    
    if (choice == 'a'){
        splitLibrary(lib);
    }
    else{
        mergeLibrary(lib);
    }
}

static void handleCount(Library *lib){
    int count;
    const char *list_name;
    
    if (lib->is_split){
        char choice = getListChoice();
        if (choice == 'a'){
            count = countBooks(lib->high_rated);
            list_name = "high-rated";
        }
        else{
            count = countBooks(lib->low_rated);
            list_name = "low-rated";
        }
    }
    else{
        count = countBooks(lib->main_list);
        list_name = "total";
    }
    
    printf(BOLD"Number of %s books: %d\n"RESET, list_name, count);
}

static void handleSort(Library *lib){
    if (lib->is_split){
        printWarning("Books are already sorted in split lists.");
    }
    else{
        sortByRating(&lib->main_list);
    }
}

static void handleAverage(Library *lib){
    Book *list = lib->is_split ? 
        getCurrentList(lib, getListChoice()) : lib->main_list;
    
    float avg = averageRating(list);
    
    if (avg == 0.0f){
        printError("No books in selected list.");
    }
    else{
        printf(BOLD"Average rating: %.2f★\n"RESET, avg);
        if (avg >= 4.0f){
            printSuccess("Excellent collection!");
        }
        else if (avg >= 3.0f){
            printWarning("Good collection.");
        }
        else{
            printf(BOLD"Collection needs improvement.\n"RESET);
        }
    }
}

static void handleFreeList(Library *lib){
    if (!lib->is_split){
        if (!lib->main_list){
            printError("No list to free.");
        }
        else{
            // Free main list manually
            Book *temp = lib->main_list;
            while (temp){
                Book *next = temp->next;
                free(temp);
                temp = next;
            }
            lib->main_list = NULL;
            printSuccess("Main list freed.");
        }
    }
    else{
        char choice = getListChoice();
        Book **listPtr = getCurrentListPtr(lib, choice);
        
        if (!*listPtr){
            printError("Selected list is already empty.");
        }
        else{
            Book *temp = *listPtr;
            while (temp){
                Book *next = temp->next;
                free(temp);
                temp = next;
            }
            *listPtr = NULL;
            printSuccess("Selected list freed.");
        }
        
        if (!lib->high_rated && !lib->low_rated){
            lib->is_split = 0;
        }
    }
}