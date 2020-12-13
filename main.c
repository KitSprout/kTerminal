/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>

#include "serial.h"

#include "kSerial.h"
#include "kFile.h"
#include "kCommand.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/

#define FIRMWARE_VERSION    "v0.2"

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

uint32_t command;
uint32_t open = KS_CLOSE;
kfile_setting_t setting =
{
    .filename = DEFAULT_FILENAME,
    .port     = &s.port,
    .baudrate = &s.cfg.baudrate,
    .databits = &s.cfg.databits,
    .stopbits = &s.cfg.stopbits,
    .parity   = &s.cfg.parity,
    .flowctrl = &s.cfg.flowctrl
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t openSerialPort( void )
{
    if (s.isConnected == KS_TRUE)
    {
        return KS_BUSY;
    }
    // check comport
    if (cplist.num == 0)
    {
        klogd("  not available port\n");
        return KS_ERROR;
    }
    // open serial port
    if (Serial_OpenComport(&s) != KS_OK)
    {
        klogd("\n  open serial error (COM%d)\n", s.port);
        return KS_ERROR;
    }

    return KS_OK;
}

void closeSerialPort( void )
{
    Serial_CloseComport(&s);
    Serial_FreeComportList(&cplist);
}

uint32_t scanSerialPort( void )
{
    if (Serial_GetComportList(&cplist) != KS_OK)
    {
        printf("\n  scan comport error\n");
        return KS_ERROR;
    }
    return KS_OK;
}

uint32_t loadSettingFile( void )
{
    if (kFile_GetSetting(&setting) != KS_OK)
    {
        klogd("\n  get setting error\n");
        return KS_ERROR;
    }
    return KS_OK;
}

uint32_t setup( char **argv, uint32_t *command )
{
    // get current path
    kFile_GetFullPath(setting.path);
    // get command
    *command = kCommand_GetCommand(argv[1]);
    if (*command == COMMAND_HELP)
    {
        return KS_OK;
    }
    // scan comport
    if (scanSerialPort() != KS_OK)
    {
        return KS_ERROR;
    }
    // load setting
    if (loadSettingFile() != KS_OK)
    {
        return KS_ERROR;
    }

    return KS_OK;
}

int main( int argc, char **argv )
{
    if (setup(argv, &command) != KS_OK)
    {
        return KS_ERROR;
    }

    // command (without serial)
    switch (command)
    {
        case COMMAND_HELP:
        {
            return kCommand_Help();
        }
        case COMMAND_VERSION:
        {
            klogd("  >> version: %s\n", FIRMWARE_VERSION);
            return KS_OK;
        }
        case COMMAND_INFO:
        {
            return kCommand_GetSettingInformation();
        }
        case COMMAND_AUTO:
        {
            return kCommand_UartConfigureAutomatic();
        }
        case COMMAND_PORT:
        {
            if (argc < 3)
            {
                klogd("  >> not enough input arguments\n\n");
                kCommand_HelpPort();
                return KS_ERROR;
            }
            return kCommand_UartComportConfigure(argv[2], argv[3]);
        }
        case COMMAND_BAUD:
        {
            if (argc < 3)
            {
                klogd("  >> not enough input arguments\n\n");
                kCommand_HelpBaud();
                return KS_ERROR;
            }
            return kCommand_UartBaudrateConfigure(argv[2]);
        }
        case COMMAND_TERMINAL_AUTO:
        {
            klogd("  >> uart terminal mode\n");
            kCommand_UartConfigureAutomatic();
            if (openSerialPort() != KS_OK)
            {
                return KS_ERROR;
            }
            kCommand_UartTerminalProcess(NULL);
            break;
        }
        case COMMAND_ERROR:
        {
            klogd("  >> command not found\n");
            return KS_ERROR;
        }
    }

    // open serial port
    if (openSerialPort() != KS_OK)
    {
        return KS_ERROR;
    }

    // command (with serial)
    switch (command)
    {
        case COMMAND_TERMINAL:
        {
            klogd("  >> uart terminal mode\n");
            kCommand_UartTerminalProcess(NULL);
            break;
        }
        case COMMAND_DEBUG:
        {
            klogd("  >> debug mode\n");
            break;
        }
    }

    // close serial port
    closeSerialPort();

    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
