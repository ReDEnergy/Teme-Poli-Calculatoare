.PHNOY: all clean check dist

TARGET = ./test
OBJECTS =
CFLAGS = -g -Wall -Wextra

all: $(TARGET)

check: $(TARGET)
	$(TARGET)

$(TARGET): $(OBJECTS)

clean:
	$(RM) $(OBJECTS) $(TARGET) clips.in clips.out

dist: clean
	zip -r t3_checker_v1 *
