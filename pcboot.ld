OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x7C00;
	.text : AT(0x7C00)
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
	.sig : AT(0x7DFE)
	{
		SHORT(0xAA55);
	}
}
