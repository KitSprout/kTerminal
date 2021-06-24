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
#include <stdlib.h>
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

static uint32_t getTargetItem( const char *itemString );

/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_Target( const char *commandString, const char *inputString )
{
    char cmd[MAX_COMMAND_SIZE] = {0};
    int32_t value = -1;
    uint32_t status = toLowercase(cmd, commandString);
    if (inputString != NULL)
    {
        value = strtoul(inputString, NULL, 0);
    }

    // >> ks -taget
    // >> ks -taget check
    if ((status == KS_ERROR) || (strcmp("check", cmd) == 0))
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
    // >> ks -taget auto
    if ((strcmp("a", cmd) == 0) || (strcmp("auto", cmd) == 0))
    {
        return kCommandTarget_AutoBaudrate();
    }
    // >> ks -taget baud [baud]
    if (strcmp("baud", cmd) == 0)
    {
        return kCommandTarget_SetBaudrate(value);
    }
    // >> ks -taget rate [rate]
    if (strcmp("rate", cmd) == 0)
    {
        return kCommandTarget_SetUpdateRate(value);
    }
    // >> ks -taget mode [mode]
    if (strcmp("mode", cmd) == 0)
    {
        return kCommandTarget_SetMode(value);
    }
    // >> ks -taget get id
    // >> ks -taget get baud
    // >> ks -taget get rate
    // >> ks -taget get mode
    if (strcmp("get", cmd) == 0)
    {
        return kCommandTarget_GetValue(inputString);
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

uint32_t kCommandTarget_AutoBaudrate( void )
{
    const int searchOrder[BAUDRATE_LIST_MAX_LENS] = 
    {
        3, 4, 5, 6, 7, 8, 0, 1, 2
    };
    for (uint32_t i = 0; i < BAUDRATE_LIST_MAX_LENS; i++)
    {
        uint32_t id = 0;
        resetSerialBaudrate(baudratelist[searchOrder[i]]);
        if (kSerial_DeviceCheck(&id) == KS_OK)
        {
            klogd("  search baudrate = %d bps\n", s.cfg.baudrate);
            updateUartSetting(-1, -1);
            return KS_OK;
        }
    }

    return KS_ERROR;
}

uint32_t kCommandTarget_SetBaudrate( int32_t baudrate )
{
    if (baudrate < 0)
    {
        klogd("  >> set target baudrate error (%d)\n", baudrate);
        return KS_ERROR;
    }
    klogd("  >> set target baudrate %d bps ... ", baudrate);
    uint8_t param[2] = {KSCMD_R0_DEVICE_BAUDRATE, 4};
    kSerial_SendPacket(param, &baudrate, param[1], KS_R0);

    uint32_t id = 0;
    resetSerialBaudrate(baudrate);
    if (kSerial_DeviceCheck(&id) != KS_OK)
    {
        klogd("ERROR\n");
        return KS_ERROR;
    }
    klogd("OK\n");
    updateUartSetting(-1, -1);
    return KS_OK;
}

uint32_t kCommandTarget_SetUpdateRate( int32_t updaterate )
{
    if (updaterate < 0)
    {
        klogd("  >> set target update rate error (%d)\n", updaterate);
        return KS_ERROR;
    }
    klogd("  >> set target update rate %d Hz\n", updaterate);
    uint8_t param[2] = {KSCMD_R0_DEVICE_RATE, 4};
    kSerial_SendPacket(param, &updaterate, param[1], KS_R0);
    return KS_OK;
}

uint32_t kCommandTarget_SetMode( int32_t mode )
{
    if (mode < 0)
    {
        klogd("  >> set target mode error (%d)\n", mode);
        return KS_ERROR;
    }
    klogd("  >> set target mode %d\n", mode);
    kSerial_SendCommand(KS_R0, KSCMD_R0_DEVICE_MDOE, mode, NULL);
    return KS_OK;
}

uint32_t kCommandTarget_GetValue( const char *inputString )
{
    kserial_ack_t ack = {0};

    if (inputString != NULL)
    {
        uint32_t item = getTargetItem(inputString);
        if (item != KSCMD_R0_NULL)
        {
            klogd("  >> get target %s", inputString, item);
        }
        else
        {
            klogd("  >> get target %s ... not found\n", inputString);
        }
        kSerial_SendCommand(KS_R0, KSCMD_R0_DEVICE_GET, item, &ack);
        uint32_t *ptr32 = (uint32_t*)ack.data;
        switch (item)
        {
            case KSCMD_R0_DEVICE_ID:        klogd(" ... 0x%08X\n", ptr32[0]);   break;
            case KSCMD_R0_DEVICE_BAUDRATE:  klogd(" ... %d bps\n", ptr32[0]);   break;
            case KSCMD_R0_DEVICE_RATE:      klogd(" ... %d Hz\n", ptr32[0]);    break;
            case KSCMD_R0_DEVICE_MDOE:      klogd(" ... mode %d\n", ptr32[0]);  break;
        }
        return KS_OK;
    }
    klogd("  >> not enough input\n");
    return KS_ERROR;
}

uint32_t getTargetItem( const char *itemString )
{
    char item[MAX_COMMAND_SIZE] = {0};
    toLowercase(item, itemString);
    if (strcmp("id", item) == 0)
    {
        return KSCMD_R0_DEVICE_ID;
    }
    if (strcmp("baud", item) == 0)
    {
        return KSCMD_R0_DEVICE_BAUDRATE;
    }
    if (strcmp("rate", item) == 0)
    {
        return KSCMD_R0_DEVICE_RATE;
    }
    if (strcmp("mode", item) == 0)
    {
        return KSCMD_R0_DEVICE_MDOE;
    }
    return KSCMD_R0_NULL;
}

/*************************************** END OF FILE ****************************************/
