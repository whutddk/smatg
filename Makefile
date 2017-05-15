INCLUDE = $(shell pkg-config --cflagsopencv) include 
LIBS = $(shell pkg-config --libs opencv ) -lwiringPi
VPATH=src:include

main:main.o rec.o OLED.o itac.o learn.o
	g++ -o main main.o rec.o OLED.o itac.o learn.o -I $(INCLUDE) $(LIBS)  

rec.o:./src/rec.cpp 
	g++ -c -o rec.o ./src/rec.cpp -I $(INCLUDE) $(LIBS) 

OLED.o:./src/OLED.cpp 
	g++ -c -o OLED.o ./src/OLED.cpp -I $(INCLUDE) $(LIBS) 

itac.o:./src/itac.cpp
	g++ -c -o itac.o ./src/itac.cpp -I $(INCLUDE) $(LIBS) 

learn.o:./src/learn.cpp 
	g++ -c -o learn.o ./src/learn.cpp -I $(INCLUDE) $(LIBS) 

main.o:./src/main.cpp
	g++ -c -o main.o ./src/main.cpp -I $(INCLUDE) $(LIBS) 
 






