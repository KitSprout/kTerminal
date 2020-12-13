/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kFile.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

#include "kFile.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kFile_GetFullPath( char *path )
{
    uint32_t lens = GetModuleFileName(NULL, path, MAX_PATH);
    char *ptr = strrchr(path, '\\');
    lens = ptr - path + 1;
    path[lens] = 0;
    return lens;
}

uint32_t kFile_GetSetting( kfile_setting_t *setting )
{
    uint32_t status;
    status = kFile_LoadSetting(setting);
    if (status != KS_OK)
    {
        if (cplist.num > 0)
        {
            s.port = cplist.port[0];
            status = kFile_UpdateSetting(setting);
        }
    }
    return status;
}

uint32_t kFile_LoadSetting( kfile_setting_t *cfg )
{
    FILE *fp;
    char filename[MAX_PATH+1] = {0};

    sprintf(filename, "%s%s", cfg->path, cfg->filename);
    fp = fopen(filename, "rb");
    if (fp != NULL)
    {
        char mode[4] = {0};
        fscanf(fp, "%d,%d,%c,%c,%c,%c",
            cfg->port, cfg->baudrate,
            &mode[0], &mode[1], &mode[2], &mode[3]);
        *cfg->databits = mode[0] - '0';
        *cfg->stopbits = mode[2] - '0';
        *cfg->flowctrl = mode[3] - '0';
        switch (mode[1])
        {
            case 'N':   *cfg->parity = SERIAL_PARITY_NONE;
            case 'O':   *cfg->parity = SERIAL_PARITY_ODD;
            case 'E':   *cfg->parity = SERIAL_PARITY_EVEN;
            case 'M':   *cfg->parity = SERIAL_PARITY_MARK;
            case 'S':   *cfg->parity = SERIAL_PARITY_SPACE;
            default:    *cfg->parity = SERIAL_PARITY_NONE;
        }
        fclose(fp);
        return KS_OK;
    }
    klogd("  open setting file error ... create new\n");
    printf("  path ... %s\n\n", cfg->path);
    fp = fopen(filename, "ab");
    *cfg->port     = DEFAULT_COMPORT;
    *cfg->baudrate = DEFAULT_BAUDRATE;
    *cfg->databits = DEFAULT_DATABITS;
    *cfg->stopbits = DEFAULT_STOPBITS;
    *cfg->parity   = DEFAULT_PARITY;
    *cfg->flowctrl = DEFAULT_FLOWCTRL;
    fprintf(fp, "%d,%d,%d,%c,%d,%d",
        *cfg->port, *cfg->baudrate,
        *cfg->databits, serial_parity_string_short[*cfg->parity], *cfg->stopbits, *cfg->flowctrl);
    fclose(fp);
    return KS_ERROR;
}

uint32_t kFile_UpdateSetting( kfile_setting_t *cfg )
{
    FILE *fp;
    char fullname[MAX_PATH+1] = {0};

    sprintf(fullname, "%s%s", cfg->path, cfg->filename);
    fp = fopen(fullname, "wb");
    if (fp != NULL)
    {
        fprintf(fp, "%d,%d,%d,%c,%d,%d",
            *cfg->port, *cfg->baudrate,
            *cfg->databits, serial_parity_string_short[*cfg->parity], *cfg->stopbits, *cfg->flowctrl);
        fclose(fp);
        return KS_OK;
    }
    return KS_ERROR;
}

/*************************************** END OF FILE ****************************************/
