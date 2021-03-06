#include <lib/gdi/lcd.h>
#include <lib/gdi/epng.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <lib/gdi/esize.h>
#include <lib/base/init.h>
#include <lib/base/init_num.h>
#ifdef HAVE_TEXTLCD
	#include <lib/base/estring.h>
#endif
#include <lib/gdi/glcddc.h>

eLCD *eLCD::instance;

eLCD::eLCD()
{
	_buffer = NULL;
	lcdfd = -1;
	locked = 0;
	instance = this;
}

eLCD *eLCD::getInstance()
{
	return instance;
}

void eLCD::setSize(int xres, int yres, int bpp)
{
	res = eSize(xres, yres);
	_buffer = new unsigned char[xres * yres * bpp/8];
	memset(_buffer, 0, res.height() * res.width() * bpp / 8);
	_stride = res.width() * bpp / 8;
	eDebug("[eLCD] (%dx%dx%d) buffer %p %d bytes, stride %d", xres, yres, bpp, _buffer, xres * yres * bpp / 8, _stride);
}

eLCD::~eLCD()
{
	if (_buffer)
		delete [] _buffer;
	instance = NULL;
}

int eLCD::lock()
{
	if (locked)
		return -1;

	locked = 1;
	return lcdfd;
}

void eLCD::unlock()
{
	locked = 0;
}

