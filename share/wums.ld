SECTIONS
{
   .wums.meta : {
	  *(.wums.meta*)
      KEEP(*(.wums.meta*))
   } > datamem
   
   .wums.exports : {
	  *(.wums.exports*)
      KEEP(*(.wums.exports*))
   } > datamem
   
   .wums.hooks : {
	  *(.wums.hooks*)
      KEEP(*(.wums.hooks*))
   } > datamem   
}