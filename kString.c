/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kString.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdio.h>
#include <windows.h>
#include "kString.h"
#include "kStatus.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t toLowercase( char *dest, const char *src )
{
    if (src != NULL)
    {
        if (dest == src)
        {
            while (*dest != 0)
            {
                *dest = ((*dest > 0x40) && (*dest < 0x5B)) ? *dest + 32 : *dest;
                dest++;
            }
        }
        else
        {
            while (*src != 0)
            {
                *dest = ((*src > 0x40) && (*src < 0x5B)) ? *src + 32 : *src;
                dest++;
                src++;
            }
        }
        return KS_OK;
    }
    return KS_ERROR;
}

uint32_t strcmpLowercase( char *lowercase, const char *src )
{
    char lc[256] = {0};
    if (toLowercase(lc, src) == KS_OK)
    {
        if (strcmp(lowercase, lc) == 0)
        {
            return KS_OK;
        }
    }
    return KS_ERROR;
}

/*************************************** END OF FILE ****************************************/
