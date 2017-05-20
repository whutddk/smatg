INCLUDE = $(shell pkg-config --cflagsopencv) include 
LIBS = $(shell pkg-config --libs opencv ) -lwiringPi
VPATH=src:include

main:main.o rec.o itac.o learn.o shot.o
	g++ -o main main.o rec.o OLED.o itac.o learn.o shot.o -I $(INCLUDE) $(LIBS)  

rec.o:./src/rec.cpp 
	g++ -c -o rec.o ./src/rec.cpp -I $(INCLUDE) $(LIBS) 

shot.o:./src/shot.cpp
	g++ -c -o shot.o ./src/shot.cpp -I $(INCLUDE) $(LIBS) 

itac.o:./src/itac.cpp
	g++ -c -o itac.o ./src/itac.cpp -I $(INCLUDE) $(LIBS) 

learn.o:./src/learn.cpp 
	g++ -c -o learn.o ./src/learn.cpp -I $(INCLUDE) $(LIBS) 

main.o:./src/main.cpp
	g++ -c -o main.o ./src/main.cpp -I $(INCLUDE) $(LIBS) 
 






