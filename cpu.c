#include <stdio.h>

int reg[] = {0, 0, 0, 0};

void add() {
	unsigned char x, y;
	scanf("%hhu %hhu ", &x, &y);
	reg[x - 5] = reg[x - 5] + reg[y - 5];
}

void sub() {
	unsigned char x, y;
	scanf("%hhu %hhu ", &x, &y);
	reg[x - 5] = reg[x - 5] - reg[y - 5];
}

void mov() {
	unsigned char n, x;
	scanf("%hhu %hhu ", &n, &x);
	reg[n - 5] = x;
}

int main()
{
	unsigned char x;
	scanf("%hhu ", &x);
	while(x != 0) {
		if (x == 1)
			add();
		if(x == 2)
			sub();
		if(x == 3)
			mov();
		if(x == 4)
			printf("%hhu %hhu %hhu %hhu\n", reg[0], reg[1], reg[2], reg[3]);
		scanf("%hhu ", &x);
	}
	return 0;
}