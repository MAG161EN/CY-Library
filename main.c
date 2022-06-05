/**
 * @file main.c
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include <time.h> // To manage borrowing time

#include "UserRole.h"
#include "Book.h"
#include "User.h"
#include "Menu.h"
#include <stdarg.h>


int main(void)
{
    Librairy lib = loadLibrairy();
    User currentUser = NULL;
    menu(&lib, &currentUser);
    
    /* foundUser(&lib, "Moi", "azerty");
    borrowingBook(currentUser, &lib, "Kensuke's Kingdom");
    printFreeBook(&lib, "Fantastic");
    saveModifUser(currentUser);
    //menu(&lib, &currentUser); */
    freeUser(currentUser);
    clearLibrairy(&lib);
    /* User user = allocUser("Moi", "azerty", STUDENT);
    borrowingBook(user, &lib, "Normal People");
    borrowingBook(user, &lib, "It ends with us");
    borrowingBook(user, &lib, "Afraid of the Light");
    borrowingBook(user, &lib, "The Forty Rules of Love");
    Book book = getBookWithId(&lib, user->borrowingBook.list->book);
    printf("L'emprunt est %s de %s", book->title, book->author);
    returnBook(user, &lib, "Normal People");
    freeUser(user);
    clearLibrairy( &lib); */

    return 0;
}