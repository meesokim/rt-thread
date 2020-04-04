/*
 * File      : mmu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-10     bernard      porting to AM1808
 */

#include <rtthread.h>
#include <rthw.h>
#include <board.h>
 
#include "cp15.h"
 
#define DESC_SEC       (0x2)
#define CB             (3<<2)  //cache_on, write_back
#define CNB            (2<<2)  //cache_on, write_through
#define NCB            (1<<2)  //cache_off,WR_BUF on
#define NCNB           (0<<2)  //cache_off,WR_BUF off
#define AP_RW          (3<<10) //supervisor=RW, user=RW
#define AP_RO          (2<<10) //supervisor=RW, user=RO
#define XN             (1<<4)  // eXecute Never
 
#define DOMAIN_FAULT   (0x0)
#define DOMAIN_CHK     (0x1)
#define DOMAIN_NOTCHK  (0x3)
#define DOMAIN0        (0x0<<5)
#define DOMAIN1        (0x1<<5)
 
#define DOMAIN0_ATTR   (DOMAIN_CHK<<0)
#define DOMAIN1_ATTR   (DOMAIN_FAULT<<2)
 
/* Read/Write, cache, write back */
#define RW_CB          (AP_RW|DOMAIN0|CB|DESC_SEC)
/* Read/Write, cache, write through */
#define RW_CNB         (AP_RW|DOMAIN0|CNB|DESC_SEC)
/* Read/Write without cache and write buffer */
#define RW_NCNB        (AP_RW|DOMAIN0|NCNB|DESC_SEC)
/* Read/Write without cache and write buffer, no execute */
#define RW_NCNBXN      (AP_RW|DOMAIN0|NCNB|DESC_SEC|XN)
/* Read/Write without cache and write buffer */
#define RW_FAULT       (AP_RW|DOMAIN1|NCNB|DESC_SEC)
 
#define MMUTABLEBASE 0x004000
 
void rt_hw_mmu_setmtt(rt_uint32_t vaddrStart,
                      rt_uint32_t vaddrEnd,
                      rt_uint32_t paddrStart,
                      rt_uint32_t attr)
{
    volatile rt_uint32_t *pTT;
    volatile int i, nSec;
    pTT  = (rt_uint32_t *)MMUTABLEBASE + (vaddrStart >> 20);
    nSec = (vaddrEnd >> 20) - (vaddrStart >> 20);
    for(i = 0; i <= nSec; i++)
    {
        *pTT = attr | (((paddrStart >> 20) + i) << 20);
        pTT++;
    }
}
extern void start_mmu(unsigned int,unsigned int);
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
 
 
unsigned int mmu_section ( unsigned int vadd, unsigned int padd, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
 
    ra=vadd>>20;
    rb= MMUTABLEBASE |(ra<<2);
    rc=(padd&0xFFF00000)|0xC00|flags|2;
    
    PUT32(rb,rc);
    return(0);
}
 
void rt_hw_mmu_init(void)
{
    rt_hw_cpu_dcache_disable();
    rt_hw_cpu_icache_disable();
    rt_cpu_mmu_disable();
    rt_hw_mmu_setmtt(0,0xffffffff,0,RW_NCNB); 
    rt_cpu_tlb_set(MMUTABLEBASE);  
    
    rt_hw_cpu_dcache_enable();
    rt_hw_cpu_icache_enable();
    rt_cpu_mmu_enable();
  
    
}
