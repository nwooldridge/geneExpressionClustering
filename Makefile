CC=g++ -g -std=c++0x
TARGET=source
OBJS=source.o parser.o clusterAlgorithm.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

source.o: 
	$(CC) -c source.cpp

parser.o:
	$(CC) -c parser.cpp

clusterAlgorithm.o:
	$(CC) -c clusterAlgorithm.cpp


clean:
	rm -f *.o $(TARGET)
