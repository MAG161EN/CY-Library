/**
 * @file Librairy.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LIBRAIRY_H
#define LIBRAIRY_H

#include <stdlib.h>
#include <stdbool.h>
#include "Book.h"

typedef struct bookLibrairy
{
    struct bookLibrairy* next;
    Book book;
    bool borrowing;
} *ptr_Librairy;

typedef struct Librairy
{
    ptr_Librairy list;
    size_t size;
} Librairy;

extern idBook getIdBook(Librairy*const librairy, const char* title, ptr_Librairy* ptrLibrairyBook, ptr_Librairy start);
extern bool addBookInLibrairy(Librairy*const librairy, const char* restrict title, const char* restrict author, const char* restrict category, idBook const id);
extern void deleteBookInLibrairy(Librairy*const librairy, const char* title);
extern void clearLibrairy(Librairy* const librairy);
extern Librairy loadLibrairy(void);
extern Book getBookWithId(Librairy *const librairy, idBook const id);
void printFreeBook(Librairy *const librairy, const char* category);

#endif