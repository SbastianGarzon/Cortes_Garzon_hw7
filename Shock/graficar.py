import matplotlib.pyplot as plt
import numpy as np
import sys

nombreArchivo = sys.argv[1]
data = np.loadtxt(nombreArchivo)
name=nombreArchivo.replace(".dat","")

x = data[:,0]
v = data[:,1]
P = data[:,2]
rho = data[:,3]

fig = plt.figure(figsize=(9, 6))
plt.subplot(2,2,1)
plt.plot(x,v)
plt.xlabel('X (m)')
plt.ylabel('V (m/s)')
plt.title('Velocidad en el tubo')

plt.subplot(2,2,2)
plt.plot(x,P)
plt.xlabel('X')
plt.ylabel('P (N/m2)')
plt.title('Presion en el tubo')


plt.subplot(2,2,3)
plt.plot(x,rho)
plt.xlabel('X')
plt.ylabel('rho (Kg/m3)')
plt.title('Densidad en el tubo')

plt.savefig(name+'.pdf', format = 'pdf', transparent = True)

