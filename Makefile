all: fifo 

OBJS = list.o main.o 
OBJS2 = procon.o
OBJS3 = banker.o
LIBS= -lpthread
CC = clang 
DEBUGFLAGS = -g3 -gdwarf-2

banker: ${OBJS3}
	${CC} -o $@ ${DEBUGFLAGS} ${LIBS} ${OBJS3}

fifo: ${OBJS}
	${CC} -o $@ ${DEBUGFLAGS} ${LIBS} ${OBJS}

clean:
	rm -rf *o fifo
