TARGET	:= gui.a
OBJS	:= gui.o

default:
	make $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET):$(OBJS)
	ar rcs $@ $^

