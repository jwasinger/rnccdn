#CC		= cc
CC		= c++
EXECUTABLE	= gf-bench
MAIN		= gf-bench.c
INTERFACES	= 
SRCS		= $(MAIN) $(INTERFACES)
OBJS		= $(SRCS:.c=.o)
LIBS		= -lgf
LIBPATH		= -L./gf/lib
INCPATH		= -I./gf/include
CFLAGS		= -Wall -O2 $(INCPATH) -g

##################################################################

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBPATH) $(LIBS)

all : $(EXECUTABLE)

clean:
	rm *.o *.core $(EXECUTABLE)

depend:
	mkdep $(CFLAGS) $(SRCS)

