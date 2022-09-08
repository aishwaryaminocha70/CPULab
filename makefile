all: scheduling 

scheduling: cpu.c
	gcc -o scheduling.out scheduling.c
  
clean:
	-rm *.out
