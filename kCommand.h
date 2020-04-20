/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommand.h
 *  @author  KitSprout
 *  @date    Mar-2020
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KCOMMAND_H
#define __KCOMMAND_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kStatus.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef enum
{
    COMMAND_NULL = 0,
    COMMAND_HELP,
    COMMAND_INFO,
    COMMAND_PATH,
    COMMAND_AUTO,
    COMMAND_LIST,
    COMMAND_PORT,
    COMMAND_BAUDRATE,
    COMMAND_UART,
    COMMAND_I2C,
    COMMAND_KSERIAL,
    COMMAND_BTCONFIG,

    COMMAND_DEBUG,
    COMMAND_ERROR,

} kcommand_e;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_GetCommand( char *commandString );

// >> ks -h
uint32_t kCommand_Help( void );

// >> ks -info
uint32_t kCommand_GetSettingInformation( void );

// >> ks -auto
uint32_t kCommand_UartConfigureAutomatic( void );

// >> ks -list
uint32_t kCommand_UartGetComportList( void );

// >> ks -port [PORT or COMx]
// >> ks -port [PORT or COMx] [BAUDRATE]
uint32_t kCommand_UartComportConfigure( char *portString, char *baudrateString );

// >> ks -baudrate list
// >> ks -baudrate [BAUDRATE]
uint32_t kCommand_UartBaudrateConfigure( char *baudrateString );

// >> ks -uart
// >> ks -uart -save
uint32_t kCommand_UartProcess( char **argv );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
