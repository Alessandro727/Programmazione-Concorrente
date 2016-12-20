all: prog

prog: buffer.o msg.o list.o poison_pill.o provider_buffer.o provider.o reader_buffer.o reader.o reader_list.o accepter_buffer.o accepter.o dispatcher.o main.o  
	gcc -o prog -g buffer.o msg.o list.o poison_pill.o provider_buffer.o provider.o reader_buffer.o reader.o reader_list.o accepter_buffer.o accepter.o dispatcher.o main.o -lpthread
CFLAGS=-g
clean:
	rm *.o
	rm prog