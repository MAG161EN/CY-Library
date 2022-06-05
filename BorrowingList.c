/**
 * @file BorrowingList.c
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "User.h"
#include "Book.h"
#include "Librairy.h"
#include "BorrowingList.h"

#define PROFESSOR_MINUTE_BORROWING 3
#define STUDENT_MINUTE_BORROWING 2



/**
 * @brief 
 * 
 * @param user 
 * @return true 
 * @return false 
 */
static bool lateReturn(User user)
{
    if (user->borrowingBook.size > 0)
    {
        ptr_Borrowing ptr = user->borrowingBook.list;
        while (ptr != NULL)
        {
            if (ptr->borrowingTime < time(NULL))
            {
                return true;
            }
            ptr = ptr->next;
        }
    }
    return false;
}



/**
 * @brief 
 * 
 * @param librairy 
 * @param title 
 * @param book 
 * @return idBook 
 */
static idBook foundFreeBook(Librairy *const librairy, const char* title, ptr_Librairy* book)
{
    ptr_Librairy tmp = NULL;
    if (book == NULL)
    {
        *book = tmp;
    }
    idBook id = 0;
    ptr_Librairy currentPos = NULL;
    do
    {
        id = getIdBook(librairy, title, book, currentPos);
        currentPos = (*book)->next;
    } while (currentPos != NULL && (*book)->borrowing && id != 0);
    return id;
}



/**
 * @brief Get the Ptr Librairy With Id object
 * 
 * @param librairy 
 * @param id 
 * @return ptr_Librairy 
 */
static ptr_Librairy getPtrLibrairyWithId(Librairy *const librairy, idBook const id)
{
    ptr_Librairy ptr = librairy->list;
    while (ptr != NULL && ptr->book->id != id)
    {
        ptr = ptr->next;
    }
    return ptr;
}



/**
 * @brief 
 * 
 * @param user 
 * @param librairy 
 * @param id 
 * @return true 
 * @return false 
 */
static bool borrowingBookWithId(User user, Librairy *const librairy, idBook const id)
{
    if (id != 0)
    {
        if (!lateReturn(user) && user->borrowingBook.size < user->role)
        {
            ptr_Borrowing new = malloc(sizeof(struct Borrowing));
            if (new == NULL)
            {
                return false;
            }

            new->borrowingTime = time(NULL) + 60 * ((user->role == PROFESSOR) 
                                                        ? PROFESSOR_MINUTE_BORROWING 
                                                        : STUDENT_MINUTE_BORROWING); //Calculating the time of delivery of the book
            new->book = id;
            new->next = NULL;
            ptr_Librairy book = getPtrLibrairyWithId(librairy, id);
            book->borrowing = true;
            user->borrowingBook.size++;
            if (user->borrowingBook.list == NULL)
            {
                user->borrowingBook.list = new;
            }
            else
            {
                ptr_Borrowing ptr = user->borrowingBook.list;
                while (ptr->next != NULL)
                {
                    ptr = ptr->next;
                }
                ptr->next = new;
            }
        }
        else
        {
            printf("Vous ne pouvez pas emprunter d'avantage de livre\n");
        }
        return true;
    }
    return false;
}



/**
 * @brief 
 * 
 * @param user 
 * @param librairy 
 * @param title 
 * @return true 
 * @return false 
 */
bool borrowingBook(User user, Librairy *const librairy, const char* title)
{
    ptr_Librairy book = NULL;
    idBook id = foundFreeBook(librairy, title, &book);
    return borrowingBookWithId(user, librairy, id);  
}



/**
 * @brief 
 * 
 * @param user 
 * @param book 
 * @return true 
 * @return false 
 */
bool idIsBorrowing(User user, idBook book)
{
    ptr_Borrowing ptr = user->borrowingBook.list;
    while (ptr != NULL)
    {
        if (ptr->book == book)
        {
            return true;
        }
    }
    return false;
}



/**
 * @brief 
 * 
 * @param user 
 * @param librairy 
 * @param title 
 * @return true 
 * @return false 
 */
bool returnBook(User user, Librairy *const librairy, const char* title)
{
    ptr_Librairy book = NULL;
    ptr_Librairy tmp = NULL;
    idBook id = 0;
    ptr_Librairy currentPos = NULL;
    do
    {
        id = getIdBook(librairy, title, &book, currentPos);
        currentPos = book->next;
    } while (currentPos != NULL && id != 0);
    ptr_Borrowing ptr = user->borrowingBook.list;
    if (ptr != NULL)
    {
        book->borrowing = true;
        if (user->borrowingBook.list != NULL && user->borrowingBook.list->book == id)
        {
            user->borrowingBook.size--;
            user->borrowingBook.list = ptr->next;
            free(ptr);
            return true;
        }
        else
        {
            while (ptr->next != NULL)
            {
                if (ptr->book == id)
                {
                    user->borrowingBook.size--;
                    ptr_Borrowing tmp = ptr->next;
                    ptr = ptr->next->next;
                    free(tmp);
                    return true;
                }
                ptr = ptr->next;
            }
            return false;
        }
    }
    return false;
}



/**
 * @brief 
 * 
 * @param list 
 */
void clearBorrowingList(BorrowingList*const list)
{
    ptr_Borrowing ptr = list->list;
    while (ptr != NULL)
    {
        ptr_Borrowing tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
}