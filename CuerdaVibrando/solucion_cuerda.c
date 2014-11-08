#include<stdio.h>
#include<math.h>
#include<stdlib.h>
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
    
  u_inicial = malloc(n_points*sizeof(float));
  u_futuro = malloc(n_points*sizeof(float));
  u_pasado = malloc(n_points*sizeof(float));
  u_presente = malloc(n_points*sizeof(float));
  
  if(argc!=2){
   printf("debe introducir el parametro de densidad ");
    exit(1);
  }

  for(i=0;i<n_points;i++){
    if((i*delta)<=(0.8*L)){
      u_inicial[i]=(1.25*i*delta)/L;
    }
    else{
      u_inicial[i]=5-((5*i*delta)/L);
    }
  }
    
  u_inicial[0] = 0.0;
  u_inicial[n_points-1] = 0.0;
  u_futuro[0] = 0.0;
  u_futuro[n_points-1] = 0.0;

  for(i=1;i<n_points-1;i++){
      u_futuro[i] = u_inicial[i] + (pow(r,2)/2.0) * (u_inicial[i+1] - 2.0 * u_inicial[i] + u_inicial[i-1]);
  }
  copiar(u_pasado,u_inicial);
  copiar(u_presente,u_futuro);
  data = fopen("olakease.dat", "w");


  for(i=0;i<=t;i++){
    for(j=1;j<n_points;j++){
      u_futuro[j] = (2.0*(1.0-pow(r,2)))*u_presente[j] - u_pasado[j] + (pow(r,2))*(u_presente[j+1] +  u_presente[j-1]);
      fprintf(data, "%f ", u_futuro[j]);
    }
    fprintf(data,"\n");
    copiar(u_pasado,u_presente);
    copiar(u_presente,u_futuro);
  }
}

void copiar(float* a, float *b){
    int i;
    for(i=0;i<n_points;i++){
       a[i]=b[i];
  }
}



