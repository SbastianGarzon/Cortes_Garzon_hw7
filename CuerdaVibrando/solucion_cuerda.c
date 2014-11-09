#include<stdio.h>
#include<math.h>
#include<stdlib.h>
//Definiendo constantes importantes
#define T 40.0 /*Tension en Newton*/
#define L 100.0 /*Longitud en metros*/
#define n_points 100
#define t 120

void copiar(float* a, float *b);
int main (int argc, char **argv){
  
  int i;
  int j;
  float delta = L/n_points;
  int m,l,p;
  float delta_t=0.0145;
  float rho = atof(argv[1]);
  float c = sqrt(T/rho);
  float r=c*(delta_t/delta);
  float* u_inicial;
  float* u_futuro;
  float* u_presente;
  float* u_pasado;
  FILE* data;
// metodo que crea el nombre del archivo
  char nombrearchivo[100];
  sprintf(nombrearchivo,"string_%.2f.dat",rho);
// Punteros a actualizar
  u_inicial = malloc(n_points*sizeof(float));
  u_futuro = malloc(n_points*sizeof(float));
  u_pasado = malloc(n_points*sizeof(float));
  u_presente = malloc(n_points*sizeof(float));
// Metodo que comprueba que el input sea correcto
  if(argc!=2){
   printf("debe introducir el parametro de densidad ");
    exit(1);
  }
// metodo que crea las condiciones iniciales del sistema
  data = fopen(nombrearchivo, "w");
  for(i=0;i<n_points;i++){
    if((i*delta)<=(0.8*L)){
      u_inicial[i]=(1.25*i*delta)/L;
    }
    else{
      u_inicial[i]=5-((5*i*delta)/L);
    }
  }
//metodo que fija las condiciones de frontera
  u_inicial[0] = 0.0;
  u_inicial[n_points-1] = 0.0;
  u_futuro[0] = 0.0;
  u_futuro[n_points-1] = 0.0;
// metodo que actualiza las condiciones iniciales
  for(i=0;i<=n_points;i++){
      if(i==0 || i==n_points){
           fprintf(data, "%f ", u_futuro[i]);
      }
      else{
          u_futuro[i] = u_inicial[i] + (pow(r,2)/2.0) * (u_inicial[i+1] - 2.0 * u_inicial[i] + u_inicial[i-1]);
          fprintf(data, "%f ", u_futuro[i]);
      }
  }
  fprintf(data,"\n");
  copiar(u_pasado,u_inicial);
  copiar(u_presente,u_futuro);
// metodo que actualiza las condiciones del sistema a lo largo del tiempo
  for(i=0;i<t;i++){
    for(j=0;j<=n_points;j++){
        if (j==0 || j==n_points){
            u_futuro[j]=0;
            fprintf(data, "%f ", u_futuro[j]);
        }
        else{
            u_futuro[j] = (2.0*(1.0-pow(r,2)))*u_presente[j] - u_pasado[j] + (pow(r,2))*(u_presente[j+1] +  u_presente[j-1]);
            fprintf(data, "%f ", u_futuro[j]);
        }
    }
    fprintf(data,"\n");
    copiar(u_pasado,u_presente);
    copiar(u_presente,u_futuro);
  }
}
// metodo que actualiza punteros, a es el puntero a actualizar y b es el puntero que se copia en a.
void copiar(float* a, float *b){
    int i;
    for(i=0;i<n_points;i++){
       a[i]=b[i];
  }
}



