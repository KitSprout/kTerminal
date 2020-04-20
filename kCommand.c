/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kCommand.c
 *  @author  KitSprout
 *  @date    Mar-2020
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "kCommand.h"
#include "kFile.h"
#include "serial.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

extern kfile_setting_t setting;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_GetCommand( char *commandString )
{
    if (commandString == NULL)
    {
        return COMMAND_NULL;
    }
    else if (commandString[0] != '-')
    {
        return COMMAND_ERROR;
    }

    if ((strcmp("-h", commandString) == 0) || (strcmp("-H", commandString) == 0) ||
        (strcmp("-help", commandString) == 0) || (strcmp("-HELP", commandString) == 0))
    {
        return COMMAND_HELP;
    }
    // >> ks -info
    if (/*(strcmp("-i", commandString) == 0) || (strcmp("-I", commandString) == 0) || */
        (strcmp("-info", commandString) == 0) || (strcmp("-INFO", commandString) == 0))
    {
        return COMMAND_INFO;
    }
    // >> ks -path
    if (/*(strcmp("-i", commandString) == 0) || (strcmp("-I", commandString) == 0) || */
        (strcmp("-path", commandString) == 0) || (strcmp("-PATH", commandString) == 0))
    {
        return COMMAND_PATH;
    }
    // >> ks -auto
    if (/*(strcmp("-a", commandString) == 0) || (strcmp("-A", commandString) == 0) || */
        (strcmp("-auto", commandString) == 0) || (strcmp("-AUTO", commandString) == 0))
    {
        return COMMAND_AUTO;
    }
    // >> ks -list
    if (/*(strcmp("-l", commandString) == 0) || (strcmp("-L", commandString) == 0) || */
        (strcmp("-list", commandString) == 0) || (strcmp("-LIST", commandString) == 0))
    {
        return COMMAND_LIST;
    }
    // >> ks -port [PORT or COMx]
    // >> ks -port [PORT or COMx] [BAUDRATE]
    if (/*(strcmp("-p", commandString) == 0) || (strcmp("-P", commandString) == 0) || */
        (strcmp("-port", commandString) == 0) || (strcmp("-PORT", commandString) == 0))
    {
        return COMMAND_PORT;
    }
    // >> ks -baudrate list
    // >> ks -baudrate [BAUDRATE]
    if (/*(strcmp("-b", commandString) == 0) || (strcmp("-B", commandString) == 0) || */
        (strcmp("-baudrate", commandString) == 0) || (strcmp("-BAUDRATE", commandString) == 0))
    {
        return COMMAND_BAUDRATE;
    }
    // >> ks -uart
    if ((strcmp("-u", commandString) == 0) || (strcmp("-U", commandString) == 0) ||
        (strcmp("-uart", commandString) == 0) || (strcmp("-UART", commandString) == 0))
    {
        return COMMAND_UART;
    }
    // >> ks -i2c
    if ((strcmp("-i", commandString) == 0) || (strcmp("-I", commandString) == 0) ||
        (strcmp("-i2c", commandString) == 0) || (strcmp("-I2C", commandString) == 0))
    {
        return COMMAND_I2C;
    }
    // >> ks -kserial
    if ((strcmp("-k", commandString) == 0) || (strcmp("-K", commandString) == 0) ||
        (strcmp("-kserial", commandString) == 0) || (strcmp("-KSERIAL", commandString) == 0))
    {
        return COMMAND_KSERIAL;
    }
    // >> ks -blecondig
    if ((strcmp("-bt", commandString) == 0) || (strcmp("-BT", commandString) == 0) ||
        (strcmp("-btconfig", commandString) == 0) || (strcmp("-BTCONFIG", commandString) == 0))
    {
        return COMMAND_BTCONFIG;
    }
    // >> ks -debug
    if ((strcmp("-d", commandString) == 0) || (strcmp("-D", commandString) == 0) ||
        (strcmp("-debug", commandString) == 0) || (strcmp("-DEBUG", commandString) == 0))
    {
        return COMMAND_DEBUG;
    }

    return COMMAND_ERROR;
}

uint32_t kCommand_Help( void )
{
    printf("\n");
    printf("  -INFO                             ... show configuration\n");
    printf("  -AUTO                             ... select available port automatically\n");
    printf("  -LIST                             ... show serial comport list\n");
    printf("  -PORT [PORT or COMx]              ... serial comport setting\n");
    printf("  -PORT [PORT or COMx] [BAUDRATE]   ... serial comport and baudrate setting\n");
    printf("  -BAUDRATE LIST                    ... show internal baudrate list\n");
    printf("  -BAUDRATE [BAUDRATE]              ... serial baudrate setting\n");

    printf("\n  *** TODO\n");
    printf("  -UART                             ... into uart terminal mode\n");
    printf("  -I2C                              ... into i2c mode\n");
    printf("  -KSERIAL                          ... into kserial mode\n");
    return KS_OK;
}

uint32_t kCommand_GetSettingInformation( void )
{
    printf("\n");
    // printf("  Path                              ... %s\n", setting.path);
    printf("  Port                              ... COM%d\n", *setting.port);
    printf("  Baud Rate                         ... %d bps\n", *setting.baudrate);
    printf("  Data Bits                         ... %d\n", *setting.databits);
    printf("  Parity                            ... %s\n", serial_parity_string[*setting.parity]);
    printf("  Stop Bits                         ... %d\n", *setting.stopbits);
    printf("  Flow Control                      ... %s\n", serial_flowctrl_string[*setting.flowctrl]);
    return KS_OK;
}

