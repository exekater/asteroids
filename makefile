CC=g++
OBJ = $(CC) -c $< -o $@ $(CXXFLAGS)
CXXFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

default: asteroids

asteroids: build/main.o build/menu.o build/interface.o
	$(CC) $^ -o $@ $(CXXFLAGS) 

build/main.o: src/main.cpp
	mkdir -p build
	 $(OBJ) -std=c++11

build/menu.o: src/menu.cpp
	$(OBJ) -std=c++11

build/interface.o: src/interface.cpp
	$(OBJ) -std=c++11

clean:
	rm -f build/*.o asteroids
	rmdir build
