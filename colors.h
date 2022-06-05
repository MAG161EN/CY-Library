/**
 * @file colors.h
 * @author MAG161EN
 * @brief 
 * @version 1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef COLORS_H
#define COLORS_H

 // Pour windows
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    static void SetColor(int textColor, int backColor) // fonction d'affichage de couleurs
    {
        HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute( outHandle, backColor*16+textColor);
    }
    #define clrscr()
    #define colors(param) SetColor(param, 0)

    #define BLUE 9
    #define GREEN 10
    #define YELLOW 14
    #define CYAN 3
    #define RED 12
    #define MAGENTA 13

    #define FLASH 13
    #define WHITE 7
    #define RESET 7

    typedef unsigned int color_t;
// Pour Unix
#else
    #include <stdio.h>
    #define clrscr() printf("\033[H\033[2J")
    #define colors(param) printf("\033[%sm",param)

    #define BLUE "34;1"
    #define GREEN "32;1"
    #define YELLOW "33;1"
    #define CYAN "36;1"
    #define RED "31;1"
    #define MAGENTA "35"

    #define FLASH "35;5"
    #define WHITE "37;1"
    #define RESET "0"

    typedef const char* color_t;


#endif // defined(_WIN32) || defined(_WIN64)

#endif // COLORS_H