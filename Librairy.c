/**
 * @file Librairy.c
 * @author MAG161EN
 * @brief The file that manages the bookstore managing all the books
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */



#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "Librairy.h"
#include "Book.h"
#include "colors.h"

static idBook autoId(Librairy*const librairy)
{
    idBook previousID = 0;
    ptr_Librairy ptr = librairy->list;
    while (ptr != NULL)
    {
        if (ptr->book->id > previousID+1)
        {
            return previousID+1;
        }
        previousID++;
        ptr = ptr->next;
    }
    return librairy->size+1;
}



/**
 * @brief Add a file to the book list
 * 
 * @param librairy Pointer to struct librairy
 * @param title title of the book
 * @param author author of the book
 * @param category category of the book
 * @param id id of the book
 * @return true The book was added
 * @return false The book was not added
 */
bool addBookInLibrairy(Librairy*const librairy, const char* restrict title, const char* restrict author, const char* restrict category, idBook const id)
{
    ptr_Librairy new = malloc(sizeof(struct bookLibrairy));
    if (new == NULL)
    {
        return false;
    }
    new->borrowing = false;
    new->book = allocBook(title, author, category, id);
    if (new->book == NULL)
    {
        free(new);
        return false;
    }
    librairy->size++;

    if (librairy->list == NULL)
    {
        new->next = NULL;
        librairy->list = new;
    }
    else 
    {
        ptr_Librairy ptr = librairy->list;
        while (ptr->next != NULL && ptr->book->id < new->book->id)
        {
            ptr = ptr->next;
        }
        new->next = ptr->next;
        ptr->next = new;
    }
    return true;
}

/**
 * @brief Get the Id Book object
 * 
 * @param librairy 
 * @param title 
 * @param ptrLibrairyBook 
 * @param start 
 * @return idBook 
 */
idBook getIdBook(Librairy*const librairy, const char* title, ptr_Librairy* ptrLibrairyBook, ptr_Librairy start)
{
    ptr_Librairy ptr = (start == NULL) ? librairy->list : start;
    while (ptr != NULL)
    {
        if (strcmp(ptr->book->title, title) == 0)
        {
            if (ptrLibrairyBook != NULL)
            {
                *ptrLibrairyBook = ptr;
            }
            return ptr->book->id;
        }
        ptr = ptr->next;
    }
    return 0;
}



/**
 * @brief Remove a book from the library
 * 
 * @param librairy 
 * @param title 
 */
void deleteBookInLibrairy(Librairy*const librairy, const char* title)
{
    if (librairy->list != NULL)
    {
        librairy->size--;
        if (librairy->size == 0)
        {
            freeBook(librairy->list->book);
            free(librairy->list);
            librairy->list = NULL;
        }
        else
        {
            ptr_Librairy ptr = librairy->list;
            bool found = false;
            while (!found && ptr->next->next != NULL)
            {
                ptr = ptr->next;
                if (strcmp(ptr->book->title, title) == 0)
                {
                    found = true;
                }
            }
            if (found)
            {
                ptr_Librairy tmp = ptr->next;
                ptr->next = tmp->next;
                freeBook(tmp->book);
                free(tmp);
            }
            
        }
    }
}



/**
 * @brief Free the memory taken by the library
 * 
 * @param librairy 
 */
void clearLibrairy(Librairy* const librairy)
{
    librairy->size = 0;
    ptr_Librairy ptr = librairy->list;
    while (ptr != NULL)
    {
        ptr_Librairy tmp = ptr;
        ptr = ptr->next;
        freeBook(tmp->book);
        free(tmp);
    }
    librairy->list = NULL;
}

/**
 * @brief remove the \\n after calling fgets
 * 
 * @param str 
 */
static void chomp(char *str)
{
    while (*str != '\n' && *str != '\0')
        ++str;
    *str = '\0';
}



/**
 * @brief Load the lib file into memory in a chained list
 * 
 * @return Librairy 
 */
Librairy loadLibrairy(void)
{
    FILE *const bookFile = fopen("book.txt", "r");
    if (bookFile == NULL)
    {
        return (Librairy){NULL, 0};
    }

    Librairy lib = {NULL, 0};
    while (!feof(bookFile))
    {
        char title[255];
        fgets(title, 255, bookFile);
        chomp(title);
        
        char author[255];
        fgets(author, 255, bookFile);
        chomp(author);

        char category[255];
        fgets(category, 255, bookFile);
        chomp(category);

        char strId[255];
        fgets(strId, 255, bookFile);
        chomp(strId);
        unsigned int id = (unsigned int)strtoul(strId, NULL, 10);
        addBookInLibrairy(&lib, title, author, category, id);
    }
    return lib;
}



/**
 * @brief Get the Book With Id object
 * 
 * @param librairy 
 * @param id 
 * @return Book 
 */
Book getBookWithId(Librairy *const librairy, idBook const id)
{
    ptr_Librairy ptr = librairy->list;
    while (ptr != NULL)
    {
        if (ptr->book->id == id)
        {
            return ptr->book;
        }
    }
    return NULL;
}



/**
 * @brief Displays available books according to a certain category
 * 
 * @param librairy 
 * @param category 
 */
void printFreeBook(Librairy *const librairy, const char* category)
{
    #define NB_COLOR 7
    color_t tabColor[NB_COLOR] = {GREEN, CYAN, RED, YELLOW, MAGENTA, BLUE, WHITE};
    unsigned int indexTabColor = 0;

    ptr_Librairy ptr = librairy->list;
    while (ptr != NULL)
    {
        if (!ptr->borrowing)
        {
            
            if (category != NULL)
            {
                if (strcmp(category, ptr->book->category) == 0)
                    {
                    colors(tabColor[indexTabColor % NB_COLOR]);
                    printf("\t\t\t[%u] ", indexTabColor+1); 
                    colors(RESET);
                    printf("%s | %s\n\n", ptr->book->title, ptr->book->author); 
                    indexTabColor++;
                }
            }
            else
            {
                colors(tabColor[indexTabColor % NB_COLOR]);
                printf("\t\t\t[%u] ", indexTabColor+1); 
                colors(RESET);
                printf("%s | %s | %s\n\n", ptr->book->title, ptr->book->author, ptr->book->category); 
                indexTabColor++;
            }
        }
        ptr = ptr->next;
    }
}
