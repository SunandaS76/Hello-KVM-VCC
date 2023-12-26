#include <stddef.h>
#include <stdint.h>


static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static inline void out(uint16_t port, uint32_t value) {

  asm("out %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");

}

static inline uint32_t inb(uint16_t port) {

  uint32_t ret;

  asm("in %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );

  return ret;

}

void HC_print8bit(uint8_t val)
{
	outb(0xE9, val);
}

void HC_print32bit(uint32_t val)
{

	if(val == 0){
	outb(0xE9, 48);
		}
	long rev = 0;
	int num_zeros = 0;	
	int flag=0;
	while(val){
		int p = val;
		if(p%10 == 0 && flag == 0){		
			while(p%10 == 0){
				p = p / 10;
				num_zeros++;
			}
			flag = 1;
		}
		flag = 1;
		
		rev = val % 10 + rev * 10;
		
		val = val / 10;
	}
	while(rev != 0){
		outb(0xE9, rev%10+48);
		rev = rev / 10;
	}
	while(num_zeros){
		outb(0xE9, 48);
		num_zeros--;
	}

		outb(0xE9, 10);

}

uint32_t HC_numExits()
{
	// Fill in here
	int rett = inb(0xE9);
	return rett;	
}

void HC_printStr(char *str)
{
	// Fill in here
	out(0xEA, (uintptr_t) str);
		
}

uint32_t HC_gvaToHva(uint32_t gva)
{
	
	out(0xEC, gva);
	uint32_t hva =  inb(0xEC);
	
	return hva;	
}

uint32_t HC_getTimestamp()
{
	// Fill in here
	uint32_t ts = inb(0xED);

	return ts;	// Remove this
}

void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;

	for (p = "Hello, world!\n"; *p; ++p)
		HC_print8bit(*p);


	/*----------Don't modify this section. We will use grading script---------*/
	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(2048);
	HC_print32bit(4294967295); // copy og number

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();

	char *str = "CS 695 Assignment 3\n";
	HC_printStr(str);

	num_exits_b = HC_numExits();

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);

	uint32_t hva;
	hva = HC_gvaToHva(1024);
	HC_print32bit(hva);
	hva = HC_gvaToHva(4294967295);
	HC_print32bit(hva);

	uint32_t tsc;
	tsc = HC_getTimestamp();
	HC_print32bit(tsc);
	/*------------------------------------------------------------------------*/

	*(long *) 0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
