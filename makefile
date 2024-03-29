flags=-Wno-deprecated -g -ggdb
comp=g++
prog=ALSP-FC

#El problema
config=Instancias/airland7.txt
resultados=Out.txt
semilla=4
debug=0

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(config) $(resultados) $(semilla) $(debug)

$(prog): main.o
	$(comp) main.o -o $(prog) $(flags)

main.o: main.cpp includes.h globales.h
	$(comp) -c main.cpp $(flags)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*