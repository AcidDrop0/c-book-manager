#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "ui_utils.h"

// ============= LIBRARY MANAGEMENT =============

Library* createLibrary(void){
    Library *lib = malloc(sizeof(Library));
    if (!lib){
        printError("Failed to create library!");
        return NULL;
    }
    lib->main_list = NULL;
    lib->high_rated = NULL;
    lib->low_rated = NULL;
    lib->last_added = NULL;  // Initialize last_added tracker
    lib->is_split = 0;
    return lib;
}

static void freeBookList(Book *head){
    while (head){
        Book *temp = head;
        head = head->next;
        free(temp);
    }
}

void destroyLibrary(Library *lib){
    if (!lib) return;
    freeBookList(lib->main_list);
    freeBookList(lib->high_rated);
    freeBookList(lib->low_rated);
    free(lib);
}

// ============= HELPER FUNCTIONS =============

Book* getCurrentList(Library *lib, char choice){
    if (choice == 'a') return lib->high_rated;
    if (choice == 'b') return lib->low_rated;
    return lib->main_list;
}

Book** getCurrentListPtr(Library *lib, char choice){
    if (choice == 'a') return &lib->high_rated;
    if (choice == 'b') return &lib->low_rated;
    return &lib->main_list;
}

// ============= BOOK OPERATIONS =============

static int isbnExists(Book *head, long isbn){
    while (head){
        if (head->isbn == isbn) return 1;
        head = head->next;
    }
    return 0;
}

Book* addBook(Library *lib, const char *title, const char *author, long isbn, float rating){
    if (!lib || lib->is_split){
        printError("Cannot add books while library is split!");
        return NULL;
    }

    if (isbnExists(lib->main_list, isbn)){
        printf(BOLD RED"Book with ISBN %ld already exists!\n"RESET, isbn);
        return NULL;
    }

    Book *newBook = malloc(sizeof(Book));
    if (!newBook){
        printError("Memory allocation failed!");
        return NULL;
    }

    strncpy(newBook->title, title, MAXNAME - 1);
    newBook->title[MAXNAME - 1] = '\0';
    strncpy(newBook->author, author, MAXNAME - 1);
    newBook->author[MAXNAME - 1] = '\0';
    newBook->isbn = isbn;
    newBook->rating = rating;
    newBook->next = NULL;

    if (!lib->main_list){
        lib->main_list = newBook;
    }
    else{
        Book *temp = lib->main_list;
        while (temp->next) temp = temp->next;
        temp->next = newBook;
    }

    lib->last_added = newBook;  // Track the most recently added book
    return newBook;
}

void displayBooks(Book *head, const char *list_name){
    if (!head){
        printf(BOLD RED"No books in %s.\n"RESET, list_name);
        return;
    }

    printf(BOLD BLUE"\n=== %s ===\n"RESET, list_name);
    int num = 1;
    while (head){
        printf(BOLD"%d. Title: %s\n"RESET, num, head->title);
        printf("   Author: %s | ISBN: %ld | Rating: %.1f★\n", 
               head->author, head->isbn, head->rating);
        head = head->next;
        num++;
    }
}

Book* findBook(Book *head, long isbn){
    while (head){
        if (head->isbn == isbn){
            printSuccess("Book found!");
            printf("Title: %s\nAuthor: %s\nISBN: %ld\nRating: %.1f★\n",
                   head->title, head->author, head->isbn, head->rating);
            return head;
        }
        head = head->next;
    }
    printf(BOLD RED"Book with ISBN %ld not found.\n"RESET, isbn);
    return NULL;
}

