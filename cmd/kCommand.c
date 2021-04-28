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
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "serial.h"
#include "kStatus.h"
#include "kCommand.h"
#include "kString.h"
#include "kSerial.h"
#include "kFile.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

extern kfile_setting_t setting;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kCommand_GetCommand( const char *command )
{
    char cmd[MAX_COMMAND_SIZE] = {0};
    if (toLowercase(cmd, command) == KS_ERROR)
    {
        return COMMAND_NULL;
    }
    else if (command[0] != '-')
    {
        return COMMAND_ERROR;
    }
    // >> ks -help
    if ((strcmp("-h", cmd) == 0) || (strcmp("-help", cmd) == 0))
    {
        return COMMAND_HELP;
    }
    // >> ks -version
    if ((strcmp("-v", cmd) == 0) || (strcmp("-version", cmd) == 0))
    {
        return COMMAND_VERSION;
    }
    // >> ks -info
    if (/*(strcmp("-i", cmd) == 0) ||*/ (strcmp("-info", cmd) == 0))
    {
        return COMMAND_INFO;
    }
    // >> ks -auto
    if ((strcmp("-a", cmd) == 0) || (strcmp("-auto", cmd) == 0))
    {
        return COMMAND_AUTO;
    }
    // >> ks -port list
    // >> ks -port [PORT/COMx]
    // >> ks -port [PORT/COMx] [BAUDRATE]
    if (/*(strcmp("-p", cmd) == 0) ||*/ (strcmp("-port", cmd) == 0))
    {
        return COMMAND_PORT;
    }
    // >> ks -baud list
    // >> ks -baud [BAUDRATE]
    if (/*(strcmp("-b", cmd) == 0) ||*/ (strcmp("-baud", cmd) == 0))
    {
        return COMMAND_BAUD;
    }
    // >> ks -terminal
    if ((strcmp("-t", cmd) == 0) || (strcmp("-terminal", cmd) == 0))
    {
        return COMMAND_TERMINAL;
    }
    // >> ks -ta
    if (strcmp("-ta", cmd) == 0)
    {
        return COMMAND_TERMINAL_AUTO;
    }
    // >> ks -kserial
    if ((strcmp("-rs", cmd) == 0) || (strcmp("-kserial", cmd) == 0))
    {
        return COMMAND_KSERIAL;
    }
    // >> ks -target check
    // >> ks -target baud [baud]
    // >> ks -target rate [rate]
    // >> ks -target mode [mode]
    if ((strcmp("-tg", cmd) == 0) || strcmp("-target", cmd) == 0)
    {
        return COMMAND_TARGET;
    }
    // >> ks -scan
    if (/*(strcmp("-s", cmd) == 0) ||*/ strcmp("-scan", cmd) == 0)
    {
        return COMMAND_SCAN;
    }
    // >> ks -reg [address]
    if (/*(strcmp("-r", cmd) == 0) ||*/ strcmp("-reg", cmd) == 0)
    {
        return COMMAND_REG;
    }
    // >> ks -read [address] [register]
    // >> ks -read [address] [register] [lenght]
    if ((strcmp("-r", cmd) == 0) || strcmp("-read", cmd) == 0)
    {
        return COMMAND_READ;
    }
    // >> ks -write [address] [register] [data]
    if ((strcmp("-w", cmd) == 0) || strcmp("-write", cmd) == 0)
    {
        return COMMAND_WRITE;
    }
    // >> ks -debug
    if ((strcmp("-d", cmd) == 0) || (strcmp("-debug", cmd) == 0))
    {
        return COMMAND_DEBUG;
    }
    return COMMAND_ERROR;
}

uint32_t kCommand_Help( void )
{
    klogd("\n");
    klogd("  **** System Command\n");
    kCommand_HelpInfo();
    kCommand_HelpVersion();
    kCommand_HelpAuto();
    kCommand_HelpPort();
    kCommand_HelpBaud();
    kCommand_HelpTerminal();
    kCommand_HelpKSerial();
    klogd("\n");
    klogd("  **** Target Command\n");
    kCommand_HelpTarget();
    klogd("\n");
    klogd("  **** I2C Command\n");
    kCommand_HelpScan();
    kCommand_HelpReg();
    kCommand_HelpRead();
    kCommand_HelpWrite();

    // TODO: 
    // set target baudrate
    // set target sampling rate
    // set target mode
    return KS_OK;
}

