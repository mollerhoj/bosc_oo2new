all: procon 

OBJS = list.o main.o 
OBJS2 = procon.o
LIBS= -lpthread
CC = clang 
DEBUGFLAGS = -g3 -gdwarf-2

procon: ${OBJS2}
	${CC} -o $@ ${DEBUGFLAGS} ${LIBS} ${OBJS2}

#fifo: main.o ${OBJS}
#	${CC} -o $@ ${DEBUGFLAGS} ${LIBS} ${OBJS}

clean:
	rm -rf *o fifo
