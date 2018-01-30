#include <stdio.h>
#include "font.h"

Font::Font(const char *fname){
	num = 0;
	data = std::vector<char>();
	LoadFile(fname);
}

Font::~Font(){
	
}

bool Font::LoadFile(const char *fname){
	FILE *fp;
	fp = fopen(fname, "rb");
	if(fp == NULL)
		throw "can't open font file!";
	
	fscanf(fp, "%d", &xsize);
	fscanf(fp, "%d", &ysize);

	printf("font size: %dx%d\n", xsize, ysize);

	size_t size = (xsize * ysize)/8;

	char *tmp;
	tmp = new char[size];
	data.reserve(size*256);

	do {
		if(fread(tmp, size, 1, fp) < 1) break;
		if(data.size() < size*num) data.reserve(size*num);
		for(size_t i=0;i<(size);i++){
			data[num*(size) + i] = tmp[i];
		}
		num++;
	} while(!feof(fp));

	fclose(fp);
	return true;
}
