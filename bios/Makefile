COPY	:= cp
GIT	:= git

seabios:
	$(GIT) clone https://github.com/qemu/seabios.git
	make -C seabios
	$(COPY) seabios/out/bios.bin bios.bin
