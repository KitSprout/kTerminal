
/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>

#include "serial.h"
#include "kSerial.h"
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
    MODE_BTCONFIG = COMMAND_BTCONFIG,
    MODE_DEBUG = COMMAND_DEBUG

} kterminal_e;

/* Variables -------------------------------------------------------------------------------*/

uint32_t command;
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

void runTerminal( void );

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
    // get current path
    kFile_GetPath(&setting);

    // get command
    command = kCommand_GetCommand(argv[1]);
    switch (command)
    {
        case COMMAND_HELP:
        {
            return kCommand_Help();
        }
        case COMMAND_PATH:
        {
            printf("  >> Path: %s\n", setting.path);
            return KS_OK;
        }
    }

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

    // command (without serial)
    switch (command)
    {
        case COMMAND_INFO:
        {
            return kCommand_GetSettingInformation();
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
        case COMMAND_BTCONFIG:
        {
            printf("  >> bluetooth config mode\n");
            // TODO: [1] baudrate setting      ... ks -bt 115200
            // TODO: [2] show at command list  ... ctrl+l
            // TODO: [2] send with \r\n        ... ks -bt 115200 \rn
            // resetting baudrate
            Serial_SetBaudrate(&s, 115200);
            if (Serial_OpenComport(&s) != KS_OK)
            {
                printf("\n  open serial error (COM%d)\n", s.port);
                return KS_ERROR;
            }
            process = MODE_BTCONFIG;
            break;
        }
        case COMMAND_HC05CONFIG:
        {
            printf("  >> bluetooth hc05 config mode\n");
            Serial_SetBaudrate(&s, 38400);
            if (Serial_OpenComport(&s) != KS_OK)
            {
                printf("\n  open serial error (COM%d)\n", s.port);
                return KS_ERROR;
            }
            process = MODE_BTCONFIG;
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
    if (process != MODE_NULL)
    {
        runTerminal();
    }

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






#define ENABLE_PRINT_THREAD (0U)

#if ENABLE_PRINT_THREAD
DWORD WINAPI log_thread( void* data );
DWORD WINAPI terminal_thread( void* data );
#else
#define MAX_RECV_LENS 1024
uint8_t rbuf[MAX_RECV_LENS] = {0};
#endif


#define MAX_KSERIAL_PACKET_LENS     (1024)
#define PACKET_RECV_BUFFER_LENS     (64 * 1024)
kserial_packet_t kspacket[MAX_KSERIAL_PACKET_LENS] = {0};
uint8_t kspacketbuff[PACKET_RECV_BUFFER_LENS] = {0};
kserial_t ks =
{
    .size = PACKET_RECV_BUFFER_LENS,
    .count = 0,
    .buffer = kspacketbuff,
    .packet = kspacket
};
uint32_t pocketLens = 0;
float pocketFreq = 0;


static float getfrequence( uint32_t lens, uint32_t time, uint32_t count )
{
    static uint64_t kslensLast = 0;
    static uint64_t ksTimeLast = 0;
    static float frequence = 0;
    if ((time - ksTimeLast) >= count)
    {
        frequence = ((float)lens - kslensLast) / (time - ksTimeLast) * 1000.0f;
        kslensLast = lens;
        ksTimeLast = time;
    }
    return frequence;
}

void runTerminal( void )
{
#if ENABLE_PRINT_THREAD
    HANDLE terminal_handle = CreateThread(NULL, 0, terminal_thread, NULL, CREATE_SUSPENDED, NULL);
    SetThreadPriority(terminal_handle, THREAD_PRIORITY_ABOVE_NORMAL);
    ResumeThread(terminal_handle);
    WaitForSingleObject(terminal_handle, INFINITE);
    CloseHandle(terminal_handle);
#else
    while (process == MODE_UART)
    {
        // recv data
        char key = getKey();
        switch (key)
        {
            // case 'Q':
            // case 'q':
            // {
            //     process = MODE_NULL;
            //     printf("\n\n  >> exit\n");
            //     break;
            // }
            case 0:
            {
                break;
            }
            case 19:    // ctrl + S
            {
                printf("\n\n  >> save file\n");
                break;
            }
            default:
            {
                // Serial_RecvData(&s, rbuf, MAX_RECV_LENS);
                // printf("%s", rbuf);
                Serial_SendByte(&s, key);
            }
        }
    }

    while (process == MODE_KSERIAL)
    {
        uint32_t idx = 0;
        uint32_t pocketCount = kSerial_Read(&ks);
        const char typeformat[16][8] = {
            "%4d", "%6d", "%11d", "%20d",
            "%4d", "%6d", "%11d", "%20d",
            "",    "%.6f", "%.6f", "%.6f",
            "", "", "", ""
        };
        while (pocketCount)
        {
            char sbuf[1024] = {0};
            int16_t bytes[512];
            uint8_t pkparam[2];
            uint32_t pktype;
            uint32_t pklens;
            kSerial_GetPacketData(ks.packet, bytes, idx);
            pkparam[0] = ks.packet[idx].param[0];
            pkparam[1] = ks.packet[idx].param[1];
            pktype = ks.packet[idx].type;
            pklens = ks.packet[idx].lens;
            pocketLens++;
            pocketFreq = getfrequence(pocketLens, bytes[0]*1000 + bytes[1], 500);

            uint32_t lens = 0;
            lens += sprintf(&sbuf[lens], "[%6d][%3d][%s][%02X:%02X][%4d Hz] ", pocketLens, pocketCount, KS_TYPE_STRING[pktype], pkparam[1], pkparam[0], (int32_t)pocketFreq);
            void *pdata = bytes;
            for (uint32_t i = 0; i < pklens; i++)
            {
                switch (pktype)
                {
                    case KS_I8:     lens += sprintf(&sbuf[lens], typeformat[pktype], ((int8_t*)pdata)[i]);     break;
                    case KS_U8:     lens += sprintf(&sbuf[lens], typeformat[pktype], ((uint8_t*)pdata)[i]);    break;
                    case KS_I16:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((int16_t*)pdata)[i]);    break;
                    case KS_U16:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((uint16_t*)pdata)[i]);   break;
                    case KS_I32:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((int32_t*)pdata)[i]);    break;
                    case KS_U32:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((uint32_t*)pdata)[i]);   break;
                    case KS_I64:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((int64_t*)pdata)[i]);    break;
                    case KS_U64:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((uint64_t*)pdata)[i]);   break;
                    case KS_F32:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((float*)pdata)[i]);      break;
                    case KS_F64:    lens += sprintf(&sbuf[lens], typeformat[pktype], ((double*)pdata)[i]);     break;
                }
                if (i != (pklens - 1))
                {
                    sbuf[lens++] = ',';
                }
            }
            puts(sbuf);

            if (++idx == pocketCount)
            {
                break;
            }
        }

        switch (getKey())
        {
            case 'Q':
            case 'q':
            {
                process = MODE_NULL;
                printf("\n\n  >> exit\n");
                break;
            }
            case 19:    // ctrl + S
            {
                printf("\n\n  >> save file\n");
                break;
            }
        }
    }

    while (process == MODE_BTCONFIG)
    {
        #define BT_BUFFER_LENS  (64U)
        uint32_t sendIdx = 0;
        uint8_t sendBuf[BT_BUFFER_LENS] = {0};
        uint8_t recvBuf[BT_BUFFER_LENS] = {0};
        while (1)
        {
            // recv data
            char key = getKey();
            if (key == 13)
            {
                printf("\n>> ");
                if (command == COMMAND_HC05CONFIG)
                {
                    sendBuf[sendIdx++] = '\r';
                    sendBuf[sendIdx++] = '\n';
                }

                // Serial_Flush(&s);
                Serial_SendData(&s, sendBuf, sendIdx);
                Serial_Delay(800);
                uint32_t nbytes = Serial_RecvData(&s, recvBuf, BT_BUFFER_LENS);
                recvBuf[nbytes] = 0;
                printf("%s\n\n", recvBuf);
                sendIdx = 0;
            }
            else if (key == 8)  // back
            {
                sendIdx--;
                const char back[4] = {8, ' ', 8, 0};
                printf("%s", back);
            }
            else if (key == 0x0C)  // ctrl + l
            {
                sendIdx = 0;
                printf("\n\n");
                printf("  >> AT Command List\n\n");
                printf("  AT+VERSION, Get the AT version\n");
                printf("  AT+NAMEname, set device name to ""name""\n");
                printf("  AT+PINxxxx, set password to ""xxxx""\n");
                printf("  AT+BAUD4, set baudrate to 9600\n");
                printf("  AT+BAUD5, set baudrate to 19200\n");
                printf("  AT+BAUD6, set baudrate to 38400\n");
                printf("  AT+BAUD7, set baudrate to 57600\n");
                printf("  AT+BAUD8, set baudrate to 115200\n");
                printf("  AT+BAUD9, set baudrate to 230400\n");
                printf("  AT+BAUDA, set baudrate to 460800\n");
                printf("  AT+BAUDB, set baudrate to 921600\n");
                printf("  AT+BAUDC, set baudrate to 1382400\n");
                printf("\n\n");
            }
            else if (key != 0)
            {
                sendBuf[sendIdx++] = key;
                printf("%c", key);
            }
        }
    }

    while (process == MODE_DEBUG)
    {
        int16_t cnt = 0;
        while (1)
        {
            switch (getKey())
            {
                case '`':
                {
                    Serial_SendByte(&s, '`');
                    break;
                }
                case 's':
                case 'S':
                {
                    cnt++;
                    kSerial_SendPacket(NULL, &cnt, 1, KS_I16);

                    uint32_t bytes;
                    uint8_t packet[256] = {0};
                    bytes = kSerial_Pack(packet, NULL, KS_I16, 2, &cnt);
                    printf("cnt = %7d ... ", cnt);
                    for (uint32_t i = 0; i < bytes; i++) {
                        printf("%02X ", packet[i]);
                    }
                    printf("\n");
                    break;
                }
                case 19:    // ctrl + S, save file
                {
                    printf("\n\n  >> save file\n");
                    break;
                }
            }
        }
    }
    // while (process == MODE_DEBUG)
    // {
    //     char key = getKey();
    //     if (key != 0)
    //     {
    //         printf("%c", key);
    //         if (key == 'H')
    //         {
    //             printf("\033[1A");
    //             printf("\033[1D");
    //         }
    //         else if (key == 'P')
    //         {
    //             printf("\033[1B");
    //             printf("\033[1D");
    //         }
    //         else if (key == 'q')
    //         {
    //             printf("\033[?25l");
    //         }
    //         else if (key == 'r')
    //         {
    //             printf("\033[31m red \033[0m");
    //         }
    //         // printf("%c, %d\n", key, key);
    //     }
    //     // switch (key)
    //     // {
    //     //     case 17:    // ctrl + Q, 
    //     //     {
    //     //         process = MODE_NULL;
    //     //         printf("\n\n  >> exit\n");
    //     //         break;
    //     //     }
    //     //     case 19:    // ctrl + S, save file
    //     //     {
    //     //         printf("\n\n  >> save file\n");
    //     //         break;
    //     //     }
    //     // }
    // }
