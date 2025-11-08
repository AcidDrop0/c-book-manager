# Book Management System (C)

A command-line book management system written in **C**.

This program allows you to maintain a book library using a linked-list data structure.  
You can add books, delete books, search by ISBN, sort the list by rating, split the library into high vs. low-rated lists, merge them again, and calculate average ratings.

---

## Features

- Add books (title, author, ISBN, rating)
- Display all books or only high-rated / low-rated lists
- Search by ISBN
- Delete last added book or delete a specific book by ISBN
- Sort books by rating
- Split main list into:
  - High Rated (≥ 3.5)
  - Low Rated
- Merge the two lists back into one
- Count books
- Compute average rating for selected list
- Free memory safely (selectively or entire library)
- CLI interface
- Fully linked-list based storage (no arrays)

---

## File Structure

```
.
├── book.c
├── book.h
├── ui_utils.c
├── ui_utils.h
└── main.c
```

---

## How to Compile

Compile like this:

```bash
gcc -o book_manager main.c book.c ui_utils.c
```

Then run:

```bash
./book_manager
```

---

## Requirements

- GCC / Clang C compiler
- Standard C library

No 3rd-party libraries required.

---

## Example Usage (flow)

```text
> 1) Add book
> Title: Dune
> Author: Herbert
> ISBN: 9780441013593
> Rating: 4.8
```

Then later:

```text
> 8) Sort by rating
> 9) Average
> 6) Split / Merge library
```

---

## Notes

- Everything is stored dynamically (malloc / free).
- When split mode is active, you actively work with one of two linked lists instead of the main one.
- Memory routines allow selective or full freeing.
