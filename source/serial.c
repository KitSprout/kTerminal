/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "kLogger.h"
#include "serial.h"

/* Define ----------------------------------------------------------------------------------*/

#define SERIAL_MAX_PORT_NUM             (255U)
#define SERIAL_STRING_BUFFER_SIZE       (64U)
#define COMPORT_REG_SUBKEY              "HARDWARE\\DEVICEMAP\\SERIALCOMM\\"

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

const char serial_parity_string[][8] =
{
    "NONE", "ODD", "EVEN", "MARK", "SPACE"
};

const char serial_parity_string_short[] =
{
    'N', 'O', 'E', '_', '_'
};

const char serial_flowctrl_string[][8] =
{
    "NONE", "XONXOFF", "RTSCTS", "DTRDSR"
};

const int baudratelist[BAUDRATE_LIST_MAX_LENS] =
{
    9600,
    19200,
    38400,
    115200,
    230400,
    250000,
    // 256000,
    460800,
    // 500000,
    // 512000,
    921600,
    1000000,
};

comport_list_t cplist;
serial_t s = DEFAULT_SERIAL_CONFIG;

/* Prototypes ------------------------------------------------------------------------------*/

static int get_dcb_config( const serial_t *serial, serial_config_t *config, DCB *dcb );
static int set_dcb_config( const serial_t *serial, serial_config_t *config, DCB *dcb );

/* Functions -------------------------------------------------------------------------------*/

uint32_t openSerialPort( void )
{
    if (s.isConnected == KS_OPEN)
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
}

uint32_t scanSerialPort( void )
{
    if (Serial_GetComportList(&cplist) != KS_OK)
    {
        klogd("\n  scan comport error\n");
        return KS_ERROR;
    }
    return KS_OK;
}

void releaseSerialPortList( void )
{
    Serial_FreeComportList(&cplist);
}

void resetSerialBaudrate( int baudrate )
{
    Serial_CloseComport(&s);
    s.cfg.baudrate = baudrate;
    openSerialPort();
}

void Serial_Delay( uint32_t ms )
{
    Sleep(ms);
}

int Serial_OpenComport( serial_t *serial )
{
    serial_config_t serSetting;
    DCB dcbSettings;
    DWORD desiredAccess;
    char portName[SERIAL_STRING_BUFFER_SIZE];

    if (serial->isConnected == KS_OPEN)
    {
        return KS_ERROR;
    }
    // check max comport number
    if (serial->port >= SERIAL_MAX_PORT_NUM)
    {
        klogd("  max port number\n");
        return KS_ERROR;
    }

    // TODO: check baudrate
    // TODO: check databits
    // TODO: check stopbits
    // TODO: check rts, cts, dtr, dsr, xonoff

    // update port name
    sprintf(serial->name, "COM%d", serial->port);
    sprintf(portName, "\\\\.\\%s", serial->name);

    // open comport
    desiredAccess = GENERIC_READ | GENERIC_WRITE;
    serial->handle = CreateFileA(portName, desiredAccess, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serial->handle == INVALID_HANDLE_VALUE)
    {
        klogd("  open comport failed (%s)\n", serial->name);
        return KS_ERROR;
    }

#if 0
    // event setting
    if (!SetCommMask(serial->handle, EV_RXCHAR | EV_ERR))
    {
        klogd("  set comport event failed\n");
        CloseHandle(serial->handle);
        return KS_ERROR;
    }
#endif

#if 1
    // rx, tx buffer size setting
    if (!SetupComm(serial->handle, serial->txBufferSize, serial->txBufferSize))
    {
        klogd("  set comport buffer size failed\n");
        CloseHandle(serial->handle);
        return KS_ERROR;
    }
#endif

    // timeout setting
    serial->timeouts.ReadIntervalTimeout          = MAXDWORD;
    serial->timeouts.ReadTotalTimeoutMultiplier   = 0;
    serial->timeouts.ReadTotalTimeoutConstant     = 0;
    serial->timeouts.WriteTotalTimeoutMultiplier  = 0;
    serial->timeouts.WriteTotalTimeoutConstant    = 0;
    if (!SetCommTimeouts(serial->handle, &serial->timeouts))
    {
        klogd("  set comport timeout failed\n");
        CloseHandle(serial->handle);
        return KS_ERROR;
    }

    get_dcb_config(serial, &serSetting, &dcbSettings);
    dcbSettings.fBinary         = TRUE;
    dcbSettings.fDsrSensitivity = FALSE;
    dcbSettings.fErrorChar      = FALSE;
    dcbSettings.fNull           = FALSE;
    dcbSettings.fAbortOnError   = FALSE;
    set_dcb_config(serial, &serial->cfg, &dcbSettings);
    serial->isConnected = KS_OPEN;

    return KS_OK;
}

