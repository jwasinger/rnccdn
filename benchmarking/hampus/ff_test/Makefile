CC		= cc
#CC		= c++
EXECUTABLE	= gf-bench
MAIN		= ff_benchmark.c
INTERFACES	= ff_2_163.c ff_2_163.h
SRCS		= $(MAIN) $(INTERFACES)
OBJS		= $(SRCS:.c=.o)
#LIBS		= -lgf
#LIBPATH		= -L./gf/lib
#INCPATH		= -I./gf/include
CFLAGS		= -Wall -O2 -g

##################################################################

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE) : $(OBJS)
	$(CC) -o $@ $(OBJS) 

all : $(EXECUTABLE)

clean:
	rm *.o *.core $(EXECUTABLE)

depend:
	mkdep $(CFLAGS) $(SRCS)

