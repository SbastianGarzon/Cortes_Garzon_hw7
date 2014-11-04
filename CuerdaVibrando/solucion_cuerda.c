#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define T 40.0 /*Tension en Newton*/
#define L 100.0 /*Longitud en metros*/
#define n_points 1000 
 

int main (int argc, char **argv){

  int i;
  float rho = atof(argv[1]);
  float c = sqrt(T/rho);
  float r;
  float* u_inicial;
  u_inicial = malloc(n_points*sizeof(float));
  float delta = L/n_points;

  

  for(i=0;i<n_points;i++){
    if((i*delta)<=(0.8*L)){
      u_inicial[i]=(1.25*i)/L;
    }
    else{
      u_inicial[i]=(5-5*i)/L;
    }
  }



  if(argc!=2){
    printf("debe introducir los parámetros de energía cinética y el ángulo pitch");
    exit(1);
  }


}
