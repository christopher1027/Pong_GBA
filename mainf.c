//Screen 240x160 pixels
//?rrrrrgggggbbbbb (16 bit color word)
//32 bit ARM processor (ARM7tdmi 16.78 MHz)
//Thumb instructions (16 bits)
//0x00000000 -> 0x00003fff (System ROM, executable not readable)
//0x02000000 -> 0x02020000 (External to the CPU RAM 256 KB)
//0x03000000 -> 0x03007fff (internal to CPU RAM 32 KB)
//0x04000000 -> 0x040003ff (I/O registers)
//0x05000000 -> 0x050003ff (Color palette)
//0x06000000 -> 0x06017fff (Video RAM 96 KB)
//0x07000000 -> 0x070003ff (OAM RAM (Object Attribute Memory))
//0x08000000 -> 0x???????? (Gamepak ROM size (up to 32 MB))
//0x0E000000 -> 0x???????? (Gamepak RAM)

int main(){
	//volatile - says do not get rid of.
	volatile unsigned char *ioram = (unsigned char *)0x04000000;
	ioram[0] = 0x03; //set video mode to 3 (which is BG2)
	ioram[1] = 0x04; //enables the above video mode (BG0 = 1, BG1 = 2, BG2 = 4...)
	
	volatile unsigned short *vram = (unsigned shor *)0x06000000;
	
	//run forever and ever
	int inc = 0;
	while(1)
	{
		int clearLine = inc - 1;
		if(clearLine < 0)
			clearLine = 159;
		vram[clearLine*240 = 110] = 0x0000;
		vram[clearLine*240 = 120] = 0x0000;
		vram[clearLine*240 = 130] = 0x0000;
		
		
		vram[inc*240 + 110] = 0x001f;
		vram[inc*240 + 120] = 0x03e0;
		vram[inc*240 + 130] = 0x7c00;
		inc++;
		if(inc == 160) 
			inc = 0;
	}
	
	return 0;
}