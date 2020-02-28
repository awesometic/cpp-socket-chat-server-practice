CC = g++
SERV_TARGET = server
SERV_SRC = server.cpp
SERV_OBJS = server.o
CLI_TARGET = client
CLI_SRC = client.cpp
CLI_OBJS = client.o

ifneq ($V,1)
Q ?= @
endif

.PHONY = all clean

all: $(SERV_TARGET) $(CLI_TARGET)

$(SERV_TARGET): $(SERV_SRC)
	$(Q) $(CC) -o $@ $^

$(CLI_TARGET): $(CLI_SRC)
	$(Q) $(CC) -o $@ $^

clean:
	rm -f *.o
	rm -f $(SERV_TARGET) $(CLI_TARGET)
