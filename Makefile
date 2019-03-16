all: BankersAlg

BankersAlg: BankersAlg.c
	gcc BankersAlg.c -lpthread -o BankersAlg

clean:
	rm -rf BankersAlg

run: 
	./BankersAlg
