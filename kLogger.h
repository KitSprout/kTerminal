/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kLogger.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KLOGGER_H
#define __KLOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_SEGGER_RTT                   (0U)

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kStatus.h"
#if ENABLE_SEGGER_RTT
#include "SEGGER_RTT.h"
#endif

/* Define ----------------------------------------------------------------------------------*/

#define KLOG_BUFFER_SIZE                (1024 * 8)

#define KLOG_RTT_PRINTF_SUPPORT         (0U)
#define KLOG_RTT_BUFFER_INDEX           (0U)
#define KLOG_RTT_JSCOPE_BUFFER_INDEX    (1U)

/* Macro -----------------------------------------------------------------------------------*/

#if ENABLE_SEGGER_RTT
#define klog_getkey()                   SEGGER_RTT_GetKey()
#define klog_waitkey()                  SEGGER_RTT_WaitKey()
#define klog_haskey()                   SEGGER_RTT_HasKey()
#define klog_read(__DATA, __LENS)       SEGGER_RTT_Read(KLOG_RTT_BUFFER_INDEX, __DATA, __LENS)
#define klog_write(__DATA, __LENS)      SEGGER_RTT_Write(KLOG_RTT_BUFFER_INDEX, __DATA, __LENS)
#define klog_setTerminal(__IDX)         SEGGER_RTT_SetTerminal(__IDX)
#endif

/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        kputs( const char *s, uint32_t lens );
int         klogd( const char *fmt, ... );
int         klogc( const char *fmt, ... );

#if ENABLE_SEGGER_RTT
int         klog_rtt_init( const int mode );
int         klog_rtt_scope_init( uint8_t *buf, const uint32_t lens, const char *format );
int         klog_rtt_scope_send( const void *buf, const uint32_t lens );
#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
