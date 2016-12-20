all: testprog

testprog: ../buffer.o ../msg.o ../list.o ../poison_pill.o ../provider_buffer.o ../provider.o ../reader_buffer.o ../reader.o ../reader_list.o ../accepter_buffer.o ../accepter.o ../dispatcher.o *.o
	gcc -o ../buffer.o ../msg.o ../list.o ../poison_pill.o ../provider_buffer.o ../provider.o ../reader_buffer.o ../reader.o ../reader_list.o ../accepter_buffer.o ../accepter.o ../dispatcher.o *.o -lcunit -lpthread
clean: 
	rm *.o
	rm ../*.o
	rm testprog