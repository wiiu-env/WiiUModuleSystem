.extern main

.section .crt0, "ax", @progbits

.global __wums_start
__wums_start:
   b main