uint32_t kCommand_GetVersion( const char *version )
{
    klogd("  >> version: %s\n", version);
    return KS_OK;
}

uint32_t kCommand_GetSettingInformation( void )
{
    klogd("\n");
    klogd("  Path                               ... %s\n", setting.path);
    klogd("  Port                               ... COM%d\n", *setting.port);
    klogd("  Baud Rate                          ... %d bps\n", *setting.baudrate);
    klogd("  Data Bits                          ... %d\n", *setting.databits);
    klogd("  Parity                             ... %s\n", serial_parity_string[*setting.parity]);
    klogd("  Stop Bits                          ... %d\n", *setting.stopbits);
    klogd("  Flow Control                       ... %s\n", serial_flowctrl_string[*setting.flowctrl]);
    return KS_OK;
}

static char getKey( void )
{
    char ch = 0;
    if (_kbhit())
    {
        ch = _getch();
    }
    return ch;
}

static int menuSelect( uint32_t maxlens )
{
    uint32_t select = getch();
    uint32_t value = select - '0';
    if (value > maxlens)
    {
        klogd("  not in list ... %d\n", value);
        return -1;
    }
    if ((select == 27) || (select == 'q') || (select == 'Q'))
    {
        klogd("  >> exit\n");
        return -1;
    }
    else if ((select < 48) || (select > 57))
    {
        klogd("  wrong input ... \"%c\"(%d)\n", select, select);
        return -1;
    }
    else if (select == '0')
    {
        klogd("  >> default\n");
    }
    return value;
}

uint32_t updateUartSetting( int port, int baudrate )
{
    if (port != -1)
    {
        s.port = port;
    }
    if (baudrate != -1)
    {
        s.cfg.baudrate = baudrate;
    }
    klogd("  >> set COM%d, baudrate %d bps\n", s.port, s.cfg.baudrate);
    if (kFile_UpdateSetting(&setting) != KS_OK)
    {
        klogd("  update %s failed!!!\n", setting.filename);
        return KS_ERROR;
    }
    return KS_OK;
}

uint32_t kCommand_UartConfigureAutomatic( void )
{
    if (cplist.num == 0)
    {
        klogd("  not available comport\n");
        return KS_ERROR;
    }
#if 0
    return updateUartSetting(cplist.port[0], DEFAULT_BAUDRATE);
#else
    return updateUartSetting(cplist.port[0], -1);
#endif
}

uint32_t kCommand_UartComportConfigure( const char *portString, const char *baudrateString )
{
    int port = -1, baudrate = -1;

    if (strcmpLowercase("list", portString) == KS_OK)
    {
        if (cplist.num == 0)
        {
            klogd("  not available comport\n");
            return KS_ERROR;
        }
        // print list
        klogd("\n");
        for (uint32_t i = 0; i < cplist.num; i++)
        {
            klogd("  [%d] COM%d\t(%s)\n", i + 1, cplist.port[i], cplist.info[i]);
        }
        klogd("\n");
        // select port
        port = menuSelect(cplist.num);
        if (port < 0)
        {
            return KS_ERROR;
        }
        else
        {
            port = (port != 0) ? cplist.port[port - 1] : cplist.port[0];
        }
    }
    else
    {
        char sport[16] = {0};
        strcpy(sport, portString);
        sport[3] = 0;
        if (strcmpLowercase("com", sport) == KS_OK)
        {
            strcpy(sport, &portString[3]);
            port = strtoul(sport, NULL, 0);
        }
        else
        {
            port = strtoul(portString, NULL, 0);
        }
        if (baudrateString != NULL)
        {
            baudrate = strtoul(baudrateString, NULL, 0);
            if (baudrate <= 0)
            {
                baudrate = -1;
            }
        }
    }
    if (port <= 0)
    {
        klogd("  wrong input\n");
        return KS_ERROR;
    }
    return updateUartSetting(port, baudrate);
}

