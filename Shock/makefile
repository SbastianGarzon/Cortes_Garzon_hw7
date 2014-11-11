#Este makefile funciona para la condicion t=0.1 s, para que funcione en otra condicion se debe cambiar el numero que aparece luego de _ en el archivo .dat, recuerde que se deben conservar 5 decimales. Ademas, debe cambiar en el comando del target estado_0.10000.dat el numero que va luego de ./sod_test.x por su tiempo de preferencia

all: sod_test.x estado_0.100000.dat graficar.py estado_0.100000.pdf

sod_test.x: solucion_fluido.c
	cc solucion_fluido.c -lm -o sod_test.x
estado_0.100000.dat: sod_test.x
	./sod_test.x 0.1
estado_0.100000.pdf: estado_0.100000.dat graficar.py
	python graficar.py estado_0.100000.dat

