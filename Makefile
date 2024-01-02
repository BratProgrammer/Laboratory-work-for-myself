.PHONY: all clean

APP=lab2adtN3150_2
CFLAGS=-Wall -Wextra -Werror -g

all: $(APP)

$(APP):
	gcc -o $(APP) $(CFLAGS) $(APP).c -lm

clean:
	rm $(APP)