/**
 * @file Book.c
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#include <stdlib.h>
#include <string.h>

#include "Book.h"



/**
 * @brief allocate dynamically disk space for books
 * 
 * @param title Le titre du livre
 * @param author 
 * @param category 
 * @param id 
 * @return Book 
 */
Book allocBook(const char* title, const char* author, const char* category, idBook id)
{
    size_t const sizeTitre = strnlen( title, 255) +1;
    size_t const sizeAuthor = strnlen( author, 255) +1;
    size_t const sizeCategory = strnlen( category, 255) +1;
    Book book = malloc(sizeof(struct Book));
    if (book == NULL)
    {
        return NULL;
    }

    book->title = malloc(sizeof(char) * sizeTitre);
    if (book->title == NULL)
    {
        free(book);
        return NULL;
    }

    book->author = malloc(sizeof(char) * sizeAuthor);
    if (book->author == NULL)
    {
        free(book->title);
        free(book);
        return NULL;    
    }

    book->category = malloc(sizeof(char) * sizeCategory);
    if (book->category == NULL)
    {
        free(book->title);
        free(book->author);
        free(book);
        return NULL;    
    }

    strncpy(book->title, title, sizeTitre);
    strncpy(book->author, author, sizeAuthor);
    strncpy(book->category, category, sizeCategory);
    book->id = id;
    return book;
}



/**
 * @brief free up space --> allocBook
 * 
 * @param book 
 */
void freeBook(Book book)
{
    free(book->category);
    free(book->author);
    free(book->title);
    free(book);
}