#endif
}

/*************************************** END OF FILE ****************************************/



#if ENABLE_PRINT_THREAD

volatile uint32_t printlens = 0;
char printbuf[1024*1024] = {0};

uint32_t stringlens = 0;
char stringbuf[1024*1024] = {0};

DWORD WINAPI log_thread( void* data )
{
    while (process != MODE_NULL)
    {
        if (printlens != 0)
        {
            puts(printbuf);
            printlens = 0;
        }
    }
    return 0;
}

#define MAX_RECV_LENS 1024
uint32_t lens;
uint8_t rbuf[MAX_RECV_LENS] = {0};
uint32_t recvlens = 0;
uint8_t recvbuf[8*1024] = {0};

uint32_t pkLens = 0;
float pkFreq = 0;

DWORD WINAPI terminal_thread( void* data )
{
    HANDLE log_handle = CreateThread(NULL, 0, log_thread, NULL, 0, NULL);

    // while (process == MODE_KSERIAL)
    // {
    //     // uint32_t lens = 0;
    //     while (pocketCount)
    //     {

    //     }

    pkLens = 0;
    while (process == MODE_KSERIAL)
    {
        uint32_t idx = 0;
        uint32_t pkCount = kSerial_Read(&ks);
        while (pkCount)
        {
            int16_t bytes[512];
            kSerial_GetPacketData(ks.packet, bytes, idx);
            pkLens++;
            pkFreq = getfrequence(pkLens, bytes[0]*1000 + bytes[1], 500);
            stringlens += sprintf(&stringbuf[stringlens], "%d,%d,%d,%6d,%6d,%d\n", (int32_t)pkFreq, pkLens, pkCount, bytes[0], bytes[1], stringlens);

            // pocketFreq = getfrequence(pocketLens, bytes[0]*1000 + bytes[1], 500);
            // stringlens += sprintf(&stringbuf[stringlens], "%4.0f,%6d,%3d,%6d,%6d\n", pocketFreq, pocketLens, pocketCount, bytes[0], bytes[1]);
            // if log thread is free, update buffer
            if (printlens == 0)
            {
                for (uint32_t i = 0; i < stringlens; i++)
                {
                    printbuf[i] = stringbuf[i];
                }
                printlens = stringlens;
                printbuf[printlens] = 0;
                stringlens = 0;
            }
            if (++idx == pkCount)
            {
                break;
            }
        }

        switch (getKey())
        {
            case 'Q':
            case 'q':
            {
                process = MODE_NULL;
                printf("\n\n  >> exit\n");
                break;
            }
            case 19:    // ctrl + S
            {
                printf("\n\n  >> save file\n");
                break;
            }
        }
    }

    CloseHandle(log_handle);
    return 0;
}

