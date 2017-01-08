OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0;
	.head : AT(0)
	{
		SHORT(0xAA55);
		BYTE(0);
	}
	.text : AT(3)
	{
		_text = .;
		*(.text*);
	}
	.data :
	{
		_rodata = .;
		*(.rodata*);
		_data = .;
		*(.data*);
		_bss = .;
		*(.bss*);
		_end = .;
	}
	.csum ALIGN(0x400)-1 :
	{
		BYTE(0);
	}
}
