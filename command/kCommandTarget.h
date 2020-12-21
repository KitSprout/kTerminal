/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandTarget.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KCOMMANDTARGET_H
#define __KCOMMANDTARGET_H

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

uint32_t    kCommand_Target( const char *commandString, const char *valueString );

uint32_t    kCommandTarget_CheckDevice( void );
uint32_t    kCommandTarget_SetBaudrate( uint32_t baudrate );
uint32_t    kCommandTarget_SetUpdateRate( uint32_t rate );
uint32_t    kCommandTarget_SetMode( uint32_t mode );
uint32_t    kCommandTarget_GetValue( const char *inputString );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
