all:
	mpic++ -g *.cpp -o test.exe -lpthread

clean:
	rm -f *.o *.exe
	
trenton:
	mpic++ bridge_intersection.cpp grid.cpp intersection.cpp LinkedNode.cpp main.cpp Rank.cpp -o test.exe -Wall -lpthread