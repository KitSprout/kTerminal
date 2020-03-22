
/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>

#include "serial.h"
#include "kFile.h"
#include "kCommand.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef enum
{
    MODE_NULL = 0,
    MODE_UART = COMMAND_UART,
    MODE_I2C = COMMAND_I2C,
    MODE_KSERIAL = COMMAND_KSERIAL,
    MODE_DEBUG = COMMAND_DEBUG

} kterminal_e;

/* Variables -------------------------------------------------------------------------------*/

uint32_t process = MODE_NULL;
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

void terminal( void );

/* Functions -------------------------------------------------------------------------------*/

uint32_t openSerial( void )
{
    // check comport
    if (cplist.num == 0)
    {
        printf("  not available port\n");
        return KS_ERROR;
    }
    // open serial port
    if (Serial_OpenComport(&s) != KS_OK)
    {
        printf("\n  open serial error (COM%d)\n", s.port);
        return KS_ERROR;
    }
    return KS_OK;
}

void closeSerial( void )
{
    Serial_CloseComport(&s);
    Serial_FreeComportList(&cplist);
}

int main( int argc, char **argv )
{
    uint32_t command;

    // get current path
    kFile_GetPath(&setting);

    // scan comport
    if (Serial_GetComportList(&cplist) != KS_OK)
    {
        printf("\n  scan comport error\n");
        return KS_ERROR;
    }

    // get setting
    if (kFile_GetSetting(&setting) != KS_OK)
    {
        printf("\n  get setting error\n");
        return KS_ERROR;
    }

    // get command
    command = kCommand_GetCommand(argv[1]);

    // command (without serial)
    switch (command)
    {
        case COMMAND_HELP:
        {
            return kCommand_Help();
        }
        case COMMAND_INFO:
        {
            return kCommand_GetSettingInformation();
        }
        case COMMAND_PATH:
        {
            printf("  >> Path: %s\n", setting.path);
            return KS_OK;
        }
        case COMMAND_AUTO:
        {
            return kCommand_UartConfigureAutomatic();
        }
        case COMMAND_LIST:
        {
            return kCommand_UartGetComportList();
        }
        case COMMAND_PORT:
        {
            if (argc < 3)
            {
                printf("  not enough input arguments\n\n");
                printf("  -PORT [PORT or COMx]              ... serial comport setting\n");
                printf("  -PORT [PORT or COMx] [BAUDRATE]   ... serial comport and baudrate setting\n");
                return KS_ERROR;
            }
            return kCommand_UartComportConfigure(argv[2], argv[3]);
        }
        case COMMAND_BAUDRATE:
        {
            if (argc < 3)
            {
                printf("  not enough input arguments\n\n");
                printf("  -BAUDRATE LIST                    ... show internal baudrate list\n");
                printf("  -BAUDRATE [BAUDRATE]              ... serial baudrate setting\n");
                return KS_ERROR;
            }
            return kCommand_UartBaudrateConfigure(argv[2]);
        }
        case COMMAND_ERROR:
        {
            printf("  command not found\n");
            return KS_ERROR;
        }
    }

    // open serial port
    if (openSerial() != KS_OK)
    {
        return KS_ERROR;
    }

    // command (with serial)
    switch (command)
    {
        case COMMAND_UART:
        {
            printf("  >> uart terminal mode\n");
            process = MODE_UART;
            break;
        }
        case COMMAND_I2C:
        {
            printf("  >> i2c mode\n");
            // TODO: check device
            process = MODE_I2C;
            break;
        }
        case COMMAND_KSERIAL:
        {
            printf("  >> kserial mode\n");
            // TODO: check device
            process = MODE_KSERIAL;
            break;
        }
        case COMMAND_DEBUG:
        {
            printf("  >> debug mode\n");
            process = MODE_DEBUG;
            break;
        }
    }

    // run process
    terminal();

    // close serial port
    closeSerial();

    return KS_OK;
}

char getKey( void )
{
    char ch = 0;
    if (_kbhit())
    {
        ch = _getch();
    }
    return ch;
}

void terminal( void )
{
    #define MAX_RECV_LENS 1024
    while (process == MODE_UART)
    {
        // recv
        uint8_t recv[MAX_RECV_LENS] = {0};
        Serial_RecvData(&s, recv, MAX_RECV_LENS);
        printf("%s", recv);

        switch (getKey())
        {
            // case 'Q':
            // case 'q':
            // {
            //     process = MODE_NULL;
            //     printf("\n\n  >> exit\n");
            //     break;
            // }
            case 19:    // ctrl + S
            {
                printf("\n\n  >> save file\n");
                break;
            }
        }
    }

    while (process == MODE_DEBUG)
    {
        char key = getKey();
        if (key != 0)
        {
            printf("%c", key);
            if (key == 'H')
            {
                printf("\033[1A");
                printf("\033[1D");
            }
            else if (key == 'P')
            {
                printf("\033[1B");
                printf("\033[1D");
            }
            else if (key == 'q')
            {
                printf("\033[?25l");
            }
            else if (key == 'r')
            {
                printf("\033[31m red \033[0m");
            }
            // printf("%c, %d\n", key, key);
        }
        // switch (key)
        // {
        //     case 17:    // ctrl + Q, 
        //     {
        //         process = MODE_NULL;
        //         printf("\n\n  >> exit\n");
        //         break;
        //     }
        //     case 19:    // ctrl + S, save file
        //     {
        //         printf("\n\n  >> save file\n");
        //         break;
        //     }
        // }
    }
}

/*************************************** END OF FILE ****************************************/
