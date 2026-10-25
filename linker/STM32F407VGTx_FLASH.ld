ENTRY(Reset_Handler)

_Min_Heap_Size  = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
    CCMRAM (rw)  : ORIGIN = 0x10000000, LENGTH = 64K   /* DMA erişemez */
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .isr_vector : { . = ALIGN(4); KEEP(*(.isr_vector)) . = ALIGN(4); } >FLASH

    .text : {
        . = ALIGN(4);
        *(.text*)
        KEEP(*(.init)) KEEP(*(.fini))
        . = ALIGN(4);
    } >FLASH

    .rodata : { . = ALIGN(4); *(.rodata*) . = ALIGN(4); } >FLASH

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM       : { __exidx_start = .; *(.ARM.exidx*) __exidx_end = .; } >FLASH

    .init_array : {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        __init_array_end = .;
    } >FLASH

    _sidata = LOADADDR(.data);

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*) *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /* newlib sbrk için "end" sembolü + heap/stack için yer ayırma kontrolü */
    ._user_heap_stack : {
        . = ALIGN(8);
        PROVIDE(end = .);
        PROVIDE(_end = .);
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM

    .ARM.attributes 0 : { *(.ARM.attributes) }
}