BIN = ircc.exe
# default Cygwin directory is /home/eralsaz
INSTALL_DIR = C:/Users/eralsaz/git/ircc/bin
SRCS = main.c socket.c commands.c
OBJ = main.o 

CC = gcc-4
CFLAGS = -D_BSD_SOURCE -D_POSIX_C_SOURCE -DSYSLOG -Wall -Wextra -std=c89 -pedantic -O0 #-Werror #-Wfatal-errors # -I../cdb-0.55 -I/usr/local/BerkeleyDB/include 
LDFLAGS = #-L../cdb-0.55 -lcdb -L/usr/local/BerkeleyDB/lib -ldb
LFLAGS = -g -lpthread
INSTALL_BIN=$(INSTALL_DIR)/$(BIN)
# can't use because Cygwin does not support Valgrind
VALGRIND_FLAGS = --tool=memcheck --show-reachable=yes --leak-check=full -q

all default: $(BIN)

# can't use because Cygwin does not support Valgrind
valgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN)


TAGS:
	cscope -b -q *.c *.h



$(BIN): $(SRCS)
	$(CC) $(LFLAGS) $(LDFLAGS) $(INCLUDES) -o $@ $(SRCS) $(LIBS)
	#mkdir $(INSTALL_DIR)
	cp $(BIN) $(INSTALL_DIR)
	rm $(BIN)
	
	
clean:
	rm -f *.o *~ core a.out *.exe *.stackdump
	rm -r $(INSTALL_DIR)/$(BIN)
