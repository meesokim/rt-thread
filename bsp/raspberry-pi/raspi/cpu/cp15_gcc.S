/*
 * File      : cp15_gcc.S
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2013, RT-Thread Development Team
 * http://www.rt-thread.org
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-07-05     Bernard      the first version
 */

.globl rt_hw_cpu_dcache_enable
rt_hw_cpu_dcache_enable:
    mrc p15,0,r2,c1,c0,0
    orr r2,r2,#0x4
    mcr p15,0,r2,c1,c0,0
    bx lr
 
.globl rt_hw_cpu_icache_enable
rt_hw_cpu_icache_enable:
    mrc p15,0,r2,c1,c0,0
    orr r2,r2,#0x1000
    mcr p15,0,r2,c1,c0,0
    bx      lr
 
.globl rt_hw_cpu_dcache_disable
rt_hw_cpu_dcache_disable:
    mrc p15,0,r2,c1,c0,0
    bic r2,#0x4
    mcr p15,0,r2,c1,c0,0
    bx lr
 
.globl rt_hw_cpu_icache_disable
rt_hw_cpu_icache_disable:
    mrc p15,0,r2,c1,c0,0
    bic r2,#0x1000
    mcr p15,0,r2,c1,c0,0
    bx lr
 
.globl rt_cpu_mmu_disable
rt_cpu_mmu_disable:
    mrc p15,0,r2,c1,c0,0
    bic r2,#0x0001
    mcr p15,0,r2,c1,c0,0
    bx lr
 
.globl rt_cpu_mmu_enable
rt_cpu_mmu_enable:
 
    mov r2,#0
    mcr p15,0,r2,c7,c7,0 ;@ invalidate caches
    mcr p15,0,r2,c8,c7,0 ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??
 
    mvn r2,#0
    bic r2,#0xC
    mcr p15,0,r2,c3,c0,0 ;@ domain
 
    mrc p15,0,r2,c1,c0,0
    orr r2,r2,#0x01
    mcr p15,0,r2,c1,c0,0
 
    bx lr
 
.globl rt_cpu_tlb_set
rt_cpu_tlb_set:
    mcr p15,0,r0,c2,c0,0 ;@ tlb base
    mcr p15,0,r0,c2,c0,1 ;@ tlb base
