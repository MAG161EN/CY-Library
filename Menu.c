/**
 * @file Menu.c
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "Menu.h"
#include "User.h"
#include "UserRole.h"
#include "colors.h"

#define MAX_SIZE_BUFFER 255

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR() system("cls")
#else
#define CLEAR() system("clear")
#endif


/**
 * @brief show header for each menu
 * 
 * @param color 
 * @param text 
 */
static void header(color_t const color, const char* text)
{
    colors(RESET);
    printf("============================================================================\n\n");
    colors(color);
    printf("\t\t\t");
    printf("%s", text);
    colors(RESET);
    printf(" | ");
    colors(BLUE);
    printf("CY-library TECH\n\n");
    colors(RESET);
    printf("============================================================================\n\n");
    colors(RESET);
}

void forceDeconnect(void){
    header(RED, "Error: Login Failed");
}


/**
 * @brief display the options available to users in each menu
 * 
 * @param nbFields 
 * @param ... 
 */
static void printMenu(unsigned int nbFields, ...)
{
#define NB_COLOR 7
    color_t tabColor[NB_COLOR] = { GREEN, CYAN, RED, YELLOW, MAGENTA, BLUE, WHITE };
    unsigned int indexTabColor = 0;
    va_list listField;
    va_start(listField, nbFields);

    printf("\nPlease choose your operation");
    colors(FLASH);
    printf(" ...\n\n\n");
    colors(RESET);

    while (nbFields > 0)
    {
        const char* field = va_arg(listField, const char*);
        nbFields--;
        colors(tabColor[indexTabColor % NB_COLOR]);
        printf("\t\t\t[%u] ", indexTabColor + 1);
        colors(RESET);
        printf("%s\n\n", field);
        indexTabColor++;
    }
}


/**
 * @brief Secure the scanf
 * 
 * @param str 
 * @param size 
 * @return char* 
 */
static char* secureEntry(char* str, int const size)
{
    char* data;

    data = fgets(str, size, stdin);

    if (data != NULL)
    {
        size_t const len = strlen(str) - 1;

        if (str[len] == '\n')
        {
            str[len] = '\0';
        }
        else
        {
            fscanf(stdin, "%*[^\n]");
            fgetc(stdin);
        }
    }
    return data;
}



/**
 * @brief Displays a goodbye message
 * 
 */
void see_you_msg(void) {
    header(RED, "See you soon !");
}


/**
 * @brief 
 * 
 * @param librairy 
 * @param currentUser 
 */
void menu(Librairy* const librairy, User* const currentUser) {
    int opt = 0;
    CLEAR();
    header(CYAN, "Welcome");

    printMenu(3, "Sign in", "Sign up", "Exit");

    char strOpt[2];
    if (secureEntry(strOpt, 2) == NULL)
    {
        printf("An error occurred while entering");
        return;
    }
    opt = strOpt[0] - '1' + 1;

    switch (opt) {

    case 1: CLEAR();
        signin(librairy, currentUser);
        break;
    case 2: CLEAR();
        signup(librairy, currentUser);
        break;
    case 3: CLEAR();
        see_you_msg();
        break;
    default: CLEAR();
        colors(RED);
        printf("No match found !\n");
        colors(RESET);
        menu(librairy, currentUser);
        break;
    }
} 



/**
 * @brief Allows an existing user to login
 * 
 * @param librairy 
 * @param currentUser 
 */
void signin(Librairy* const librairy, User* const currentUser)
{
    char username[255];
    char password[255];


    header(YELLOW, "Sign In");

    colors(WHITE);

    printf("Enter your username: ");
    if (secureEntry(username, 255) == NULL)
    {
        puts("An error has occurred");
        return;
    }

    printf("Enter your password: ");
    if (secureEntry(password, 255) == NULL)
    {
        puts("An error has occurred");
        return;
    }

    *currentUser = foundUser(librairy, username, password);
   
    if (*currentUser == NULL)
    {
        CLEAR();
        forceDeconnect();
        return;
    }
    CLEAR();
    library_menu(librairy, currentUser);
}



/**
 * @brief Allows a user to choose their role
 * 
 * @return enum UserRole 
 */
static enum UserRole getRole(void)
{
    printMenu(2, "You are a student", "You are a teacher");

    char strRole[255];
    secureEntry(strRole, 255);
    unsigned long IntRole = strtoul(strRole, NULL, 10);
    if (IntRole == 1)
    {
        return STUDENT;
    }
    else if (IntRole == 2)
    {
        return PROFESSOR;
    }
    else
    {
        printf("this post does not exist");
        return 0;
    }
}

/**
 * @brief Allows a new user to create his "account"
 * 
 * @param librairy 
 * @param currentUser 
 */
void signup(Librairy* const librairy, User* const currentUser) {

    char username[255];
    char password[255];
    char pswd[255];

    header(GREEN, "Sign Up");

    colors(WHITE);

    printf("\t\t\tEnter your username: ");
    if (secureEntry(username, 255) == NULL)
    {
        puts("An error has occurred");
        return;
    }

    printf("\t\t\tEnter your password: ");
    if (secureEntry(password, 255) == NULL)
    {
        puts("An error has occurred");
        return;
    }

    printf("\t\t\tConfirm your password: ");
    if (secureEntry(pswd, 255) == NULL)
    {
        puts("An error has occurred");
        return;
    }



    colors(RESET);

    if (strcmp(password, pswd) == 0)
    {
        enum UserRole role = getRole();
        *currentUser = addUser(username, password, role);
        if (*currentUser == NULL)
        {
            CLEAR();
            menu(librairy, currentUser);
        }
        else
        {
            CLEAR();
            library_menu(librairy, currentUser);
        }
    }
    else
    {
        printf("The passwords do not match, please try again.");

    }

}