int Serial_CloseComport( serial_t *serial )
{
    if (serial->isConnected != KS_CLOSE)
    {
        CloseHandle(serial->handle);
        serial->isConnected = KS_CLOSE;
        return KS_OK;
    }
    return KS_ERROR;
}

void Serial_SetBaudrate( serial_t *serial, int baudrate )
{
    CloseHandle(serial->handle);
    serial->cfg.baudrate = baudrate;
}

int Serial_GetComportList( comport_list_t *list )
{
    HKEY hKey;
    LONG retVal;
    DWORD type;
    DWORD index = 0;
    TCHAR info[SERIAL_STRING_BUFFER_SIZE];
    TCHAR name[SERIAL_STRING_BUFFER_SIZE];

    /* http://winapi.freetechsecrets.com/win32/WIN32RegOpenKeyEx.htm */
    retVal = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE, // handle of open key
        COMPORT_REG_SUBKEY, // address of name of subkey to open
        0,                  // reserved
        KEY_QUERY_VALUE,    // security access mask
        &hKey               // address of handle of open key
    );
    if (retVal != ERROR_SUCCESS)
    {
        klogd("  open registry key failed\n");
        return KS_ERROR;
    }

    /* http://winapi.freetechsecrets.com/win32/WIN32RegQueryInfoKey.htm */
    retVal = RegQueryInfoKey(
        hKey,   // handle of key to query
        NULL,   // address of buffer for class string
        NULL,   // address of size of class string buffer
        NULL,   // reserved
        NULL,   // address of buffer for number of subkeys
        NULL,   // address of buffer for longest subkey name length
        NULL,   // address of buffer for longest class string length
        NULL,   // address of buffer for number of value entries
        NULL,   // address of buffer for longest value name length
        NULL,   // address of buffer for longest value data length
        NULL,   // address of buffer for security descriptor length
        NULL    // address of buffer for last write time
    );
    if (retVal != ERROR_SUCCESS)
    {
        klogd("  get registry key information failed\n");
        RegCloseKey(hKey);
        return KS_ERROR;
    }

    Serial_FreeComportList(list);

    while (1)
    {
        DWORD infoBufSize = SERIAL_STRING_BUFFER_SIZE;
        DWORD nameBufSize = SERIAL_STRING_BUFFER_SIZE;

        /* http://winapi.freetechsecrets.com/win32/WIN32RegEnumValue.htm */
        retVal = RegEnumValue(
            hKey,           // handle of key to query
            index,          // index of value to query
            info,           // address of buffer for value string
            &infoBufSize,   // address for size of value buffer
            NULL,           // reserved
            &type,          // address of buffer for type code
            (LPBYTE)name,   // address of buffer for value data
            &nameBufSize    // address for size of data buffer
        );
        if (retVal != ERROR_SUCCESS)
        {
            break;
        }

        if (type == REG_SZ)
        {
            name[nameBufSize] = '\0';
            info[infoBufSize] = '\0';
            list->info[list->num] = malloc(sizeof(char) * infoBufSize + 1);
            memcpy(list->info[list->num], info, infoBufSize + 1);
            sscanf(name, "COM%d", &list->port[list->num]);
            list->num++;
        }
        index++;
    }

    RegCloseKey(hKey);

    return KS_OK;
}

void Serial_FreeComportList( comport_list_t *list )
{
    for (uint32_t i = 0; i < list->num; i++)
    {
        free(list->info[i]);
    }
    list->num = 0;
}

int Serial_SendByte( const serial_t *serial, const uint8_t sdata )
{
    DWORD n;
    WriteFile(serial->handle, &sdata, 1, &n, NULL);
    if (n < 0)
    {
        return KS_ERROR;
    }
    return KS_OK;
}

uint8_t Serial_RecvByte( const serial_t *serial )
{
    DWORD n;
    uint8_t rdata;
    ReadFile(serial->handle, &rdata, 1, &n, NULL);
    return rdata;
}

int Serial_SendData( const serial_t *serial, const uint8_t *sdata, const uint32_t lens )
{
    DWORD n;
    if (WriteFile(serial->handle, sdata, lens, &n, NULL))
    {
        return (n);
    }
    return (-1);
}

