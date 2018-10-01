INCLUDE generated/output_format.ld
INCLUDE generated/regions.ld
ENTRY(_start)
/*
INPUT(crti.o crtbegin.o crt0.o crtend.o crtn.o)
GROUP(-lgloss -lnosys -lc -lgcc)
*/
__DYNAMIC = 0;

SECTIONS
{
	.fbi :
	{
		. = . + 8; /* Firmware Base Image format (len/crc) data comes first. */
	} > user_flash

	.text :
	{
		_ftext = .;
		*(.text .stub .text.* .gnu.linkonce.t.*)
		_etext = .;
	} > user_flash

	.rodata :
	{
		. = ALIGN(4);
		_frodata = .;
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		*(.rodata1)
		_erodata = .;
	} > user_flash

	.data : AT (ADDR(.rodata) + SIZEOF (.rodata))
	{
		. = ALIGN(4);
		_fdata = .;
		*(.data .data.* .gnu.linkonce.d.*)
		*(.data1)
		_gp = ALIGN(16);
		*(.sdata .sdata.* .gnu.linkonce.s.*)
		_edata = ALIGN(16); /* Make sure _edata is >= _gp. */
	} > sram

	.bss :
	{
		. = ALIGN(4);
		_fbss = .;
		__bss_start = .;
		*(.dynsbss)
		*(.sbss .sbss.* .gnu.linkonce.sb.*)
		*(.scommon)
		*(.dynbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		_end = .;
		end = .;
	} > sram
}

PROVIDE(_fstack = ORIGIN(sram) + LENGTH(sram) - 4);
