ENTRY(_start)
SECTIONS
{
    . = 0x00008000;
    .text : { *(.text*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
    stack_top = . + 0x1000; // 分配1KB的栈空间
}
