/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kLogger.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
#define ENABLE_KSLOG    (0U)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static char klogdBuf[KLOG_BUFFER_SIZE];
static char klogcBuf[KLOG_BUFFER_SIZE];
static uint32_t idx;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void kputs( const char *s, uint32_t lens )
{
    while (lens--)
    {
        putchar(*s++);
    }
}

/**
 *  @brief  klogd
 */
int klogd( const char *fmt, ... )
{
    uint32_t lens = 0;
    va_list aptr;
    va_start(aptr, fmt);
    lens = vsprintf(klogdBuf, fmt, aptr);
    va_end(aptr);
#if ENABLE_SEGGER_RTT
    klog_write(klogdBuf, lens);
#else
    kputs(klogdBuf, lens);
#endif
    return lens;
}

/**
 *  @brief  klogc
 */
int klogc( const char *fmt, ... )
{
    int lens;
    if (fmt != NULL)
    {
        va_list aptr;
        va_start(aptr, fmt);
        idx += vsprintf(&klogcBuf[idx], fmt, aptr);
        va_end(aptr);
        lens = idx;
    }
    else
    {
        lens = idx;
        klogcBuf[idx] = 0;
#if ENABLE_SEGGER_RTT
        klog_write(klogcBuf, lens);
#else
        kputs(klogcBuf, lens);
#endif
        idx = 0;
    }

    return lens;
}

#if ENABLE_SEGGER_RTT
/**
 *  @brief  klog_init
 */
int klog_init( const int mode )
{
    // default mode: SEGGER_RTT_MODE_NO_BLOCK_SKIP
    return SEGGER_RTT_ConfigUpBuffer(KLOG_RTT_BUFFER_INDEX, NULL, NULL, 0, mode);
}

/**
 *  @brief  klog_scope_init
 *
 *  t4: timestamp (32bit, us)
 *  i1,i2,i4: 8,16,32bits signed
 *  u1,u2,u4: 8,16,32bits unsigned
 *
 */
int klog_scope_init( uint8_t *buf, const uint32_t lens, const char *format )
{
    return SEGGER_RTT_ConfigUpBuffer(KLOG_RTT_JSCOPE_BUFFER_INDEX, format, buf, lens, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

/**
 *  @brief  klog_scope_send
 */
int klog_scope_send( const void *buf, const uint32_t lens )
{
    return SEGGER_RTT_Write(KLOG_RTT_JSCOPE_BUFFER_INDEX, buf, lens);
}
#endif

#if KLOG_RTT_PRINTF_SUPPORT
/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
    char ch8 = ch;
    klog_write(&ch8, 1);
    return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
    return klog_waitkey();
}
#endif

/*************************************** END OF FILE ****************************************/
