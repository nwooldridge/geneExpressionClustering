CC=g++ -g -std=c++0x
TARGET=source
OBJS=source.o parser.o clusterAlgorithm.o generateBMP.o Iteration.o geneticAlgorithm.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

source.o: 
	$(CC) -c source.cpp

parser.o:
	$(CC) -c parser.cpp

clusterAlgorithm.o:
	$(CC) -c clusterAlgorithm.cpp

generateBMP.o:
	$(CC) -c generateBMP.cpp

Iteration.o:
	$(CC) -c Iteration.cpp

geneticAlgorithm.o:
	$(CC) -c geneticAlgorithm.cpp

clean:
	rm -f *.o $(TARGET)
