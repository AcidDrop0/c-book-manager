#ifndef BOOK_H
#define BOOK_H

#define MAXNAME 100

// Book structure
typedef struct Book{
    char title[MAXNAME];
    char author[MAXNAME];
    long isbn;
    float rating;  // 0.0 - 5.0
    struct Book *next;
}Book;

// Library management structure
typedef struct{
    Book *main_list;
    Book *high_rated;
    Book *low_rated;
    Book *last_added;  // variable to track the most recently added book
    int is_split;      // indicates if the library is split 
}Library;

// Library management
Library* createLibrary(void);
void destroyLibrary(Library *lib);

// Book operations
Book* addBook(Library *lib, const char *title, const char *author, long isbn, float rating);
void displayBooks(Book *head, const char *list_name);
Book* findBook(Book *head, long isbn);
void deleteLastAddedBook(Library *lib);
void deleteBookByISBN(Book **head, long isbn);
int countBooks(Book *head);
float averageRating(Book *head);

// List operations
void sortByRating(Book **head);
void splitLibrary(Library *lib);
void mergeLibrary(Library *lib);

// Helper functions for main.c
Book* getCurrentList(Library *lib, char choice);
Book** getCurrentListPtr(Library *lib, char choice);

#endif // BOOK_H