#ifdef HAVE_TEXTLCD
void eLCD::renderText(ePoint start, const char *text)
{
	if (lcdfd >= 0 && start.y() < 5)
	{
#ifdef AZBOXHD_LCD_CHARACTERS
		char *text2 = (char*)malloc (64);
		int n = 0;
		int fd = open("/proc/stb/info/model", O_RDONLY);
		char tmp[255];
		int rd = fd >= 0 ? read(fd, tmp, 255) : 0;
		if (fd >= 0)
			close(fd);
		if (!strncmp(tmp, "premium+\n", rd) || !strncmp(tmp, "ultra\n", rd))
		{
			while (*text != 0 && n<63)
			{
				if (*text == 0xffffffC3)
				{
					++text;
					switch (*text) 
					{
						case 0xffffff80: text2[n++] = 0xb7;     //?. - A
										 break;
						case 0xffffff81: text2[n++] = 0xb5;     //?~A - A
										 break;
						case 0xffffff82: text2[n++] = 0xb6;     //?. - A
										 break;
						case 0xffffff83: text2[n++] = 0xc7;     //?.  - A
										 break;
						case 0xffffff84: text2[n++] = 0x8E;     //?.
										 break;
						case 0xffffff85: text2[n++] = 0x8F;     //?.
										 break;
						case 0xffffff86: text2[n++] = 0x92;     //?.
										 break;
						case 0xffffff87: text2[n++] = 0x80;     //?.
										 break;
						case 0xffffff88: text2[n++] = 0xd4;     //?. -E
										 break;
						case 0xffffff89: text2[n++] = 0x90;     //?.
										 break;
						case 0xffffff8a: text2[n++] = 0xd2;     //?. - E
										 break;
						case 0xffffff8b: text2[n++] = 0xd3;     //?. - E
										 break;
						case 0xffffff8c: text2[n++] = 0xde;     //?. - I
										 break;
						case 0xffffff8d: text2[n++] = 0xd6;     //?~M - I
										 break;
						case 0xffffff8e: text2[n++] = 0xd7;     //?. - I
										 break;
						case 0xffffff8f: text2[n++] = 0xd8;     //?~O - I
										 break;
						case 0xffffff90: text2[n++] = 0xd0;     //?~P - D
										 break;
						case 0xffffff91: text2[n++] = 0xa5;     //?.
										 break;
						case 0xffffff92: text2[n++] = 0xe3;     //?. - O
										 break;
						case 0xffffff93: text2[n++] = 0xe0;     //?. - O
										 break;
						case 0xffffff94: text2[n++] = 0xe2;     //?. - O
										 break;
						case 0xffffff95: text2[n++] = 0xe5;     //?. - O
										 break;
						case 0xffffff96: text2[n++] = 0x99;     //?.
										 break;
						case 0xffffff97: text2[n++] = 0x78;     //x
										 break;
						case 0xffffff99: text2[n++] = 0xeb;     //?. - U
										 break;
						case 0xffffff9a: text2[n++] = 0xe9;     //?. - U
										 break;
						case 0xffffff9b: text2[n++] = 0xea;     //?.  - U
										 break;
						case 0xffffff9c: text2[n++] = 0x9a;     //?.
										 break;
						case 0xffffff9d: text2[n++] = 0xed;     //?~] - Y
										 break;
						case 0xffffffa0: text2[n++] = 0x85;     //?
										 break;
						case 0xffffffa1: text2[n++] = 0xa0;     //á
										 break;
						case 0xffffffa2: text2[n++] = 0x83;     //â
										 break;
						case 0xffffffa3: text2[n++] = 0xc6;     //ã
										 break;
						case 0xffffffa4: text2[n++] = 0x84;     //ä
										 break;
						case 0xffffffa5: text2[n++] = 0x86;     //å
										 break;
						case 0xffffffa6: text2[n++] = 0x91;     //æ
										 break;
						case 0xffffffa7: text2[n++] = 0x87;     //ç
										 break;
						case 0xffffffa8: text2[n++] = 0x8a;     //è
										 break;
						case 0xffffffa9: text2[n++] = 0x82;     //é
										 break;
						case 0xffffffaa: text2[n++] = 0x88;     //ê
										 break;
						case 0xffffffab: text2[n++] = 0x89;     //ë
										 break;
						case 0xffffffac: text2[n++] = 0x8d;     //ì
										 break;
						case 0xffffffad: text2[n++] = 0xa1;     //í
										 break;
						case 0xffffffae: text2[n++] = 0x8c;     //î
										 break;
						case 0xffffffaf: text2[n++] = 0x8b;     //ï
										 break;
						case 0xffffffb1: text2[n++] = 0xa4;     //ñ
										 break;
						case 0xffffffb2: text2[n++] = 0x95;     //ò
										 break;
						case 0xffffffb3: text2[n++] = 0xa2;     //ó
										 break;
						case 0xffffffb4: text2[n++] = 0x93;     //ô
										 break;
						case 0xffffffb5: text2[n++] = 0xe4;     //õ
										 break;
						case 0xffffffb6: text2[n++] = 0x94;     //ö
										 break;
						case 0xffffffb9: text2[n++] = 0x97;     //ù
										 break;
						case 0xffffffba: text2[n++] = 0xa3;     //ú
										 break;
						case 0xffffffbb: text2[n++] = 0x96;     //û
										 break;
						case 0xffffffbc: text2[n++] = 0x81;     //ü
										 break;
						case 0xffffffbd: text2[n++] = 0xec;     //ý - y
										 break;
						case 0xffffffbf: text2[n++] = 0x98;     //ÿ
										 break;
						default:         text2[n++] = 0xffffffC3;
										 text2[n++] = *text;
										 break;
					}
					++text;
				}
				else if (*text == 0xffffffCe)
				{
					 ++text;
					 switch (*text)
					 {
						case 0xffffff91: text2[n++] = 0x41;     //A
										 break;
						case 0xffffff92: text2[n++] = 0x42;     //B
										 break;
						case 0xffffff95: text2[n++] = 0x45;     //E
										 break;
						case 0xffffff96: text2[n++] = 0x5a;     //Z
										 break;
						case 0xffffff97: text2[n++] = 0x48;     //H
										 break;
						case 0xffffff99: text2[n++] = 0x49;     //I
										 break;
						case 0xffffff9a: text2[n++] = 0x4b;     //K
										 break;
						case 0xffffff9c: text2[n++] = 0x4d;     //M
										 break;
						case 0xffffff9d: text2[n++] = 0x4e;     //N
										 break;
						case 0xffffff9f: text2[n++] = 0x4f;     //O
										 break;
						case 0xffffffa1: text2[n++] = 0x50;     //P
										 break;
						case 0xffffffa3: text2[n++] = 0x15;     //Σ
										 break;
						case 0xffffffa4: text2[n++] = 0x54;     //T
										 break;
						case 0xffffffa5: text2[n++] = 0x59;     //Y
										 break;
						case 0xffffffa7: text2[n++] = 0x58;     //X
										 break;
						case 0xffffffa9: text2[n++] = 0xf4;     //Ω NO
										 break;
						case 0xffffffaa: text2[n++] = 0xd8;     //?~O - I
										 break;
						case 0xffffffab: text2[n++] = 0x59;     //Ÿ - Y
										 break;
						case 0xffffffb2: text2[n++] = 0xe1;     //β
										 break;
						case 0xffffffbc: text2[n++] = 0xe6;     //μ
										 break;
						case 0xffffffbd: text2[n++] = 0x76;     //v
										 break;
						case 0xffffffbf: text2[n++] = 0x6f;     //o
										 break;
						default:         text2[n++] = 0xffffffCe;
										 text2[n++] = *text;
										 break;
					}
					++text;
				}
				else if (*text == 0xffffffd0)
				{
					++text;
					switch (*text)
					{
						case 0xffffff81: text2[n++] = 0xd3;     //? - ?
							break;
						case 0xffffff84: text2[n++] = 0x45;     //? - ?
							break;
						case 0xffffff86: text2[n++] = 0x49;     //? - ?
							break;
						case 0xffffff87: text2[n++] = 0xd8;     //? - ?
							break;
						case 0xffffff90: text2[n++] = 0x41;     //? - A
							break;
						case 0xffffff91: text2[n++] = 0xb0;     //? - ?
							break;
						case 0xffffff92: text2[n++] = 0x42;     //? - ?
							break;
						case 0xffffff93: text2[n++] = 0xb1;     //? - ?
							break;
						case 0xffffff94: text2[n++] = 0xb2;     //? - ?
							break;
						case 0xffffff95: text2[n++] = 0x45;     //? - ?
							break;
						case 0xffffff96: text2[n++] = 0xb3;     //? - ?
							break;
						case 0xffffff97: text2[n++] = 0xb4;     //? - ?
							break;
						case 0xffffff98: text2[n++] = 0xb8;     //? - ?
							break;
						case 0xffffff99: text2[n++] = 0xb9;     //? - ?
							break;
						case 0xffffff9a: text2[n++] = 0x4b;     //? - ?
							break;
						case 0xffffff9b: text2[n++] = 0xba;     //? - ?
							break;
						case 0xffffff9c: text2[n++] = 0x4d;     //? - ?
							break;
						case 0xffffff9d: text2[n++] = 0x48;     //? - ?
							break;
						case 0xffffff9e: text2[n++] = 0x4f;     //? - ?
							break;
						case 0xffffff9f: text2[n++] = 0xbb;     //? - ?
							break;
						case 0xffffffa0: text2[n++] = 0x50;     //? - ?
							break;
						case 0xffffffa1: text2[n++] = 0x43;     //? - ?
							break;
						case 0xffffffa2: text2[n++] = 0x54;     //? - ?
							break;
						case 0xffffffa3: text2[n++] = 0xbc;     //? - ?
							break;
						case 0xffffffa4: text2[n++] = 0xbf;     //? - ?
							break;
						case 0xffffffa5: text2[n++] = 0x58;     //? - ?
							break;
						case 0xffffffa6: text2[n++] = 0xc0;     //? - ?
							break;
						case 0xffffffa7: text2[n++] = 0xc1;     //? - ?
							break;
						case 0xffffffa8: text2[n++] = 0xc2;     //? - ?
							break;
						case 0xffffffa9: text2[n++] = 0xc3;     //? - ?
							break;
						case 0xffffffaa: text2[n++] = 0xc4;     //? - ?
							break;
						case 0xffffffab: text2[n++] = 0xc5;     //? - ?
							break;
						case 0xffffffac: text2[n++] = 0x62;     //? - ?
							break;
						case 0xffffffad: text2[n++] = 0xc8;     //? - ?
							break;
						case 0xffffffae: text2[n++] = 0xc9;     //? - ?
							break;
						case 0xffffffaf: text2[n++] = 0xca;     //? - ?
							break;
						case 0xffffffb0: text2[n++] = 0x41;     //? - ?
							break;
						case 0xffffffb1: text2[n++] = 0xb0;     //? - ?
							break;
						case 0xffffffb2: text2[n++] = 0x42;     //? - ?
							break;
						case 0xffffffb3: text2[n++] = 0xb1;     //? - ?
							break;
						case 0xffffffb4: text2[n++] = 0xb2;     //? - ?
							break;
						case 0xffffffb5: text2[n++] = 0x45;     //? - ?
							break;
						case 0xffffffb6: text2[n++] = 0xb3;     //? - ?
							break;
						case 0xffffffb7: text2[n++] = 0xb4;     //? - ?
							break;
						case 0xffffffb8: text2[n++] = 0xb8;     //? - ?
							break;
						case 0xffffffb9: text2[n++] = 0xb9;     //? - ?
							break;
						case 0xffffffba: text2[n++] = 0x4b;     //? - ?
							break;
						case 0xffffffbb: text2[n++] = 0xba;     //? - ?
							break;
						case 0xffffffbc: text2[n++] = 0x4d;     //? - ?
							break;
						case 0xffffffbd: text2[n++] = 0x48;     //? - ?
							break;
						case 0xffffffbe: text2[n++] = 0x4f;     //? - ?
							break;
						case 0xffffffbf: text2[n++] = 0xbb;     //? - ?
							break;
						default:	text2[n++] = 0xffffffd0;
								text2[n++] = *text;
								break;
					}
					++text;
				}
				else if (*text == 0xffffffd1)
				{
					++text;
					switch (*text) 
					{
						case 0xffffff80: text2[n++] = 0x50;     //? - ?
							break;
						case 0xffffff81: text2[n++] = 0x43;     //? - ?
							break;
						case 0xffffff82: text2[n++] = 0x54;     //? - ?
							break;
						case 0xffffff83: text2[n++] = 0xbc;     //? - ?
							break;
						case 0xffffff84: text2[n++] = 0xbf;     //? - ?
							break;
						case 0xffffff85: text2[n++] = 0x58;     //? - ?
							break;
						case 0xffffff86: text2[n++] = 0xc0;     //? - ?
							break;
						case 0xffffff87: text2[n++] = 0xc1;     //? - ?
							break;
						case 0xffffff88: text2[n++] = 0xc2;     //? - ?
							break;
						case 0xffffff89: text2[n++] = 0xc3;     //? - ?
							break;
						case 0xffffff8a: text2[n++] = 0xc4;     //? - ?
							break;
						case 0xffffff8b: text2[n++] = 0xc5;     //? - ?
							break;
						case 0xffffff8c: text2[n++] = 0x62;     //? - ?
							break;
						case 0xffffff8d: text2[n++] = 0xc8;     //? - ?
							break;
						case 0xffffff8e: text2[n++] = 0xc9;     //? - ?
							break;
						case 0xffffff8f: text2[n++] = 0xca;     //? - ?
							break;
						case 0xffffff91: text2[n++] = 0xd3;     //? - ?
							break;
						case 0xffffff94: text2[n++] = 0x45;     //? - ?
							break;
						case 0xffffff96: text2[n++] = 0x49;     //? - ?
							break;
						case 0xffffff97: text2[n++] = 0xd8;     //? - ?
							break;
						default:	text2[n++] = 0xffffffd1;
								text2[n++] = *text;
								break;
					}
					++text;
				}
				else
				{
					text2[n++] = *text;
					++text;
				}
			}
			text2[n] = 0;
		}
		else
		{
			while (*text != 0 && n<63)
			{
				if (*text == 0x5c)
				{
					text2[n++] = 0x60; //
					++text;
				}
				else if (*text == 0xffffffC3)
				{
					++text;
					switch (*text) 
					{
						case 0xffffff80: text2[n++] = 0x41;     //?. - A
										 break;
						case 0xffffff81: text2[n++] = 0x41;     //?~A - A
										 break;
						case 0xffffff82: text2[n++] = 0x41;     //?. - A
										 break;
						case 0xffffff83: text2[n++] = 0x41;     //?.  - A
										 break;
						case 0xffffff84: text2[n++] = 0x8E;     //?.
										 break;
						case 0xffffff85: text2[n++] = 0x8F;     //?.
										 break;
						case 0xffffff86: text2[n++] = 0x92;     //?.
										 break;
						case 0xffffff87: text2[n++] = 0x80;     //?.
										 break;
						case 0xffffff88: text2[n++] = 0x45;     //?. -E
										 break;
						case 0xffffff89: text2[n++] = 0x90;     //?.
										 break;
						case 0xffffff8a: text2[n++] = 0x45;     //?. - E
										 break;
						case 0xffffff8b: text2[n++] = 0x45;     //?. - E
										 break;
						case 0xffffff8c: text2[n++] = 0x49;     //?. - I
										 break;
						case 0xffffff8d: text2[n++] = 0x49;     //?~M - I
										 break;
						case 0xffffff8e: text2[n++] = 0x49;     //?. - I
										 break;
						case 0xffffff8f: text2[n++] = 0x49;     //?~O - I
										 break;
						case 0xffffff90: text2[n++] = 0x44;     //?~P - D
										 break;
						case 0xffffff91: text2[n++] = 0x17;     //?.
										 break;
						case 0xffffff92: text2[n++] = 0x4f;     //?. - O
										 break;
						case 0xffffff93: text2[n++] = 0x4f;     //?. - O
										 break;
						case 0xffffff94: text2[n++] = 0x4f;     //?. - O
										 break;
						case 0xffffff95: text2[n++] = 0x4f;     //?. - O
										 break;
						case 0xffffff96: text2[n++] = 0x99;     //?.
										 break;
						case 0xffffff97: text2[n++] = 0x78;     //x
										 break;
						case 0xffffff99: text2[n++] = 0x55;     //?. - U
										 break;
						case 0xffffff9a: text2[n++] = 0x55;     //?. - U
										 break;
						case 0xffffff9b: text2[n++] = 0x55;     //?.  - U
										 break;
						case 0xffffff9c: text2[n++] = 0x9a;     //?.
										 break;
						case 0xffffff9d: text2[n++] = 0x59;     //?~] - Y
										 break;
						case 0xffffffa0: text2[n++] = 0x85;     //?
										 break;
						case 0xffffffa1: text2[n++] = 0x12;     //á
										 break;
						case 0xffffffa2: text2[n++] = 0x83;     //â
										 break;
						case 0xffffffa3: text2[n++] = 0xe1;     //ã
										 break;
						case 0xffffffa4: text2[n++] = 0x84;     //ä
										 break;
						case 0xffffffa5: text2[n++] = 0x86;     //å
										 break;
						case 0xffffffa6: text2[n++] = 0x91;     //æ
										 break;
						case 0xffffffa7: text2[n++] = 0x87;     //ç
										 break;
						case 0xffffffa8: text2[n++] = 0x8a;     //è
										 break;
						case 0xffffffa9: text2[n++] = 0x82;     //é
										 break;
						case 0xffffffaa: text2[n++] = 0x88;     //ê
										 break;
						case 0xffffffab: text2[n++] = 0x89;     //ë
										 break;
						case 0xffffffac: text2[n++] = 0x8d;     //ì
										 break;
						case 0xffffffad: text2[n++] = 0x13;     //í
										 break;
						case 0xffffffae: text2[n++] = 0x8c;     //î
										 break;
						case 0xffffffaf: text2[n++] = 0x8b;     //ï
										 break;
						case 0xffffffb1: text2[n++] = 0x16;     //ñ
										 break;
						case 0xffffffb2: text2[n++] = 0x95;     //ò
										 break;
						case 0xffffffb3: text2[n++] = 0x14;     //ó
										 break;
						case 0xffffffb4: text2[n++] = 0x93;     //ô
										 break;
						case 0xffffffb5: text2[n++] = 0xef;     //õ
										 break;
						case 0xffffffb6: text2[n++] = 0x94;     //ö
										 break;
						case 0xffffffb9: text2[n++] = 0x97;     //ù
										 break;
						case 0xffffffba: text2[n++] = 0x15;     //ú
										 break;
						case 0xffffffbb: text2[n++] = 0x96;     //û
										 break;
						case 0xffffffbc: text2[n++] = 0x81;     //ü
										 break;
						case 0xffffffbd: text2[n++] = 0x79;     //ý - y
										 break;
						case 0xffffffbf: text2[n++] = 0x98;     //ÿ
										 break;
						default:         text2[n++] = 0xffffffC3;
										 text2[n++] = *text;
										 break;
					}
					++text;
				}
				else if (*text == 0xffffffCe)
				{
					++text;
					switch (*text) 
					{
						case 0xffffff91: text2[n++] = 0x41;     //A
										 break;
						case 0xffffff92: text2[n++] = 0x42;     //B
										 break;
						case 0xffffff95: text2[n++] = 0x45;     //E
										 break;
						case 0xffffff96: text2[n++] = 0x5a;     //Z
										 break;
						case 0xffffff97: text2[n++] = 0x48;     //H
										 break;
						case 0xffffff99: text2[n++] = 0x49;     //I
										 break;
						case 0xffffff9a: text2[n++] = 0x4b;     //K
										 break;
						case 0xffffff9c: text2[n++] = 0x4d;     //M
										 break;
						case 0xffffff9d: text2[n++] = 0x4e;     //N
										 break;
						case 0xffffff9f: text2[n++] = 0x4f;     //O
										 break;
						case 0xffffffa1: text2[n++] = 0x50;     //P
										 break;
						case 0xffffffa3: text2[n++] = 0xf6;     //Σ
										 break;
						case 0xffffffa4: text2[n++] = 0x54;     //T
										 break;
						case 0xffffffa5: text2[n++] = 0x59;     //Y
										 break;
						case 0xffffffa7: text2[n++] = 0x58;     //X
										 break;
						case 0xffffffa9: text2[n++] = 0xf4;     //Ω
										 break;
						case 0xffffffaa: text2[n++] = 0x49;     //?~O - I
										 break;
						case 0xffffffab: text2[n++] = 0x59;     //Ÿ - Y
										 break;
						case 0xffffffac: text2[n++] = 0xe0;     //ά - α
										 break;
						case 0xffffffad: text2[n++] = 0xe3;     //έ - ε
										 break;
						case 0xffffffb1: text2[n++] = 0xe0;     //α
										 break;
						case 0xffffffb2: text2[n++] = 0xe2;     //β
										 break;
						case 0xffffffb5: text2[n++] = 0xe3;     //ε
										 break;
						case 0xffffffb8: text2[n++] = 0xf2;     //θ
										 break;
						case 0xffffffbc: text2[n++] = 0xe4;     //μ
										 break;
						case 0xffffffbd: text2[n++] = 0x76;     //v
										 break;
						case 0xffffffbf: text2[n++] = 0x6f;     //o
										 break;
						default:         text2[n++] = 0xffffffCe;
										 text2[n++] = *text;
										 break;
					}
					++text;
				}
				else if (*text == 0xffffffd0)
				{
					++text;
					switch (*text)
					{
						case 0xffffff81: 
							text2[n++] = 0x59;     //? - YO
							text2[n++] = 0x4f;	
							break;
						case 0xffffff84: 
							text2[n++] = 0x45;     //? - E
							break;
						case 0xffffff86: 
							text2[n++] = 0x49;     //? - I
							break;
						case 0xffffff87: 
	    					text2[n++] = 0x59;     //? - YI
							text2[n++] = 0x49;
							break;
						case 0xffffff90: 
							text2[n++] = 0x41;     //? - A
							break;
						case 0xffffff91: 
							text2[n++] = 0x42;     //? - B
							break;
						case 0xffffff92: 
							text2[n++] = 0x56;     //? - V
							break;
						case 0xffffff93: 
							text2[n++] = 0x47;     //? - G
							break;
						case 0xffffff94: 
							text2[n++] = 0x44;     //? - D
							break;
						case 0xffffff95: 
							text2[n++] = 0x45;     //? - E
							break;
						case 0xffffff96: 
							text2[n++] = 0x5a;     //? - ZH
							text2[n++] = 0x48;
							break;
						case 0xffffff97: 
							text2[n++] = 0x5a;     //? - Z
							break;
						case 0xffffff98: 
							text2[n++] = 0x49;     //? - I
							break;
						case 0xffffff99: 
							text2[n++] = 0x4a;     //? - J
							break;
						case 0xffffff9a: 
							text2[n++] = 0x4b;     //? - K
							break;
						case 0xffffff9b: 
							text2[n++] = 0x4c;     //? - L
							break;
						case 0xffffff9c: 
							text2[n++] = 0x4d;     //? - M
							break;
						case 0xffffff9d: 
							text2[n++] = 0x4e;     //? - N
							break;
						case 0xffffff9e: 
							text2[n++] = 0x4f;     //? - O
							break;
						case 0xffffff9f: 
							text2[n++] = 0x50;     //? - P
							break;
						case 0xffffffa0: 
							text2[n++] = 0x52;     //? - R
							break;
						case 0xffffffa1: 
							text2[n++] = 0x53;     //? - S
							break;
						case 0xffffffa2: 
							text2[n++] = 0x54;     //? - T
							break;
						case 0xffffffa3: 
							text2[n++] = 0x55;     //? - U
							break;
						case 0xffffffa4: 
							text2[n++] = 0x46;     //? - F
							break;
						case 0xffffffa5: 
							text2[n++] = 0x48;     //? - H
							break;
						case 0xffffffa6: 
							text2[n++] = 0x43;     //? - C
							break;
						case 0xffffffa7: 
							text2[n++] = 0x43;     //? - CH
							text2[n++] = 0x48;
							break;
						case 0xffffffa8: 
							text2[n++] = 0x53;     //? - SH
							text2[n++] = 0x48;
							break;
						case 0xffffffa9: 
							text2[n++] = 0x53;     //? - SHH
							text2[n++] = 0x48;
							text2[n++] = 0x48;
							break;
						case 0xffffffaa: 
							text2[n++] = 0x22;     //? - "
							break;
						case 0xffffffab: 
							text2[n++] = 0x59;     //? - Y
							break;
						case 0xffffffac: 
							text2[n++] = 0x27;     //? - '
							break;
						case 0xffffffad: 
							text2[n++] = 0x45;     //? - EH
							text2[n++] = 0x48;
							break;
						case 0xffffffae: 
							text2[n++] = 0x59;     //? - YU
							text2[n++] = 0x55;
							break;
						case 0xffffffaf: 
							text2[n++] = 0x59;     //? - YA
							text2[n++] = 0x41;
							break;
						case 0xffffffb0: 
							text2[n++] = 0x61;     //? - a
							break;
						case 0xffffffb1: 
							text2[n++] = 0x62;     //? - b
							break;
						case 0xffffffb2: 
							text2[n++] = 0x76;     //? - v
							break;
						case 0xffffffb3: 
							text2[n++] = 0x67;     //? - g
							break;
						case 0xffffffb4: 
							text2[n++] = 0x64;     //? - d
							break;
						case 0xffffffb5: 
							text2[n++] = 0x65;     //? - e
							break;
						case 0xffffffb6: 
							text2[n++] = 0x7a;     //? - zh
							text2[n++] = 0x68;
							break;
						case 0xffffffb7: 
							text2[n++] = 0x7a;     //? - z
							break;
						case 0xffffffb8: 
							text2[n++] = 0x69;     //? - i
							break;
						case 0xffffffb9: 
							text2[n++] = 0x6a;     //? - j
							break;
						case 0xffffffba: 
							text2[n++] = 0x6b;     //? - k
							break;
						case 0xffffffbb: 
							text2[n++] = 0x6c;     //? - l
							break;
						case 0xffffffbc: 
							text2[n++] = 0x6d;     //? - m
							break;
						case 0xffffffbd: 
							text2[n++] = 0x6e;     //? - n
							break;
						case 0xffffffbe: 
							text2[n++] = 0x6f;     //? - o
							break;
						case 0xffffffbf: 
							text2[n++] = 0x70;     //? - p
							break;
						default:
							text2[n++] = 0xffffffd0;
							text2[n++] = *text;
							break;
					}
					++text;
				}
				else if (*text == 0xffffffd1)
				{
					++text;
					switch (*text) {
						case 0xffffff80: 
							text2[n++] = 0x72;     //? - r
							break;
						case 0xffffff81:
							text2[n++] = 0x73;     //? - s
							break;
						case 0xffffff82:
							text2[n++] = 0x74;     //? - t
							break;
						case 0xffffff83:
							text2[n++] = 0x75;     //? - u
							break;
						case 0xffffff84:
							text2[n++] = 0x66;     //? - f
							break;
						case 0xffffff85:
							text2[n++] = 0x68;     //? - h
							break;
						case 0xffffff86:
							text2[n++] = 0x63;     //? - c
							break;
						case 0xffffff87:
							text2[n++] = 0x63;     //? - ch
							text2[n++] = 0x68;
							break;
						case 0xffffff88:
							text2[n++] = 0x73;     //? - sh
							text2[n++] = 0x68;
							break;
						case 0xffffff89:
							text2[n++] = 0x73;     //? - shh
							text2[n++] = 0x68;
							text2[n++] = 0x68;
							break;
						case 0xffffff8a: 
							text2[n++] = 0x22;     //? - "
							break;
						case 0xffffff8b:
							text2[n++] = 0x79;     //? - y
							break;
						case 0xffffff8c:
							text2[n++] = 0x27;     //? - '
							break;
						case 0xffffff8d:
							text2[n++] = 0x65;     //? - eh
							text2[n++] = 0x68;
							break;
						case 0xffffff8e:
							text2[n++] = 0x79;     //? - yu
							text2[n++] = 0x75;
							break;
						case 0xffffff8f:
							text2[n++] = 0x79;     //? - ya
							text2[n++] = 0x61;
							break;
						case 0xffffff91:
							text2[n++] = 0x79;     //? - yo
							text2[n++] = 0x6f;
							break;
						case 0xffffff94:
							text2[n++] = 0x65;     //? - e
							break;
						case 0xffffff96:
							text2[n++] = 0x69;     //? - i
							break;
						case 0xffffff97:
							text2[n++] = 0x79;     //? - yi
							text2[n++] = 0x69;
							break;
						default:
							text2[n++] = 0xffffffd1;
							text2[n++] = *text;
							break;
					}
					++text;
				}
				else
				{
					text2[n++] = *text;
					++text;
				}
			}
			text2[n] = 0;
		}
		std::string message = text2;
		//if (message.size() > 32)  //63)
		//	message = message.substr(0, 32); //63);
		::write(lcdfd, message.c_str(), message.size());
#else
		std::string message = text;
		message = replace_all(message, "\n", " ");
		if (::write(lcdfd, message.c_str(), message.size()) == -1)
#endif
		{
			eDebug("[eLCD] renderText %s failed (%m)", text);
		}
	}
}
#endif

