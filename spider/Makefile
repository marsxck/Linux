CC=gcc #指定编译器
CXX=g++#记录c++编译器
INCLUDE=./include#头文件路径
CFLAGS=-I$(INCLUDE) -O2 -g -Wall -c#记录C编译选项 
LDFLAGS=-L./lib#库路径
LIBS=-lphread#库名
RM=-rm -rf#清理
SRC=$(wildcard ./src/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
TARGET=app
$(TARGET):$(OBJ)
	$(CC) $(OBJ) -I$(INCLUDE) -o $@
%.o:./src/.c
	$(CC) $(CFLAGS) $^
#伪目标
.PHONY:clean
clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET)
distclean:
	sudo rm /use/bin/$(TARGET)
install:
	sudo cp $(TARGET) /usr/bin
out:
	@echo $(SRC)
	@echo $(OBJ)
push:
	./gitpush.sh
