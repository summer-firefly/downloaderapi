CFLAGS = -Wall -g
CPPFLAGS = -Wall -g
#LDFLAGS += -lsystemd -lamldbus -lpthread
CFLAGS += -I$(CURDIR)
CPPFLAGS+=-I./ -fPIC -g

CSRCS := $(wildcard *.c)
CPPSRCS := $(wildcard *.cpp)

COBJS := $(CSRCS:.c=.o)
CPPOBJS := $(CPPSRCS:.cpp=.o)

OBJS := $(COBJS) $(CPPOBJS)

TARGET = sah_downloaderapi_tester

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)