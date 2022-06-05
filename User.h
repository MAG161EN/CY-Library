/**
 * @file User.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include "UserRole.h"
#include "BorrowingList.h"

typedef struct User
{
    char *login;
    char *password;
    enum UserRole role;
    BorrowingList borrowingBook;
}*User;

extern User allocUser(const char* login, const char* password, enum UserRole role);
extern void freeUser(User user);
User addUser(const char* login, const char* password, enum UserRole role);
User foundUser(Librairy *const librairy, const char* login, const char* password);
void saveModifUser(User user);
#endif //USER_H