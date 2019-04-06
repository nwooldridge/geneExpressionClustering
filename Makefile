CC=g++ -g -std=c++0x
TARGET=source
OBJS=source.o parser.o clusterAlgorithm.o generateCSV.o Iteration.o geneticAlgorithm.o fitnessFunction.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

source.o: 
	$(CC) -c source.cpp

parser.o:
	$(CC) -c parser.cpp

clusterAlgorithm.o:
	$(CC) -c clusterAlgorithm.cpp

generateCSV.o:
	$(CC) -c generateCSV.cpp

Iteration.o:
	$(CC) -c Iteration.cpp

geneticAlgorithm.o:
	$(CC) -c geneticAlgorithm.cpp

fitnessFunction.o:
	$(CC) -c fitnessFunction.cpp

clean:
	rm -f *.o $(TARGET)