eDBoxLCD::eDBoxLCD()
{
	int xres = 132, yres = 64, bpp = 8;
	flipped = false;
	dump = false;
	inverted = 0;
	lcd_type = 0;
#ifndef NO_LCD
	lcdfd = open("/dev/dbox/oled0", O_RDWR);
	if (lcdfd < 0)
	{
		if (!access("/proc/stb/lcd/oled_brightness", W_OK) ||
		    !access("/proc/stb/fp/oled_brightness", W_OK) )
			lcd_type = 2;
		lcdfd = open("/dev/dbox/lcd0", O_RDWR);
	}
	else
		lcd_type = 1;

	if (lcdfd < 0)
		eDebug("[eLCD] No oled0 or lcd0 device found!");
	else
	{

#ifndef LCD_IOCTL_ASC_MODE
#define LCDSET                  0x1000
#define LCD_IOCTL_ASC_MODE	(21|LCDSET)
#define	LCD_MODE_ASC		0
#define	LCD_MODE_BIN		1
#endif

		int i = LCD_MODE_BIN;
		ioctl(lcdfd, LCD_IOCTL_ASC_MODE, &i);
		FILE *f = fopen("/proc/stb/lcd/xres", "r");
		if (f)
		{
			int tmp;
			if (fscanf(f, "%x", &tmp) == 1)
				xres = tmp;
			fclose(f);
			f = fopen("/proc/stb/lcd/yres", "r");
			if (f)
			{
				if (fscanf(f, "%x", &tmp) == 1)
					yres = tmp;
				fclose(f);
				f = fopen("/proc/stb/lcd/bpp", "r");
				if (f)
				{
					if (fscanf(f, "%x", &tmp) == 1)
						bpp = tmp;
					fclose(f);
				}
			}
			lcd_type = 3;
		}
		eDebug("[eLCD] xres=%d, yres=%d, bpp=%d lcd_type=%d", xres, yres, bpp, lcd_type);

		instance = this;
		setSize(xres, yres, bpp);
	}
#endif
}

