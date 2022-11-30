PROJ=jsoncpp_demo

CPP=g++

CPPFLAGS=

INC_DIRS=/usr/local/include

LIB_DIRS=/usr/local/lib

LIBS=-ljsoncpp

$(PROJ): $(PROJ).cpp
	$(CPP) $(CPPFLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIB_DIRS:%=-L%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)

