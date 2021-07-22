OUTPUT_FORMAT(pei-i386)
SECTIONS
{
	.text_start 0x7e00:
	{
		.text.start = .;
		*(.text-key_mouse)
		*(.text-os_start)
		. = .text.start + 0x00278200;
	}
	. = ALIGN(4);
	
	.text :
	{
		*(.text)
	}
	. = ALIGN(4);
	
	.data :
	{
		*(.data)
	}
	. = ALIGN(4);
	
	.reloc :
	{
		*(.reloc);
	}
	. = ALIGN(4);
	
	.rdata :
	{
		*(.rdata)
		*(.rdata$*)
	}
	. = ALIGN(4);
	
	.buildid :
	{
		*(.buildid)
	}
	. = ALIGN(4);
	
	.eh_frame :
	{
		*(.eh_frame*)
	}
	. = ALIGN(4);
	
	.bss :
	{
		*(.bss)
	}
	. = ALIGN(4);
}