uint32_t kCommand_UartConfigureAutomatic( void )
{
    uint32_t status;
    if (cplist.num == 0)
    {
        printf("  not available comport\n");
        return KS_ERROR;
    }
    s.port = cplist.port[0];
    s.cfg.baudrate = DEFAULT_BAUDRATE;
    status = kFile_UpdateSetting(&setting);
    if (status != KS_OK)
    {
        printf("  update %s failed\n", setting.filename);
        return KS_ERROR;
    }
    printf("  >> set COM%d, baudrate %d bps\n", s.port, s.cfg.baudrate);
    return KS_OK;
}

uint32_t kCommand_UartGetComportList( void )
{
    uint32_t status;
    // print list
    if (cplist.num == 0)
    {
        printf("  not available comport\n");
        return KS_ERROR;
    }
    printf("\n");
    for (uint32_t i = 0; i < cplist.num; i++)
    {
        printf("  [%d] COM%d\t(%s)\n", i + 1, cplist.port[i], cplist.info[i]);
    }
    printf("\n");
    // select port
    uint32_t portSelect = getch();
    uint32_t port = portSelect - '0';
    if ((portSelect == 27) || (portSelect == '0') || (portSelect == 'q') || (portSelect == 'Q'))
    {
        printf("  >> exit\n");
        return KS_OK;
    }
    else if ((portSelect < 48) || (portSelect > 57))
    {
        printf("  wrong input ... \"%c\"(%d)\n", portSelect, portSelect);
        return KS_ERROR;
    }
    if (port > cplist.num)
    {
        printf("  not in list ... %d\n", port);
        return KS_ERROR;
    }
    s.port = cplist.port[port - 1];
    status = kFile_UpdateSetting(&setting);
    if (status != KS_OK)
    {
        printf("  update %s failed\n", setting.filename);
        return KS_ERROR;
    }
    printf("  >> select COM%d, baudrate %d bps\n", s.port, s.cfg.baudrate);

    return KS_OK;
}

uint32_t kCommand_UartComportConfigure( char *portString, char *baudrateString )
{
    uint32_t status;
    int port, baudrate;

    char sport[8] = {0};
    uint32_t lens = strlen(portString);
    if (lens > 3)
    {
        for (uint32_t i = 3; i < lens; i++)
        {
            sport[i-3] = portString[i];
        }
    }
    else
    {
        sport[0] = portString[0];
        sport[1] = portString[1];
        sport[2] = portString[2];
    }
    port = strtoul(sport, NULL, 0);

    // setting mode
    if (baudrateString != NULL)
    {
        baudrate = strtoul(baudrateString, NULL, 0);
        if (baudrate < 1200)
        {
            printf("  wrong baudrate ... %d bps\n", baudrate);
            return KS_ERROR;
        }
        s.cfg.baudrate = baudrate;
    }

    // update setting file
    s.port = port;
    printf("  >> set COM%d, baudrate %d bps\n", s.port, s.cfg.baudrate);
    status = kFile_UpdateSetting(&setting);
    if (status != KS_OK)
    {
        printf("  update %s failed\n", setting.filename);
        return KS_ERROR;
    }

    return KS_OK;
}

uint32_t kCommand_UartBaudrateConfigure( char *baudrateString )
{
    uint32_t status;
    int baudrate;
    // select mode
    if ((strcmp("list", baudrateString) == 0) || (strcmp("LIST", baudrateString) == 0))
    {
        // print list
        printf("\n");
        for (uint32_t i = 0; i < BAUDRATE_LIST_MAX_LENS; i++)
        {
            printf("  [%2d] %7d bps\n", i + 1, baudratelist[i]);
        }
        printf("\n");
        // select port
        uint32_t baudSelect = getch();
        baudrate = baudSelect - '0';
        if ((baudSelect == 27) || (baudSelect == '0') || (baudSelect == 'q') || (baudSelect == 'Q'))
        {
            printf("  >> exit\n");
            return KS_OK;
        }
        else if ((baudSelect < 48) || (baudSelect > 57))
        {
            printf("  wrong input ... \"%c\"(%d)\n", baudSelect, baudSelect);
            return KS_ERROR;
        }
        if (baudrate > BAUDRATE_LIST_MAX_LENS)
        {
            printf("  not in list ... %d\n", baudrate);
            return KS_ERROR;
        }
        s.cfg.baudrate = baudratelist[baudrate - 1];
        status = kFile_UpdateSetting(&setting);
        if (status != KS_OK)
        {
            printf("  update %s failed\n", setting.filename);
            return KS_ERROR;
        }
        printf("  >> select baudrate %d bps\n", s.cfg.baudrate);
        return KS_OK;
    }

    // setting mode
    baudrate = strtoul(baudrateString, NULL, 0);
    if (baudrate < 1200)
    {
        printf("  wrong baudrate ... %d bps\n", baudrate);
        return KS_ERROR;
    }
    printf("  >> set baudrate %d bps\n", baudrate);
    s.cfg.baudrate = baudrate;
    status = kFile_UpdateSetting(&setting);
    if (status != KS_OK)
    {
        printf("  update %s failed\n", setting.filename);
        return KS_ERROR;
    }

    return KS_OK;
}

uint32_t kCommand_UartProcess( char **argv )
{
    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
