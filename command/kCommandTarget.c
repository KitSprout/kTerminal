/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandTarget.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kSerial.h"
#include "kString.h"
#include "kLogger.h"
#include "kCommand.h"
#include "kCommandHelp.h"
#include "kCommandTarget.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_Target( const char *commandString, const char *valueString )
{
    char cmd[MAX_COMMAND_SIZE] = {0};
    if (toLowercase(cmd, commandString) == KS_ERROR)
    {
        return kCommandTarget_CheckDevice();
    }
    // >> ks -taget help
    if ((strcmp("h", cmd) == 0) || (strcmp("help", cmd) == 0))
    {
        klogd("\n");
        kCommand_HelpTarget();
        return KS_OK;
    }
    // >> ks -taget baud [baud]
    if (strcmp("baud", cmd) == 0)
    {
        klogd("\n set target baudrate\n");
        return KS_OK;
    }
    // >> ks -taget rate [rate]
    if (strcmp("rate", cmd) == 0)
    {
        klogd("\n set target updaterate\n");
        return KS_OK;
    }
    // >> ks -taget mode [mode]
    if (strcmp("mode", cmd) == 0)
    {
        klogd("\n set target mode\n");
        return KS_OK;
    }

    return KS_OK;
}

uint32_t kCommandTarget_CheckDevice( void )
{
    uint32_t id = 0;

    if (kSerial_DeviceCheck(&id) != KS_OK)
    {
        klogd("  >> Device Not Found\n");
        return KS_ERROR;
    }
    klogd("  >> Device ID = %04X\n", id);
    return KS_OK;
}

uint32_t kCommandTarget_SetBaudrate( uint32_t baudrate )
{
    return KS_OK;
}

uint32_t kCommandTarget_SetUpdateRate( uint32_t updaterate )
{
    return KS_OK;
}

uint32_t    kCommandTarget_SetMode( uint32_t mode )
{
    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
