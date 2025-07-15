CC= g++ -c

portugol: main.o
	g++ main.o -o portugol
	rm *.o
main.o:
	$(CC) main.cpp

clean:
	rm -f *.o portugol