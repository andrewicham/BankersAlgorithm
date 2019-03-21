all: BankersAlgorithm

BankersAlgorithm: BankersAlgorithm.c
	gcc BankersAlgorithm.c -lpthread -o BankersAlgorithm

clean:
	rm -rf BankersAlgorithm

run: 
	./BankersAlgorithm
