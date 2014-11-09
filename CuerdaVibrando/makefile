all:	string.x	string_0.01.dat	string_0.01.pdf

string.x:	grafica.py	solucion_cuerda.c
	touch string.x
	cc solucion_cuerda.c -o string.x -lm
string_0.01.dat:	string.x
	./string.x 0.01
string_0.01.pdf:	string_0.01.dat
	python grafica.py string_0.01.dat