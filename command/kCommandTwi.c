/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommandTwi.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include "kSerial.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static uint8_t buf[256];

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_TwiScanDevice( void )
{
    // scan device address
    uint32_t devicenum;
    devicenum = kSerial_TwiScanDevice(buf);
    if (devicenum > 128)
    {
        klogd("  >> i2c secn device error\n");
        return KS_ERROR;
    }
    else
    {
        klogd("  >> found %d device ...", devicenum);
        for (uint32_t i = 0; i < devicenum; i++)
        {
            klogd(" %02X", buf[i]);
        }
        klogd("\n");
    }
    return KS_OK;
}

uint32_t kCommand_TwiScanRegister( char *addressString )
{
    uint32_t address = strtoul(addressString, NULL, 0);

    // scan device register
    if (kSerial_TwiScanRegister(address, buf) != KS_OK)
    {
        klogd("  >> i2c secn register error\n");
        return KS_ERROR;
    }
    else
    {
        klogd("  >> secn address = 0x%02X\n", address);
        klogd("\n");
        klogd("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        for (uint32_t i = 0; i < 256; i += 16)
        {
            klogd("  %02X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                i,
                buf[i +  0], buf[i +  1], buf[i +  2], buf[i +  3],
                buf[i +  4], buf[i +  5], buf[i +  6], buf[i +  7],
                buf[i +  8], buf[i +  9], buf[i + 10], buf[i + 11],
                buf[i + 12], buf[i + 13], buf[i + 14], buf[i + 15]
            );
        }
    }
    return KS_OK;
}

uint32_t kCommand_TwiRead( char *addressString, char *registerString, char *lenghtString )
{
    uint32_t slaveAddress = strtoul(addressString, NULL, 0);
    uint32_t deviceRegister = strtoul(registerString, NULL, 0);
    uint32_t readLength = strtoul(lenghtString, NULL, 0);
    readLength = (lenghtString != NULL) ? strtoul(lenghtString, NULL, 0) : 1;
    if (kSerial_TwiReadRegs(slaveAddress, deviceRegister, buf, readLength) != KS_OK)
    {
        klogd("  >> i2c read error\n");
        return KS_ERROR;
    }
    else
    {
        klogd(" ");
        for (uint32_t i = 0; i < readLength; i++)
        {
            klogd(" %02X", buf[i]);
        }
        klogd("\n");
    }
    return KS_OK;
}

uint32_t kCommand_TwiWrite( char *addressString, char *registerString, char **dataString, int length )
{
    uint32_t slaveAddress = strtoul(addressString, NULL, 0);
    uint32_t deviceRegister = strtoul(registerString, NULL, 0);
    klogd(" ");
    for (uint32_t i = 0; i < length; i++)
    {
        buf[i] = strtoul(dataString[i], NULL, 0);
        klogd(" %02X", buf[i]);
    }
    klogd("\n");
    kSerial_TwiWriteRegs(slaveAddress, deviceRegister, buf, length);
    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
