// originally from https://github.com/HariboteOS/tolsrc
// [ADD] header

#include <stdio.h>

int main(int argc, char **argv){
	FILE *fp0, *fp1;
	if(argc < 3){
		puts("usage> makefont source.txt font.bin");
		return 1;
	}
	
	fp0 = fopen(argv[1], "r");
	fp1 = fopen(argv[2], "wb");

	if(fp0 == NULL){
		puts("can't open input file.");
		return 2;
	}

	if(fp1 == NULL){
		puts("can't open output file.");
		return 3;
	}

	// load header
	unsigned int xsize,ysize;
	fscanf(fp0, "%d", &xsize);
	fscanf(fp0, "%d", &ysize);

	printf("font size: %dx%d\n", xsize, ysize);
	fprintf(fp1, "%d\n%d\n", xsize, ysize);

	do {
		char s[12];
		int i;
		if(fgets(s, 12, fp0) != NULL && (s[0] == ' ' || s[0] == '*' || s[0] == '.')){
			i  = (s[0] == '*') << 7;
			i |= (s[1] == '*') << 6;
			i |= (s[2] == '*') << 5;
			i |= (s[3] == '*') << 4;
			i |= (s[4] == '*') << 3;
			i |= (s[5] == '*') << 2;
			i |= (s[6] == '*') << 1;
			i |= (s[7] == '*')     ;
			fputc(i, fp1);
		}
	} while (!feof(fp0));

	fclose(fp0);
	fclose(fp1);

	return 0;
}
