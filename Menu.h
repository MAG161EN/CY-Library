/**
 * @file Menu.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MENU_H
#define MENU_H

#include "Librairy.h"
#include "User.h"


void menu(Librairy *const librairy, User *const currentUser);

void signin(Librairy *const librairy, User *const currentUser);

void signup(Librairy *const librairy, User *const currentUser);

void library_menu(Librairy *const librairy, User *const currentUser);

void book_theme(Librairy *const librairy, User *const currentUser);

void return_book(Librairy *const librairy, User *const currentUser);



#endif // MENU_H