void eDBoxLCD::setInverted(unsigned char inv)
{
	inverted = inv;
	update();
}

void eDBoxLCD::setFlipped(bool onoff)
{
	flipped = onoff;
	update();
}

void eDBoxLCD::setDump(bool onoff)
{
 	dump = onoff;
 	dumpLCD2PNG();
}

int eDBoxLCD::setLCDContrast(int contrast)
{
#ifndef NO_LCD
	if (lcdfd < 0)
		return(0);

#ifndef LCD_IOCTL_SRV
#define LCDSET                  0x1000
#define	LCD_IOCTL_SRV			(10|LCDSET)
#endif
	eDebug("[eLCD] setLCDContrast %d", contrast);

	int fp;
	if((fp = open("/dev/dbox/fp0", O_RDWR)) < 0)
	{
		eDebug("[eLCD] can't open /dev/dbox/fp0: %m");
		return(-1);
	}

	if(ioctl(lcdfd, LCD_IOCTL_SRV, &contrast) < 0)
		eDebug("[eLCD] can't set lcd contrast: %m");
	close(fp);
#endif
	return(0);
}

int eDBoxLCD::setLCDBrightness(int brightness)
{
#ifndef NO_LCD
	if (lcdfd < 0)
		return(0);

	eDebug("[eLCD] setLCDBrightness %d", brightness);
	FILE *f = fopen("/proc/stb/lcd/oled_brightness", "w");
	if (!f)
		f = fopen("/proc/stb/fp/oled_brightness", "w");
	if (f)
	{
		if (fprintf(f, "%d", brightness) == 0)
			eDebug("[eLCD] write /proc/stb/lcd|fp/oled_brightness failed: %m");
		fclose(f);
	}
	else
	{
		int fp;
		if ((fp = open("/dev/dbox/fp0", O_RDWR)) < 0)
		{
			eDebug("[eLCD] can't open /dev/dbox/fp0: %m");
			return(-1);
		}
#ifndef FP_IOCTL_LCD_DIMM
#define FP_IOCTL_LCD_DIMM       3
#endif
		if (ioctl(fp, FP_IOCTL_LCD_DIMM, &brightness) < 0)
			eDebug("[eLCD] can't set lcd brightness: %m");
		close(fp);
	}
#endif
	return(0);
}

