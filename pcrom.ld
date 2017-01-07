OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x7C00;
	.head : AT(0x7C00)
	{
		SHORT(0xAA55);
		BYTE(2);
	}
	.text : AT(0x7C03)
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
	.csum : AT(0x7FFF)
	{
		BYTE(0);
	}
}
