/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kFile.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KFILE_H
#define __KFILE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

#include "kStatus.h"
#include "serial.h"

/* Define ----------------------------------------------------------------------------------*/

#define DEFAULT_FILENAME    "setting.txt"

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    char path[MAX_PATH + 1];
    char filename[64];

    int *port;
    int *baudrate;
    int *databits;
    int *stopbits;
    serial_parity_e *parity;
    serial_flowcontrol_e *flowctrl;

} kfile_setting_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
uint32_t    kFile_GetFullPath( char *path );
uint32_t    kFile_GetSetting( kfile_setting_t *setting );
uint32_t    kFile_LoadSetting( kfile_setting_t *cfg );
uint32_t    kFile_UpdateSetting( kfile_setting_t *cfg );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
