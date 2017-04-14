SHELL_LIB := shell.a
SHELL_OBJS:= shell.o command.o

default:
	make $(SHELL_LIB)

clean:
	rm -f *.a *.o

$(SHELL_LIB):$(SHELL_OBJS)
	ar rcs $@ $^