#endif


// #if ENABLE_PRINT_THREAD

// volatile uint32_t printlens = 0;
// volatile uint8_t printbuf[8*1024] = {0};

// DWORD WINAPI log_thread( void* data )
// {
//     while (process != MODE_NULL)
//     {
//         if (printlens != 0)
//         {
//             printf("%s", printbuf);
//             printlens = 0;
//         }
//     }
//     return 0;
// }

// #define MAX_RECV_LENS 1024
// uint32_t lens;
// uint8_t rbuf[MAX_RECV_LENS] = {0};
// uint32_t recvlens = 0;
// uint8_t recvbuf[8*1024] = {0};

// DWORD WINAPI terminal_thread( void* data )
// {
//     HANDLE log_handle = CreateThread(NULL, 0, log_thread, NULL, 0, NULL);

//     while (process != MODE_NULL)
//     {
//         lens = Serial_RecvData(&s, rbuf, MAX_RECV_LENS);
//         if (lens != 0)
//         {
//             // move to receive buffer
//             for (uint32_t i = 0; i < lens; i++)
//             {
//                 recvbuf[recvlens + i] = rbuf[i];
//             }
//             recvlens += lens;
//             // if log thread is free, update buffer
//             if (printlens == 0)
//             {
//                 for (uint32_t i = 0; i < recvlens; i++)
//                 {
//                     printbuf[i] = recvbuf[i];
//                 }
//                 printlens = recvlens;
//                 printbuf[printlens] = 0;
//                 recvlens = 0;
//             }
//         }

//         switch (getKey())
//         {
//             case 'Q':
//             case 'q':
//             {
//                 process = MODE_NULL;
//                 printf("\n\n  >> exit\n");
//                 break;
//             }
//             case 19:    // ctrl + S
//             {
//                 printf("\n\n  >> save file\n");
//                 break;
//             }
//         }
//     }

//     CloseHandle(log_handle);
//     return 0;
// }

// #endif