int eDBoxLCD::setLED(int value, int option)
{
	switch(option)
	{
		case LED_BRIGHTNESS:
			eDebug("setLEDNormalState %d", value);
			if(ioctl(lcdfd, LED_IOCTL_BRIGHTNESS_NORMAL, (unsigned char)value) < 0)
				eDebug("[LED] can't set led brightness");
			break;
		case LED_DEEPSTANDBY:
			eDebug("setLEDBlinkingTime %d", value);
			if(ioctl(lcdfd, LED_IOCTL_BRIGHTNESS_DEEPSTANDBY, (unsigned char)value) < 0)
				eDebug("[LED] can't set led deep standby");
			break;
		case LED_BLINKINGTIME:
			eDebug("setLEDBlinkingTime %d", value);
			if(ioctl(lcdfd, LED_IOCTL_BLINKING_TIME, (unsigned char)value) < 0)
				eDebug("[LED] can't set led blinking time");
			break;
	}
	return(0);
}

eDBoxLCD::~eDBoxLCD()
{
	if (lcdfd >= 0)
	{
		close(lcdfd);
		lcdfd = -1;
	}
}

void eDBoxLCD::dumpLCD2PNG(void)
{
	if (dump)
	{
		int bpp =( _stride *8)/res.width();
		int lcd_width = res.width();
		int lcd_hight = res.height();
		ePtr<gPixmap> pixmap32;
		pixmap32 = new gPixmap(eSize(lcd_width, lcd_hight), 32, gPixmap::accelAuto);
		const uint8_t *srcptr = (uint8_t*)_buffer;
		uint8_t *dstptr=(uint8_t*)pixmap32->surface->data;

		switch(bpp)
		{
			case 8:
				eDebug("[eLCD] 8 bit not supportet yet");
				break;
			case 16:
				{

					for (int y = lcd_hight; y != 0; --y)
					{
						gRGB pixel32;
						uint16_t pixel16;
						int x = lcd_width;
						gRGB *dst = (gRGB *)dstptr;
						const uint16_t *src = (const uint16_t *)srcptr;
						while (x--)
						{
#if BYTE_ORDER == LITTLE_ENDIAN
							pixel16 = bswap_16(*src++);
#else
							pixel16 = *src++;;
#endif
							pixel32.a = 0xFF;
							pixel32.r = (pixel16 << 3) & 0xF8;
							pixel32.g = (pixel16 >> 3) & 0xFC;
							pixel32.b = (pixel16 >> 8) & 0xF8;
							*dst++ = pixel32;
						}
						srcptr += _stride;
						dstptr += pixmap32->surface->stride;
					}
					savePNG("/tmp/lcd.png", pixmap32);
				}
				break;
			case 32:
				eDebug("[eLCD]  32 bit not supportet yet");
				break;
			default:
				eDebug("[eLCD] %d bit not supportet yet",bpp);
		}
	}
}

