#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int adr;

byte mem[64 * 1024];

byte b_read  (adr a) {
	byte b = mem[a];
	return b;
}
void b_write (adr a, byte val) {
	mem[a] = val;
}
word w_read  (adr a) {
	assert(a % 2 == 0);
	word w = b_read(a) | b_read(a + 1) << 8;
	return w;
}
void w_write (adr a, word val) {
	assert(a % 2 == 0);
	b_write(a, (byte) val);
	b_write(a + 1, (byte) (val >> 8));
}

void load_file () {
	adr x;
	byte w;
	unsigned int n, i;
	while(scanf("%x %x ", &x, &n) == 2) {
		for(i = 0; i < n; i++) {
			scanf("%hhx", &w);
			b_write (x + i, w);
		}
		
	}
}

void mem_dump (adr start, word n) {
	word i;
	for(i = 0; i < n; i = i + 2) {
		printf("%.6o : %.6o\n", start + i, w_read(start + i));
	}
}

int main(int argc, char const *argv[])
{
	load_file();
	//mem_dump(0x40, 4);
	return 0;
}