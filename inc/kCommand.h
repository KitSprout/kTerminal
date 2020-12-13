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
#include "kCommandHelp.h"
#include "kCommandTarget.h"
#include "kCommandTwi.h"

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

    COMMAND_CHECK,
    COMMAND_SCAN,
    COMMAND_REG,
    COMMAND_READ,
    COMMAND_WRITE,

    COMMAND_DEBUG,
    COMMAND_ERROR,

} kcommand_e;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t    kCommand_GetCommand( const char *command );

uint32_t    kCommand_Help( void );

uint32_t    kCommand_GetVersion( char *version );
uint32_t    kCommand_GetSettingInformation( void );

uint32_t    kCommand_UartConfigureAutomatic( void );
uint32_t    kCommand_UartComportConfigure( char *portString, char *baudrateString );
uint32_t    kCommand_UartBaudrateConfigure( char *baudrateString );
uint32_t    kCommand_UartTerminalProcess( char **argv );

uint32_t    kCommand_CheckDevice( void );
uint32_t    kCommand_TwiScanDevice( void );
uint32_t    kCommand_TwiScanRegister( char *addressString );
uint32_t    kCommand_TwiRead( char *addressString, char *registerString, char *lenghtString );
uint32_t    kCommand_TwiWrite( char *addressString, char *registerString, char **dataString, int length );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
