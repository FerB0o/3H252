run : bin/Animacion
	./bin/Animacion

bin/Animacion: src/Animacion.cpp
	c++ src/Animacion.cpp -o bin/Animacion -lftxui-screen