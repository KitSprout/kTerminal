/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandHelp.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KCOMMANDHELP_H
#define __KCOMMANDHELP_H

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

void    kCommand_HelpInfo( void );
void    kCommand_HelpVersion( void );
void    kCommand_HelpAuto( void );
void    kCommand_HelpPort( void );
void    kCommand_HelpBaud( void );
void    kCommand_HelpTerminal( void );
void    kCommand_HelpKSerial( void );
void    kCommand_HelpTarget( void );
void    kCommand_HelpScan( void );
void    kCommand_HelpReg( void );
void    kCommand_HelpRead( void );
void    kCommand_HelpWrite( void );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
