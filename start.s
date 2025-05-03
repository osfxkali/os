.section .multiboot_header
.align 8
header_start:
    .long 0xE85250D6                  /* magic */
    .long 0                           /* architecture */
    .long header_end - header_start   /* header length */
    .long -(0xE85250D6 + 0 + (header_end - header_start)) /* checksum */

    /* End tag */
    .word 0    /* type */
    .word 0    /* flags */
    .long 8    /* size */
header_end: