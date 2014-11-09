from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import sys
nombreArchivo = sys.argv[1]
data = np.loadtxt(nombreArchivo)
name=nombreArchivo.replace(".dat","")
y = data;
x = np.zeros(shape=(121,101))
t= np.zeros(shape=(121,101))
contador=0
contador1=0
for i in range (0,101):
    for j in range(0,121):
        x[j][i]=contador
    contador+=1
for i in range (0,121):
    for j in range(0,101):
        t[i][j]=contador1
    contador1+=1
fig = plt.figure(figsize=(9, 6))
ax = fig.add_subplot(111, projection = '3d')
ax.plot_wireframe(t,x,y,rstride=2,cstride=1,  color ='y')
ax.view_init(elev=40, azim=15)
ax.set_xlabel('tiempo',fontsize=15)
ax.set_ylabel('Eje x',fontsize=15)
ax.set_zlabel('Eje Y',fontsize=15)
ax.set_title('Comportamiento cuerda',fontsize=20)
plt.savefig(name+'.pdf', format = 'pdf', transparent = True)