/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandTwi.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KCOMMANDTWI_H
#define __KCOMMANDTWI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kStatus.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t    kCommand_TwiScanDevice( void );
uint32_t    kCommand_TwiScanRegister( const char *addressString );
uint32_t    kCommand_TwiRead( const char *addressString, const char *registerString, const char *lenghtString );
uint32_t    kCommand_TwiWrite( const char *addressString, const char *registerString, char **dataString, int length );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
