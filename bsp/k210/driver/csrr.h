#ifndef __CSRR_H
#define __CSRR_H

#define SBI_GET_HARTID 0x01

#define SBI_GET_CYCLE  0x02

#define SBI_GET_MIE 0x03
#define SBI_SET_MIE 0x13
#define SBI_CLR_MIE 0x23
#define SBI_WRI_MIE 0x33

#define SBI_GET_STATUS 0x04
#define SBI_SET_STATUS 0x14
#define SBI_CLR_STATUS 0x24

#define SBI_GET_MIP 0x05
#define SBI_SET_MIP 0x15
#define SBI_CLR_MIP 0x25

#define SBI_GET_INSTRET 0x06

#define SBI_CALL(which, arg0, arg1, arg2) ({			\
	register unsigned long long a0 asm ("a0") = (unsigned long long)(arg0);	\
	register unsigned long long a1 asm ("a1") = (unsigned long long)(arg1);	\
	register unsigned long long a2 asm ("a2") = (unsigned long long)(arg2);	\
	register unsigned long long a7 asm ("a7") = (unsigned long long)(which);	\
	asm volatile ("ecall"					\
		      : "+r" (a0)				\
		      : "r" (a1), "r" (a2), "r" (a7)		\
		      : "memory");				\
	a0;							\
})

int printk2(const char *format, ...);
unsigned long long handlecsr(unsigned long long cmd,unsigned long long arg0);
extern int usermode;

unsigned long long sbi_call(unsigned long long which,unsigned long long arg0,unsigned long long arg1,unsigned long long arg2,const char *func,int line);

/* Lazy implementations until SBI is finalized */
#define SBI_CALL_0(which) sbi_call(which,0,0,0,__func__,__LINE__)
#define SBI_CALL_1(which, arg0) sbi_call(which, arg0, 0, 0,__func__,__LINE__)
#define SBI_CALL_2(which, arg0, arg1) SBI_CALL(which, arg0, arg1, 0)

#define get_hartid() SBI_CALL_0(SBI_GET_HARTID)
#define get_cycle() SBI_CALL_0(SBI_GET_CYCLE)
#define get_mie() SBI_CALL_0(SBI_GET_MIE)
#define set_mie(x) SBI_CALL_1(SBI_SET_MIE,(x))
#define clr_mie(x) SBI_CALL_1(SBI_CLR_MIE,(x))
#define wri_mie(x) SBI_CALL_1(SBI_WRI_MIE,(x))
#define get_status() SBI_CALL_0(SBI_GET_STATUS)
#define set_status(x) SBI_CALL_1(SBI_SET_STATUS,(x))
#define clr_status(x) SBI_CALL_1(SBI_CLR_STATUS,(x))

#define get_mip() SBI_CALL_0(SBI_GET_MIP)
#define set_mip(x) SBI_CALL_1(SBI_SET_MIP,(x))
#define clr_mip(x) SBI_CALL_1(SBI_CLR_MIP,(x))

#define get_instret() SBI_CALL_0(SBI_GET_INSTRET)

#endif
