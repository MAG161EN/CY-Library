/**
 * @file Book.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#ifndef BOOK_H
#define BOOK_H

typedef unsigned int idBook;

typedef struct Book
{
    char* title;
    char* author;
    char* category;
    idBook id;
} *Book;

extern Book allocBook(const char* title, const char* author, const char* category, idBook id);
extern void freeBook(Book book);


#endif //BOOK_H