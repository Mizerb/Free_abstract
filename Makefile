all:
	mpic++ *.cpp -o test.exe

clean:
	rm -f *.o *.exe