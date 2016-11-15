ENTRY(_start)

__DYNAMIC = 0;

MEMORY {
	rom : ORIGIN = 0x00000000, LENGTH = 0x00080000
	sram : ORIGIN = 0x10000000, LENGTH = 0x00008000
	spiflash : ORIGIN = 0x20000000, LENGTH = 0x00180000
	main_ram : ORIGIN = 0x40000000, LENGTH = 0x02000000
}

SECTIONS
{
	.text :
	{
		_ftext = .;
		*(.text .stub .text.* .gnu.linkonce.t.*)
		_etext = .;
	} > main_ram

	.rodata :
	{
		. = ALIGN(4);
		_frodata = .;
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		*(.rodata1)
		_erodata = .;
	} > main_ram

	.data :
	{
		. = ALIGN(4);
		_fdata = .;
		*(.data .data.* .gnu.linkonce.d.*)
		*(.data1)
		_gp = ALIGN(16);
		*(.sdata .sdata.* .gnu.linkonce.s.*)
		_edata = .;
	} > main_ram

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
