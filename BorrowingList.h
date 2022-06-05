/**
 * @file BorrowingList.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef BORROWING_LIST_H
#define BORROWING_LIST_H
#include <time.h>
#include <stdbool.h>
#include "Book.h"
#include "Librairy.h"

typedef struct Borrowing
{
    struct Borrowing* next;
    idBook book;
    time_t borrowingTime;
} *ptr_Borrowing;

typedef struct BorrowingList
{
    ptr_Borrowing list;
    size_t size;
} BorrowingList;

typedef struct User *User;

extern bool borrowingBook(User user, Librairy* const librairy, const char* title);
extern bool returnBook(User user, Librairy *const librairy, const char* title);
extern void clearBorrowingList(BorrowingList*const list);

#endif //BORROWING_LIST_H