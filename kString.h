/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kString.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KSTRING_H
#define __KSTRING_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t    toLowercase( char *dest, const char *src );
uint32_t    strcmpLowercase( char *lowercase, const char *src );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
