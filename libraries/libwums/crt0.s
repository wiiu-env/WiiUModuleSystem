.extern main
.extern exit
.extern __init_wut
.extern __fini_wut

.global __wums_start
__wums_start:
   stwu 1, -0x28(1)
   mflr 0
   stw 0, 0x2C(1)
   stw 31, 0x24(1)
   or 31, 1, 1
   stw 3, 0x18(31)
   stw 4, 0x1C(31)
   bl __init_wut
   lwz 4, 0x1C(31)   
   lwz 3, 0x18(31)   
   bl main
   or 9, 3, 3
   stw 9, 0x8(31)   
   bl __fini_wut
   lwz 9, 0x8(31)
   or 3, 9, 9
   addi 11, 31, 0x28
   lwz 0, 0x4(11)
   mtlr 0
   lwz 31, -0x4(11)
   or 1, 11, 11
   blr