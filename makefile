render_OBJECTS:= render.o

default: render

render:
	g++ -lglfw -lglew -framework OpenGL helperFunctions.cpp render.cpp -o render.o