int Serial_RecvData( const serial_t *serial, uint8_t *rdata, const uint32_t lens )
{
    DWORD n;
    ReadFile(serial->handle, rdata, lens, &n, NULL);
    return (n);
}

void Serial_Flush( const serial_t *serial )
{
    PurgeComm(serial->handle, PURGE_RXCLEAR | PURGE_RXABORT);
    PurgeComm(serial->handle, PURGE_TXCLEAR | PURGE_TXABORT);
}

void Serial_FlushRX( const serial_t *serial )
{
    PurgeComm(serial->handle, PURGE_RXCLEAR | PURGE_RXABORT);
}

void Serial_FlushTX( const serial_t *serial )
{
    PurgeComm(serial->handle, PURGE_TXCLEAR | PURGE_TXABORT);
}

static int get_dcb_config( const serial_t *serial, serial_config_t *config, DCB *dcb )
{
    if (!GetCommState(serial->handle, dcb))
    {
        klogd("  get comport dcb setting failed\n");
        return KS_ERROR;
    }

    config->baudrate = dcb->BaudRate;
    config->databits = dcb->ByteSize;

    if (dcb->fParity)
    {
        switch (dcb->Parity)
        {
            case NOPARITY:    config->parity = SERIAL_PARITY_NONE;    break;
            case ODDPARITY:   config->parity = SERIAL_PARITY_ODD;     break;
            case EVENPARITY:  config->parity = SERIAL_PARITY_EVEN;    break;
            case MARKPARITY:  config->parity = SERIAL_PARITY_MARK;    break;
            case SPACEPARITY: config->parity = SERIAL_PARITY_SPACE;   break;
            // default:          config->parity = SERIAL_PARITY_INVALID; break;
            default:          config->parity = SERIAL_PARITY_NONE; break;
        }
    }
    else
    {
        config->parity = SERIAL_PARITY_NONE;
    }

    switch (dcb->StopBits)
    {
        case ONESTOPBIT:  config->stopbits =  1;  break;
        case TWOSTOPBITS: config->stopbits =  2;  break;
        default:          config->stopbits = -1;  break;
    }

    switch (dcb->fRtsControl)
    {
        case RTS_CONTROL_DISABLE:   config->rts = SERIAL_RTS_OFF;           break;
        case RTS_CONTROL_ENABLE:    config->rts = SERIAL_RTS_ON;            break;
        case RTS_CONTROL_HANDSHAKE: config->rts = SERIAL_RTS_FLOW_CONTROL;  break;
        default:                    config->rts = SERIAL_RTS_INVALID;       break;
    }

    config->cts = (dcb->fOutxCtsFlow) ? SERIAL_CTS_FLOW_CONTROL : SERIAL_CTS_IGNORE;

    switch (dcb->fDtrControl)
    {
        case DTR_CONTROL_DISABLE:   config->dtr = SERIAL_DTR_OFF;           break;
        case DTR_CONTROL_ENABLE:    config->dtr = SERIAL_DTR_ON;            break;
        case DTR_CONTROL_HANDSHAKE: config->dtr = SERIAL_DTR_FLOW_CONTROL;  break;
        default:                    config->dtr = -1;                       break;
    }

    config->dsr = (dcb->fOutxDsrFlow) ? SERIAL_DSR_FLOW_CONTROL : SERIAL_DSR_IGNORE;

    if (dcb->fInX)
    {
        config->xonoff = (dcb->fOutX) ? SERIAL_XONXOFF_INOUT : SERIAL_XONXOFF_IN;
    }
    else
    {
        config->xonoff = (dcb->fOutX) ? SERIAL_XONXOFF_OUT : SERIAL_XONXOFF_DISABLED;
    }

    config->flowctrl = SERIAL_FLOWCONTROL_NONE;

    // if (flowcontrol > SP_FLOWCONTROL_DTRDSR)
    //     RETURN_ERROR(SP_ERR_ARG, "Invalid flow control setting");
    // if (flowcontrol == SP_FLOWCONTROL_XONXOFF)
    //     config->xon_xoff = SP_XONXOFF_INOUT;
    // else
    //     config->xon_xoff = SP_XONXOFF_DISABLED;
    // if (flowcontrol == SP_FLOWCONTROL_RTSCTS) {
    //     config->rts = SP_RTS_FLOW_CONTROL;
    //     config->cts = SP_CTS_FLOW_CONTROL;
    // } else {
    // if (config->rts == SP_RTS_FLOW_CONTROL)
    //     config->rts = SP_RTS_ON;
    // config->cts = SP_CTS_IGNORE;
    // }
    // if (flowcontrol == SP_FLOWCONTROL_DTRDSR) {
    //     config->dtr = SP_DTR_FLOW_CONTROL;
    //     config->dsr = SP_DSR_FLOW_CONTROL;
    // } else {
    // if (config->dtr == SP_DTR_FLOW_CONTROL)
    //     config->dtr = SP_DTR_ON;
    //     config->dsr = SP_DSR_IGNORE;
    // }

    return KS_OK;
}

