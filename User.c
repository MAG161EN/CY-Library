/**
 * @file User.c
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
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "User.h"
#include "UserRole.h"

// allocate dynamically disk space for a new user
User allocUser(const char* login, const char* password, enum UserRole role)
{
    User user = malloc(sizeof(struct User));
    if (user == NULL)
    {
        return NULL;
    }

    user->login = malloc((sizeof(char) * strnlen(login, 255)) +1);
    if (user->login == NULL)
    {
        free(user);
        return NULL;
    }

    user->password = malloc((sizeof(char) * strnlen(password, 255)) +1);
    if (user->password == NULL)
    {
        free(user->login);
        free(user);
        return NULL;    
    }

    strcpy(user->login, login);
    strcpy(user->password, password);
    user->role = role;
    user->borrowingBook = (BorrowingList){NULL, 0ULL};
    return user;
}

// free up space --> allocUser
void freeUser(User user)
{
    if (user != NULL)
    {
        clearBorrowingList(&user->borrowingBook);
        free(user->password);
        free(user->login);
        free(user);
    }
    
}

// skip a line in the file
static void skipLine(FILE* file)
{
    char c;
    while ((c = getc(file)) != '\n' && c != EOF);
}

// remove the \n after calling fgets
static void chomp(char *str)
{
    while (*str != '\n' && *str != '\0')
        ++str;
    *str = '\0';
}

// test if the user is in the file
static bool ifUserExist(const char* login)
{
    FILE* userFile = fopen("./user.txt", "r");
    if (userFile == NULL)
    {
        return NULL;
    }

    while (!feof(userFile))
    {
        char buffer[255];
        fgets(buffer, 255, userFile);
        if (buffer[0] == '*')
        {
            fgets(buffer, 255, userFile);
            chomp(buffer);
            if (strcmp(buffer, login) == 0)
            {
                fclose(userFile);
                return true;
            }
        }
    }
    fclose(userFile);
    return false;
}

// adds a new user and returns this new dynamically allocated user
User addUser(const char* login, const char* password, enum UserRole role)
{
    if (!ifUserExist(login))
    {
        FILE* userFile = fopen("./user.txt", "a");
        if (userFile == NULL)
        {
            return NULL;
        }

        User user = allocUser(login, password, role);
        if (user == NULL)
        {
            fclose(userFile);
            return NULL;
        }


        fprintf(userFile, "*****\n%s\n%s\n%d\n", user->login, user->password, user->role);
        ptr_Borrowing ptr = user->borrowingBook.list;
        while (ptr != NULL)
        {
            fprintf(userFile, "%u ", ptr->book);
        }

        fclose(userFile);
        return user;
    }
    return NULL;
}

// look for a user in the file
User foundUser(Librairy *const librairy, const char* login, const char* password)
{
    FILE* userFile = fopen("./user.txt", "r");
    if (userFile == NULL)
    {
        return NULL;
    }

    skipLine(userFile);
    while (!feof(userFile))
    {
        char loginUser[255];
        fgets(loginUser, 255, userFile);
        chomp(loginUser);

        if (strcmp(loginUser, login) == 0)
        {
            printf("True");
            char passwordUser[255];
            fgets(passwordUser, 255, userFile);
            chomp(passwordUser);

            if (strcmp(passwordUser, password) == 0)
            {
                printf("True2");
                char strRole[255];
                fgets(strRole, 255, userFile);
                chomp(strRole);
                enum UserRole role = (enum UserRole)strtoul(strRole, NULL, 10);

                User user = allocUser(loginUser, passwordUser, role);

                if (!feof(userFile))
                {
                    printf("True3");
                    char borrowing[255];
                    fgets(borrowing, 255, userFile);
                    if (borrowing[0] != '*')
                    {
                        chomp(borrowing);

                        const char *ptr = borrowing;
                        char *end;
                        for (idBook id = (idBook)strtoul(ptr, &end, 10); ptr != end; id = (idBook)strtoul(ptr, &end, 10))
                        {
                            ptr = end;
                            if (errno == ERANGE){
                                printf("range error, got ");
                                errno = 0;
                            }


                            ptr_Librairy ptr_lib = librairy->list;
                            while (ptr_lib != NULL)
                            {
                                if (ptr_lib->book->id == id)
                                {
                                    break;
                                }
                                ptr_lib = ptr_lib->next;
                            }
                            if (ptr_lib != NULL)
                            {
                                borrowingBook(user, librairy, ptr_lib->book->title);
                            }
                        }
                    }
                }
                
                return user;

            }
            else
            {
                skipLine(userFile);
                skipLine(userFile);
                if (fgetc(userFile) == '*')
                {
                    skipLine(userFile);
                }
                else
                {
                    fseek(userFile, -1, ftell(userFile));
                }
            }
        }
        else
        {
            skipLine(userFile);
            skipLine(userFile);
            skipLine(userFile);
            if (fgetc(userFile) == '*')
            {
                skipLine(userFile);
            }
            else
            {
                fseek(userFile, -1,ftell(userFile));
            }
            
        }
                    
    }
    return NULL;
}

// save changes (user) to file
void saveModifUser(User user)
{
    FILE* userFile = fopen("user.txt", "r");
    if (userFile == NULL)
    {
        return;
    }

    unsigned int nbLines = 0;
    char c;
    while ((c = getc(userFile)) != EOF)
    {
        if (c == '\n')
            nbLines++;
    }
    nbLines++;
    rewind(userFile);

    char** file = malloc(sizeof(char*)*nbLines);
    if (file == NULL)
    {
        fclose(userFile);
        return;
    }
    for (unsigned int i = 0; i < nbLines; i++)
    {
        file[i] = malloc(sizeof(char)*255);
        if (file[i] == NULL)
        {
            for (unsigned int j = i-1; j < i; j--)
            {
                free(file[j]);
                free(file);
                fclose(userFile);
            }
            return;
        }
    }

    for (unsigned int line = 0; line < nbLines; line++)
    {
        fgets(file[line], 255, userFile);
        chomp(file[line]);
    }

    fclose(userFile);

    userFile = fopen("user.txt", "w");
    if (userFile == NULL)
    {
        return;
    }

    unsigned int currentLine = 0;
    while (strcmp(file[currentLine], user->login) != 0)
    {
        fprintf(userFile, "%s\n", file[currentLine]);
        currentLine++;
    }
    fprintf(userFile, "%s\n%s\n%d\n", user->login, user->password, user->role);
    ptr_Borrowing ptr = user->borrowingBook.list;
    while (ptr != NULL)
    {
        fprintf(userFile, "%u ", ptr->book);
        ptr = ptr->next;
    }
    fputs("\n", userFile);
    currentLine+=3;
    if (currentLine < nbLines && file[currentLine][0] != '*')
    {
        currentLine++;
    }
    while (currentLine < nbLines)
    {
        if (file[currentLine][0] != '\r')
            fprintf(userFile, "%s\n", file[currentLine]);
        currentLine++;
    }
    for (unsigned int i = 0; i < nbLines; i++)
    {
        free(file[i]);
    }
    free(file);
    fclose(userFile);
}