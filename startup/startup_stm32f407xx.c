#include <stdint.h>

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);

void Reset_Handler(void);
void Default_Handler(void) { for (;;) { } }

#define WEAK_HANDLER(name) void name(void) __attribute__((weak, alias("Default_Handler")))
WEAK_HANDLER(NMI_Handler);
WEAK_HANDLER(HardFault_Handler);
WEAK_HANDLER(MemManage_Handler);
WEAK_HANDLER(BusFault_Handler);
WEAK_HANDLER(UsageFault_Handler);
WEAK_HANDLER(SVC_Handler);
WEAK_HANDLER(DebugMon_Handler);
WEAK_HANDLER(PendSV_Handler);
WEAK_HANDLER(SysTick_Handler);

__attribute__((section(".isr_vector"), used))
void (* const g_vectors[16])(void) = {
    (void (*)(void))&_estack,
    Reset_Handler, NMI_Handler, HardFault_Handler,
    MemManage_Handler, BusFault_Handler, UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler, DebugMon_Handler, 0,
    PendSV_Handler, SysTick_Handler,
};

void Reset_Handler(void)
{
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
    /* FPU'yu aç (CPACR CP10/CP11); açmazsan ilk float komutunda UsageFault */
    *(volatile uint32_t *)0xE000ED88U |= (0xFU << 20);
    __asm volatile ("dsb\n isb");
#endif

    uint32_t *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) { *dst++ = *src++; }
    for (dst = &_sbss; dst < &_ebss; ) { *dst++ = 0; }

    main();
    for (;;) { }
}