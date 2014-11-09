#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define gamma 1.4
#define v_iI 0.0
#define rho_iI 1.0
#define P_iI 100000.0
#define v_iD 0.0
#define rho_iD 0.125
#define P_iD 10000.0
#define t_min 0.0
#define t_max 0.1
#define min_long -10.0
#define max_long 10.0
#define n_points 1000
#define dt 0.00001

float predictor_sol(float* a, float *b, float* c, float* d, float* e, float* f);
float corrector_sol(float* a, float *b, float* c, float* d, floar* e, float* f);

int main (int argc, char **argv){
  
  if(argc!=2){
    printf("debe introducir el parametro de tiempo");
    exit(1);
  }

float* u1;
float* u2;
float* u3;
float* f1;
float* f2;
float* f3;

float dx= (max_long-min_long)/1000;

u1 = malloc(n_points*sizeof(float));
u2 = malloc(n_points*sizeof(float));
u3 = malloc(n_points*sizeof(float));
f1 = malloc(n_points*sizeof(float));
f2 = malloc(n_points*sizeof(float));
f3 = malloc(n_points*sizeof(float));



//inicializar los punteros de acuerdo a las condiciones iniciales
 int i;
 float et_iI = (P_iI/((gamma -1)*rho_iI)) - (0.5*v_iI*v_iI);
 float et_iD = (P_iD/((gamma -1)*rho_iD)) - (0.5*v_iD*v_iD);

 for (i=0;i<n_points;i++){
   while(i<=(0.5*n_points)){
     u1[i]=rho_iI;
     u2[i]=rho_iI*v_iI;
     u3[i]=P_iI*et_iI;
     f1[i]=u2[i];
     f2[i]=(u2[i]*u2[i]/u1[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
     f3[i]=(u3[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
   }
   while(i>(0.5*n_points)){
     u1[i]=rho_iD;
     u2[i]=rho_iD*v_iD;
     u3[i]=P_iD*et_iD;
     f1[i]=u2[i];
     f2[i]=(u2[i]*u2[i]/u1[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
     f3[i]=(u3[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
   }
 }

 //comenzar for de tiempo

 for(i=0;i<n_points;i++){
   for(j=1;j<n_points;j++){

     


   }

 }
 return 0;

}

float predictor_sol(float* u1, float *u2, float* u3, float* f1, float* f2, float* f3, float delta) {
  int j;
  int i;
  float u_mediop1;
  float u_medion1;
  float u_mediop2;
  float u_medion2;
  float u_mediop3;
  float u_medion3;

  for(i=1;i<n_points;i++){
	u_mediop1= (0.5*(u1[i+1]+u1[i])) - (delta*0.5*(f1[i+1]-f1[i]));
	u_medion1= (0.5*(u1[i-1]+u1[i])) - (delta*0.5*(f1[i-1]-f1[i]));
	u_mediop2= (0.5*(u2[i+1]+u2[i])) - (delta*0.5*(f2[i+1]-f2[i]));
	u_medion2= (0.5*(u2[i-1]+u2[i])) - (delta*0.5*(f2[i-1]-f2[i]));
	u_mediop3= (0.5*(u3[i+1]+u3[i])) - (delta*0.5*(f3[i+1]-f3[i]));
	u_medion3= (0.5*(u3[i-1]+u3[i])) - (delta*0.5*(f3[i-1]-f3[i]));
	f
  }
}

