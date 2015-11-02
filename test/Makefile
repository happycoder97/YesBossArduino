
objects = Utils.o UtilsTest.o
all: $(objects)
	g++ -o UtilsTest $(objects)

UtilsTest.o: Utils.o

Utils.o: ../Utils.cpp
	g++ -c ../Utils.cpp
