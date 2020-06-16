projeto: main.o clientes.o produtos.o
	gcc -o projeto main.o clientes.o produtos.o

main.o: src/main.c src/clientes.h src/produtos.h
	gcc -Wall -Wextra -c src/main.c

clientes.o: src/clientes.c src/clientes.h
	gcc -Wall -Wextra -c src/clientes.c

produtos.o: src/produtos.c src/produtos.h
	gcc -Wall -Wextra -c src/produtos.c

clean:
	-rm -f *.o