void deleteLastAddedBook(Library *lib){
    if (!lib){
        printError("Library does not exist.");
        return;
    }

    // Check which list we're working with
    Book **current_list;
    if (lib->is_split){
        printError("Cannot delete last added book from split lists. Use delete by ISBN instead.");
        return;
    }
    else{
        current_list = &lib->main_list;
    }

    if (!*current_list){
        printError("List is empty, nothing to delete.");
        return;
    }

    if (!lib->last_added){
        printError("No record of last added book. Use delete by ISBN instead.");
        return;
    }

    // Check if last_added is in the current list
    Book *temp = *current_list;
    Book *prev = NULL;
    int found = 0;

    while (temp){
        if (temp == lib->last_added){
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found){
        printError("Last added book not found in current list.");
        lib->last_added = NULL;
        return;
    }

    // Delete the book
    if (prev == NULL){
        // Deleting the head
        *current_list = lib->last_added->next;
    }
    else{
        prev->next = lib->last_added->next;
    }

    printf(BOLD GREEN"Deleted last added book: '%s' by %s\n"RESET, 
           lib->last_added->title, lib->last_added->author);
    
    free(lib->last_added);
    lib->last_added = NULL;
}

void deleteBookByISBN(Book **head, long isbn){
    if (!*head){
        printError("List is empty.");
        return;
    }

    if ((*head)->isbn == isbn){
        Book *temp = *head;
        *head = (*head)->next;
        free(temp);
        printf(BOLD GREEN"Book with ISBN %ld deleted.\n"RESET, isbn);
        return;
    }

    Book *temp = *head;
    while (temp->next && temp->next->isbn != isbn){
        temp = temp->next;
    }

    if (!temp->next){
        printf(BOLD RED"Book with ISBN %ld not found.\n"RESET, isbn);
        return;
    }

    Book *toDelete = temp->next;
    temp->next = temp->next->next;
    free(toDelete);
    printf(BOLD GREEN"Book with ISBN %ld deleted.\n"RESET, isbn);
}

int countBooks(Book *head){
    int count = 0;
    while (head){
        count++;
        head = head->next;
    }
    return count;
}

float averageRating(Book *head){
    if (!head) return 0.0f;
    
    float sum = 0.0f;
    int count = 0;
    while (head){
        sum += head->rating;
        count++;
        head = head->next;
    }
    return sum / count;
}

// ============= SORTING =============

static Book* splitForSort(Book *head){
    Book *fast = head, *slow = head;
    while (fast && fast->next){
        fast = fast->next->next;
        if (fast) slow = slow->next;
    }
    Book *second = slow->next;
    slow->next = NULL;
    return second;
}

static Book* mergeByRating(Book *first, Book *second){
    if (!first) return second;
    if (!second) return first;
    
    if (first->rating <= second->rating){
        first->next = mergeByRating(first->next, second);
        return first;
    }
    else{
        second->next = mergeByRating(first, second->next);
        return second;
    }
}

static Book* mergeSortRecursive(Book *head){
    if (!head || !head->next) return head;
    
    Book *second = splitForSort(head);
    head = mergeSortRecursive(head);
    second = mergeSortRecursive(second);
    return mergeByRating(head, second);
}

void sortByRating(Book **head){
    if (!*head || !(*head)->next){
        printWarning("List has fewer than 2 books. No sorting needed.");
        return;
    }
    *head = mergeSortRecursive(*head);
    printSuccess("Books sorted by rating.");
}

// ============= SPLIT/MERGE =============

void splitLibrary(Library *lib){
    if (!lib){
        printError("Library does not exist.");
        return;
    }

    if (lib->is_split){
        printWarning("Library is already split. Merge first before splitting again.");
        return;
    }

    if (!lib->main_list){
        printError("Cannot split: main library is empty.");
        return;
    }

    Book *temp = lib->main_list;
    Book *highTail = NULL, *lowTail = NULL;

    while (temp){
        Book *newBook = malloc(sizeof(Book));
        if (!newBook){
            printError("Memory allocation failed during split.");
            return;
        }

        memcpy(newBook, temp, sizeof(Book));
        newBook->next = NULL;

        if (temp->rating >= 3.5f){
            if (!lib->high_rated){
                lib->high_rated = newBook;
            }
            else{
                highTail->next = newBook;
            }
            highTail = newBook;
        }
        else{
            if (!lib->low_rated){
                lib->low_rated = newBook;
            }
            else{
                lowTail->next = newBook;
            }
            lowTail = newBook;
        }
        temp = temp->next;
    }

    freeBookList(lib->main_list);
    lib->main_list = NULL;
    lib->last_added = NULL;  // Clear last_added since we're working with split lists now
    lib->is_split = 1;
    printSuccess("Library split into high-rated (≥3.5★) and low-rated (<3.5★) books.");
}

void mergeLibrary(Library *lib){
    if (!lib || !lib->is_split){
        printError("Library is not split.");
        return;
    }

    if (!lib->high_rated && !lib->low_rated){
        printError("Both split lists are empty.");
        lib->is_split = 0;
        return;
    }

    if (!lib->high_rated){
        lib->main_list = lib->low_rated;
    }
    else if (!lib->low_rated){
        lib->main_list = lib->high_rated;
    }
    else{
        Book *temp = lib->high_rated;
        while (temp->next) temp = temp->next;
        temp->next = lib->low_rated;
        lib->main_list = lib->high_rated;
    }

    lib->high_rated = NULL;
    lib->low_rated = NULL;
    lib->is_split = 0;
    printSuccess("Library merged successfully.");
}