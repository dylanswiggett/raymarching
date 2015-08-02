CXX = g++
LINKERS = -lSDL2 -lGLEW -lGL
FLAGS = -Wall -g -std=c++11 -DUSE_MESA=ON
LIBS = main Scene shader_loader

main: $(foreach file, $(LIBS), $(file).o)
	$(CXX) $(FLAGS) -o main $(foreach file, $(LIBS), $(file).o) $(LINKERS)

main.o: main.cpp Scene.hpp
	$(CXX) $(FLAGS) -c -o main.o main.cpp

Scene.o: Scene.hpp Scene.cpp shader_loader.hpp
	$(CXX) $(FLAGS) -c -o Scene.o Scene.cpp

shader_loader.o: shader_loader.cpp shader_loader.hpp
	$(CXX) $(FLAGS) -c -o shader_loader.o shader_loader.cpp

clean:
	rm -f *.o