void eDBoxLCD::update()
{
#ifndef HAVE_TEXTLCD
	if (lcdfd < 0)
		return;

	if (lcd_type == 0 || lcd_type == 2)
	{
		unsigned char raw[132 * 8];
		int x, y, yy;
		for (y = 0; y < 8; y++)
		{
			for (x = 0; x < 132; x++)
			{
				int pix = 0;
				for (yy = 0; yy < 8; yy++)
					pix |= (_buffer[(y * 8 + yy) * 132 + x] >= 108) << yy;
				if (flipped)
				{
					/* 8 pixels per byte, swap bits */
#define BIT_SWAP(a) (( ((a << 7)&0x80) + ((a << 5)&0x40) + ((a << 3)&0x20) + ((a << 1)&0x10) + ((a >> 1)&0x08) + ((a >> 3)&0x04) + ((a >> 5)&0x02) + ((a >> 7)&0x01) )&0xff)
					raw[(7 - y) * 132 + (131 - x)] = BIT_SWAP(pix ^ inverted);
				}
				else
					raw[y * 132 + x] = pix ^ inverted;
			}
		}
		write(lcdfd, raw, 132 * 8);
	}
	else if (lcd_type == 3)
	{
		/* for now, only support flipping / inverting for 8bpp displays */
		if ((flipped || inverted) && _stride == res.width())
		{
			unsigned int height = res.height();
			unsigned int width = res.width();
			unsigned char raw[_stride * height];
			for (unsigned int y = 0; y < height; y++)
			{
				for (unsigned int x = 0; x < width; x++)
				{
					if (flipped)
						/* 8bpp, no bit swapping */
						raw[(height - 1 - y) * width + (width - 1 - x)] = _buffer[y * width + x] ^ inverted;
					else
						raw[y * width + x] = _buffer[y * width + x] ^ inverted;
				}
			}
			write(lcdfd, raw, _stride * height);
		}
		else
#if !defined(DREAMBOX_MOVE_LCD)
			write(lcdfd, _buffer, _stride * res.height());
#else
		{
			unsigned char gb_buffer[_stride * res.height()];
			for (int offset = 0; offset < ((_stride * res.height())>>2); offset ++)
			{
				unsigned int src = 0;
				if (offset%(_stride>>2) >= 4) // 4 is offset for dm9x0
					src = ((unsigned int*)_buffer)[offset - 4];  
			//                                             blue                         red                  green low                     green high
			((unsigned int*)gb_buffer)[offset] = ((src >> 3) & 0x001F001F) | ((src << 3) & 0xF800F800) | ((src >> 8) & 0x00E000E0) | ((src << 8) & 0x07000700);
			}
			write(lcdfd, gb_buffer, _stride * res.height());
		}
#endif
	}
	else /* lcd_type == 1 */
	{
		unsigned char raw[64 * 64];
		int x, y;
		memset(raw, 0, 64 * 64);
		for (y = 0; y < 64; y++)
		{
			int pix = 0;
			for (x = 0; x < 128 / 2; x++)
			{
				pix = (_buffer[y * 132 + x * 2 + 2] & 0xF0) | (_buffer[y * 132 + x * 2 + 1 + 2] >> 4);
				if (inverted)
					pix = 0xFF - pix;
				if (flipped)
				{
					/* device seems to be 4bpp, swap nibbles */
					unsigned char byte;
					byte = (pix >> 4) & 0x0f;
					byte |= (pix << 4) & 0xf0;
					raw[(63 - y) * 64 + (63 - x)] = byte;
				}
				else
					raw[y * 64 + x] = pix;
			}
		}
		write(lcdfd, raw, 64 * 64);
	}
	dumpLCD2PNG();
#endif
}

void eDBoxLCD::dumpLCD(bool png)
{
	return;
}
