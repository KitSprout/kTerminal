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

#define MAX_COMMAND_SIZE                (256)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef enum
{
    COMMAND_NULL = 0,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_INFO,
    COMMAND_AUTO,
    COMMAND_PORT,
    COMMAND_BAUD,

    COMMAND_TERMINAL,
    COMMAND_TERMINAL_AUTO,

    COMMAND_DEBUG,
    COMMAND_ERROR,

} kcommand_e;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_GetCommand( const char *command );

uint32_t kCommand_Help( void );
uint32_t kCommand_HelpPort( void );
uint32_t kCommand_HelpBaud( void );

uint32_t kCommand_GetSettingInformation( void );

uint32_t kCommand_UartConfigureAutomatic( void );
// uint32_t kCommand_UartGetComportList( void );
uint32_t kCommand_UartComportConfigure( char *portString, char *baudrateString );
uint32_t kCommand_UartBaudrateConfigure( char *baudrateString );

uint32_t kCommand_UartTerminalProcess( char **argv );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
