/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandHelp.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void kCommand_HelpInfo( void )
{
    klogd("  -INFO                                  ... show configuration\n");
}

void kCommand_HelpVersion( void )
{
    klogd("  -VERSION                               ... show firmware version\n");
}

void kCommand_HelpAuto( void )
{
    klogd("  -AUTO                                  ... select available port automatically\n");
}

void kCommand_HelpPort( void )
{
    klogd("  -PORT LIST                             ... show serial comport list\n");
    klogd("  -PORT [PORT/COMx]                      ... serial comport setting\n");
    klogd("  -PORT [PORT/COMx] [BAUDRATE]           ... serial comport and baudrate setting\n");
}

void kCommand_HelpBaud( void )
{
    klogd("  -BAUD LIST                             ... show internal baudrate list\n");
    klogd("  -BAUD [BAUDRATE]                       ... serial baudrate setting\n");
}

void kCommand_HelpTerminal( void )
{
    klogd("  -TERMINAL                              ... uart terminal mode\n");
}

void kCommand_HelpCheck( void )
{
    klogd("  -CHECK                                 ... check device\n");
}

void kCommand_HelpScan( void )
{
    klogd("  -SCAN                                  ... i2c scan device\n");
}

void kCommand_HelpReg( void )
{
    klogd("  -REG [ADDRESS]                         ... i2c scan register data\n");
}

void kCommand_HelpRead( void )
{
    klogd("  -READ [ADDRESS] [REGISTER]             ... i2c single read data\n");
    klogd("  -READ [ADDRESS] [REGISTER] [LENS]      ... i2c multiple read data\n");
}

void kCommand_HelpWrite( void )
{
    klogd("  -WRITE [ADDRESS] [REGISTER] [DATA]     ... i2c single write data\n");
}

/*************************************** END OF FILE ****************************************/