uint32_t kCommand_UartBaudrateConfigure( const char *baudrateString )
{
    int baudrate = -1;
    if (strcmpLowercase("list", baudrateString) == KS_OK)
    {
        // print list
        klogd("\n");
        for (uint32_t i = 0; i < BAUDRATE_LIST_MAX_LENS; i++)
        {
            klogd("  [%2d] %7d bps\n", i + 1, baudratelist[i]);
        }
        klogd("\n");
        // select port
        baudrate = menuSelect(BAUDRATE_LIST_MAX_LENS);
        if (baudrate < 0)
        {
            return KS_ERROR;
        }
        else
        {
            baudrate = (baudrate != 0) ? baudratelist[baudrate - 1] : DEFAULT_BAUDRATE;
        }
    }
    else
    {
        baudrate = strtoul(baudrateString, NULL, 0);
    }
    if (baudrate <= 0)
    {
        klogd("  wrong input\n");
        return KS_ERROR;
    }
    return updateUartSetting(-1, baudrate);
}

#define UART_RECV_MAX_BUFFER_SIZE   (8 * 1024)
uint32_t kCommand_UartTerminalProcess( const char **argv )
{
    char buf[UART_RECV_MAX_BUFFER_SIZE] = {0};
    uint32_t loop = KS_TRUE;
    uint32_t lens;

    kSerial_RecvFlush();
    while (loop)
    {
        lens = kSerial_Recv((uint8_t*)buf, UART_RECV_MAX_BUFFER_SIZE);
        if (lens != 0)
        {
            kputs(buf, lens);
        }
        uint8_t key = getKey();
        if (key)
        {
            switch (key)
            {
                case 17:    // ctrl + q
                {
                    klogd("\n  >> exit\n");
                    loop = KS_FALSE;
                    break;
                }
                case 19:    // ctrl + S
                {
                    // TODO: save log
                    klogd("\n  >> save file\n");
                    kSerial_Delay(100);
                    break;
                }
                default:
                {
                    kSerial_SendByte(key);
                }
            }
        }
    }

    return KS_OK;
}

uint32_t kCommand_UartKSerialRecv( const char **argv )
{
    char logbuf[1024] = {0};

    uint32_t loop = KS_TRUE;

    int16_t bytes[KSERIAL_MAX_PACKET_LENS >> 1];
    uint32_t index = 0, count = 0, total = 0;
    kserial_packet_t pk = {
        .param = {0},
        .type = 0,
        .lens = 0,
        .nbyte = 0,
        .data = bytes
    };

    uint32_t ts = 0, tn = 0;
    float packetFreq;

    while (loop)
    {
        kSerial_ContinuousRead(&pk, &index, &count, &total);
        if (count != 0)
        {
            ts = tn;
            tn = ((int16_t*)pk.data)[0]*1000 + ((int16_t*)pk.data)[1];
            packetFreq = 1000.0 / (tn - ts);

            uint32_t lens;
            lens = sprintf(logbuf, "[%6d][%3d][%s][%02X:%02X][%4dHz] ", total, count, KS_TYPE_STRING[pk.type], pk.param[0], pk.param[1], (int32_t)packetFreq);
            for (uint32_t i = 0; i < pk.lens; i++)
            {
                switch (pk.type)
                {
                    case KS_I8:     lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((int8_t*)pk.data)[i]);    break;
                    case KS_U8:     lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((uint8_t*)pk.data)[i]);   break;
                    case KS_I16:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((int16_t*)pk.data)[i]);   break;
                    case KS_U16:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((uint16_t*)pk.data)[i]);  break;
                    case KS_I32:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((int32_t*)pk.data)[i]);   break;
                    case KS_U32:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((uint32_t*)pk.data)[i]);  break;
                    case KS_I64:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((int64_t*)pk.data)[i]);   break;
                    case KS_U64:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((uint64_t*)pk.data)[i]);  break;
                    case KS_F32:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((float*)pk.data)[i]);     break;
                    case KS_F64:    lens += sprintf(&logbuf[lens], KS_TYPE_FORMATE[pk.type], ((double*)pk.data)[i]);    break;
                }
                if (i != (pk.lens - 1))
                {
                    logbuf[lens++] = ',';
                }
            }
            logbuf[lens++] = 0;
            puts(logbuf);
        }
        switch (getKey())
        {
            case 17:    // ctrl + q
            {
                klogd("\n  >> exit\n");
                loop = KS_FALSE;
                break;
            }
            case 19:    // ctrl + S
            {
                // TODO: save log
                klogd("\n  >> save file\n");
                kSerial_Delay(100);
                break;
            }
        }
    }
    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
