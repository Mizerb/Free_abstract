all:
	mpic++ test.cpp -o test.exe

clean:
	rm -f *.o *.exe