/**
 * @brief Displays user books
 * 
 * @param librairy 
 * @param currentUser 
 */
static void printBookOfUser(Librairy* const librairy, User const currentUser)
{
    ptr_Borrowing ptrB = currentUser->borrowingBook.list;
    #define NB_COLOR 7
    color_t tabColor[NB_COLOR] = { GREEN, CYAN, RED, YELLOW, MAGENTA, BLUE, WHITE };
    unsigned int indexTabColor = 0;

    while (ptrB != NULL)
    {
        Book book = getBookWithId(librairy, ptrB->book);
        colors(tabColor[indexTabColor % NB_COLOR]);
        printf("\t\t\t[%u] ", indexTabColor + 1);
        colors(RESET);
        printf("%s %s to be returned to %dh%d\n\n", book->title, book->author, localtime(&ptrB->borrowingTime)->tm_hour, localtime(&ptrB->borrowingTime)->tm_hour);
        indexTabColor++;
        ptrB = ptrB->next;

    }
}

/**
 * @brief Library management menu
 * 
 * @param librairy 
 * @param currentUser 
 */
void library_menu(Librairy* const librairy, User* const currentUser) {

    int opt = 0;

    header(RED, "Application");
    printBookOfUser(librairy, *currentUser);
    printMenu(3, "Borrow a new book", "Return a book", "Sign Out");

    char strOpt[2];
    if (secureEntry(strOpt, 2) == NULL)
    {
        printf("An error occurred while entering");
        return;
    }
    opt = strOpt[0] - '1' + 1;

    switch (opt) {

    case 1:
        CLEAR();
        book_theme(librairy, currentUser);
        break;
    case 2:
        CLEAR();
        return_book(librairy, currentUser);
        break;
    case 3:
        CLEAR();
        see_you_msg();
        menu(librairy, currentUser);
        break;
    default: printf("No match found...\n");
        //library_menu();
        break;
    }
}

void borrowBookWithTheme(Librairy* const librairy, User* const currentUser, const char* theme);

/**
 * @brief 
 * 
 * @param librairy 
 * @param currentUser 
 */
void book_theme(Librairy* const librairy, User* const currentUser)
{
    header(GREEN, "Borrow a new book");
    printMenu(7, "Love", "Action", "Detective", "Fantasy", "Fantastic", "Sci-Fi", "All themes");

    int opt = 0;

    char strOpt[2];
    if (secureEntry(strOpt, 2) == NULL)
    {
        printf("An error occurred while entering");
        return;
    }
    opt = strOpt[0] - '1' + 1;

    switch (opt) {
    case 1:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Love");
        break;
    case 2:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Action");
        break;
    case 3:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Detective");
        break;
    case 4:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Fantasy");
        break;
    case 5:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Fantastic");
        break;
    case 6:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, "Sci-Fi");
        break;
    case 7:
        CLEAR();
        borrowBookWithTheme(librairy, currentUser, NULL);
        break;
    default:
        break;
    }
    saveModifUser(*currentUser);
    library_menu(librairy, currentUser);
}

/**
 * @brief Borrowed a book with the different themes
 * 
 * @param librairy 
 * @param currentUser 
 * @param theme 
 */
void borrowBookWithTheme(Librairy* const librairy, User* const currentUser, const char* theme) {

    int opt = 0;
    header(GREEN, "Borrow a new book");
    printFreeBook(librairy, theme);

    char strOpt[255];
    if (secureEntry(strOpt, 2) == NULL)
    {
        printf("An error occurred while entering");
        return;
    }
    opt = (int)strtol(strOpt, NULL, 10);

    ptr_Librairy ptr = librairy->list;
    int count = 1;
    while (ptr != NULL && count != opt)
    {
        if (!ptr->borrowing)
        {
            if (theme != NULL)
            {
                if (strcmp(ptr->book->category, theme) == 0)
                {
                    count++;
                }
            }
            else
            {
                count++;
            }
        }
        ptr = ptr->next;
    }
    CLEAR();
    borrowingBook(*currentUser, librairy, ptr->book->title);
}

/**
 * @brief 
 * 
 * @param librairy 
 * @param currentUser 
 */
void return_book(Librairy* const librairy, User* const currentUser) {

    /*int opt = 0;

    printBookOfUser(librairy, *currentUser);

    char strOpt[255];
    if (secureEntry(strOpt, 2) == NULL)
    {
        printf("An error occurred while entering");
        return;
    }
    opt = (int)strtol(strOpt, NULL, 10);

    ptr_Borrowing ptr = (*currentUser)->borrowingBook.list;
    int count = 0;
    while (ptr != NULL && count < opt)
    {
        count++;
    }
    if (ptr != NULL)
    {
        returnBook(*currentUser, librairy, getBookWithId(librairy, ptr->book)->title);
        CLEAR();
        library_menu(librairy, currentUser);
    }
    else
    {
        CLEAR();
        library_menu(librairy, currentUser);
    }
    saveModifUser(*currentUser);*/

    header(RED, "Function not available");
    colors(WHITE);
    printf("See menu.c --> return_book()\n");
}