static int set_dcb_config( const serial_t *serial, serial_config_t *config, DCB *dcb )
{
    dcb->BaudRate = config->baudrate;
    dcb->ByteSize = config->databits;

    switch (config->parity)
    {
        case SERIAL_PARITY_NONE:  dcb->Parity = NOPARITY;     break;
        case SERIAL_PARITY_ODD:   dcb->Parity = ODDPARITY;    break;
        case SERIAL_PARITY_EVEN:  dcb->Parity = EVENPARITY;   break;
        case SERIAL_PARITY_MARK:  dcb->Parity = MARKPARITY;   break;
        case SERIAL_PARITY_SPACE: dcb->Parity = SPACEPARITY;  break;
        default:
        {
            klogd("  invalid parity setting\n");
            return KS_ERROR;
        }
    }

    switch (config->stopbits)
    {
        case 1:   dcb->StopBits = ONESTOPBIT;   break;
        case 2:   dcb->StopBits = TWOSTOPBITS;  break;
        default:
        {
            klogd("  invalid stopbits setting\n");
            return KS_ERROR;
        }
    }

    switch (config->rts)
    {
        case SERIAL_RTS_OFF:          dcb->fRtsControl = RTS_CONTROL_DISABLE;   break;
        case SERIAL_RTS_ON:           dcb->fRtsControl = RTS_CONTROL_ENABLE;    break;
        case SERIAL_RTS_FLOW_CONTROL: dcb->fRtsControl = RTS_CONTROL_HANDSHAKE; break;
        default:
        {
            klogd("  invalid rts setting\n");
            return KS_ERROR;
        }
    }

    switch (config->cts)
    {
        case SERIAL_CTS_IGNORE:       dcb->fOutxCtsFlow = FALSE;  break;
        case SERIAL_CTS_FLOW_CONTROL: dcb->fOutxCtsFlow = TRUE;   break;
        default:
        {
            klogd("  invalid cts setting\n");
            return KS_ERROR;
        }
    }

    switch (config->dtr)
    {
        case SERIAL_DTR_OFF:          dcb->fDtrControl = DTR_CONTROL_DISABLE;   break;
        case SERIAL_DTR_ON:           dcb->fDtrControl = DTR_CONTROL_ENABLE;    break;
        case SERIAL_DTR_FLOW_CONTROL: dcb->fDtrControl = DTR_CONTROL_HANDSHAKE; break;
        default:
        {
            klogd("  invalid dtr setting\n");
            return KS_ERROR;
        }
    }

    switch (config->dsr)
    {
        case SERIAL_DSR_IGNORE:       dcb->fOutxDsrFlow = FALSE;  break;
        case SERIAL_DSR_FLOW_CONTROL: dcb->fOutxDsrFlow = TRUE;   break;
        default:
        {
            klogd("  invalid dsr setting\n");
            return KS_ERROR;
        }
    }

    switch (config->xonoff)
    {
        case SERIAL_XONXOFF_DISABLED:   dcb->fInX = FALSE;  dcb->fOutX = FALSE;   break;
        case SERIAL_XONXOFF_IN:         dcb->fInX = TRUE;   dcb->fOutX = FALSE;   break;
        case SERIAL_XONXOFF_OUT:        dcb->fInX = FALSE;  dcb->fOutX = TRUE;    break;
        case SERIAL_XONXOFF_INOUT:      dcb->fInX = TRUE;   dcb->fOutX = TRUE;    break;
        default:
        {
            klogd("  invalid xon/xoff setting\n");
            return KS_ERROR;
        }
    }

    if (!SetCommState(serial->handle, dcb))
    {
        klogd("  set comport dcb setting failed\n");
        CloseHandle(serial->handle);
        return KS_ERROR;
    }

    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
