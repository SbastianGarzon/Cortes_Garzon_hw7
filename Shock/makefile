all: sod_test.x estado_0.10000.dat graficar.py estado_0.10000.dat

sod_test.x: solucion_fluido.c
	cc solucion_fluido.c -lm -o sod_test.x
estado_0.10000.dat: sod_test.x
	./sod_test.x 0.1
graficar.py: estado_0.10000.dat
	python graficar.py estado_0.10000.dat
estado_0.10000.dat: graficar.py
	python graficar.py estado_0.10000.dat
