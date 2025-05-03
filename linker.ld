MEMORY {
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  ROM (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
}

SECTIONS {
  .text : {
    *(.text*)
  } > ROM
  
  .data : {
    *(.data*)
  } > RAM
  
  .bss : {
    *(.bss*)
  } > RAM
}