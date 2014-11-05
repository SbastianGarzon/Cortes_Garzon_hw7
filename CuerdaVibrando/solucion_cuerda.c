#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define T 40.0 /*Tension en Newton*/
#define L 100.0 /*Longitud en metros*/
#define n_points 100
#define t 121


int main (int argc, char **argv){
  
  int i;
  int j;
  int m,l,p;
  float rho = atof(argv[1]);
  float c = sqrt(T/rho);
  float r;
  float* u_inicial;
  float* u_futuro;
  float* u_presente;
  float* u_pasado;
  FILE* data;
  float delta = L/n_points;  
  u_inicial = malloc(n_points*sizeof(float));
  u_futuro = malloc(n_points*sizeof(float));
  u_pasado = malloc(n_points*sizeof(float));
  u_presente = malloc(n_points*sizeof(float));
  
  if(argc!=2){
   printf("debe introducir los parámetros de energía cinética y el ángulo pitch");
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
    u_futuro[i]= u_inicial[i] + (((pow(r,2))/2.0)*(u_inicial[i+1] - (2.0*u_inicial[i]) + u_inicial[i-1]));
  }
  
  for(i=0;i<n_points;i++){
    u_pasado[i]= u_inicial[i];
    u_presente[i]= u_futuro[i];
  }

  data = fopen("olakease.dat", "w");


  for(i=0;i<=t;i++){
    for(j=1;j<n_points-1;j++){
      u_futuro[j] = ((2.0*(1.0-pow(r,2)))*(u_presente[j]))-u_pasado[j]+ ((pow(r,2))*(u_presente[j+1]+u_presente[j-1]));
    }
    for(m=0;m<n_points;m++){
        u_pasado[m] = u_presente[m];
    }
    for(l=0;l<n_points;l++){
     u_presente[l] = u_futuro[l];
    }
    for(p=0;p<n_points;p++){
        if(p<(n_points-1)){
            fprintf(data, "%f ", u_presente[p]);
      }
        else{
            fprintf(data, "%f \n ", u_presente[p]);
        }
    }
  }
}
