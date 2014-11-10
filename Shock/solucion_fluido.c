#include<stdio.h>
#include<math.h>
#include<stdlib.h>

float predictor_solp(float* u1, float* f1, float delta,int i);
float predictor_soln(float* u1, float* f1, float delta,int i);
float hallarf1(float u2);
float hallarf2(float u1, float u2, float u3,float gamma);
float hallarf3(float u1, float u2, float u3,float gamma);
float corrector_sol(float* u1, float f_medp, float f_medn, float delta,int i);

int main (int argc, char **argv){

  if(argc!=2){
    printf("debe introducir el parametro de tiempo");
    exit(1);
  }

  float v_iI = 0.0;
  float rho_iI =1.0;
  float P_iI =100000.0;
  float v_iD =0.0;
  float rho_iD =0.125;
  float P_iD =10000.0;
  float t_min =0.0;
  float t_max =0.1;
  float min_long =-10.0;
  float max_long =10.0;
  float n_points =1000;
  float dt =0.00001;
  
  float* u1;
  float* u2;
  float* u3;
  float* f1;
  float* f2;
  float* f3;
  FILE* data;

  float dx= (max_long-min_long)/1000;
  float gamma = 1.4;
  float delta = dt/dx;
  
  
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
    if(i<=(0.5*n_points)){
      u1[i]=rho_iI;
      u2[i]=rho_iI*v_iI;
      u3[i]=P_iI*et_iI;
      f1[i]=u2[i];
      f2[i]=(u2[i]*u2[i]/u1[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
      f3[i]=(u3[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
    }
    else{
      u1[i]=rho_iD;
      u2[i]=rho_iD*v_iD;
      u3[i]=P_iD*et_iD;
      f1[i]=u2[i];
      f2[i]=(u2[i]*u2[i]/u1[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
      f3[i]=(u3[i])+((gamma-1)*(u3[i] - (0.5*u2[i]*u2[i]/u1[i])));
    }
  }
 
  
  //comenzar for de tiempo
  float u_mediop1;
  float u_medion1;
  float u_mediop2;
  float u_medion2;
  float u_mediop3;
  float u_medion3;
  float f_mediop1;
  float f_medion1;
  float f_mediop2;
  float f_medion2;
  float f_mediop3;
  float f_medion3;
  
  for(i=1;i<n_points-1;i++){
    u_mediop1 = predictor_solp(u1,f1,delta,i);
    u_mediop2 = predictor_solp(u2,f2,delta,i);
    u_mediop3 = predictor_solp(u3,f3,delta,i);
    u_medion1 = predictor_soln(u1,f1,delta,i);
    u_medion2 = predictor_soln(u2,f2,delta,i);
    u_medion3 = predictor_soln(u3,f3,delta,i);
    f_mediop1 = hallarf1(u_mediop2);
    f_medion1 = hallarf1(u_medion2);
    f_mediop2 = hallarf2(u_mediop1,u_mediop2,u_mediop3,gamma);
    f_medion2 = hallarf2(u_medion1,u_medion2,u_medion3,gamma);
    f_mediop3 = hallarf3(u_mediop1,u_mediop2,u_mediop3,gamma);
    f_medion3 = hallarf3(u_medion1,u_medion2,u_medion3,gamma);
    u1[i+1] = corrector_sol(u1,f_mediop1,f_medion1,delta,i);
    u2[i+1] = corrector_sol(u2,f_mediop2,f_medion2,delta,i);
    u3[i+1] = corrector_sol(u3,f_mediop3,f_medion3,delta,i);

  }


  char n[150];
  sprintf(n,"estado_%.0f.dat",t_max);
  data = fopen(n, "w");
  for(i=0;i<n_points;i++){
    fprintf(data,"%f %f %f \n",u1[i],u2[i],u3[i]);
  }

  return 0;
  
}

float predictor_solp(float* u1, float* f1, float delta,int i) {
  
  float u_mediop1;
  
  u_mediop1= (0.5*(u1[i+1]+u1[i])) - (delta*0.5*(f1[i+1]-f1[i]));
  return u_mediop1;
}
float predictor_soln(float* u1, float* f1, float delta,int i) {
  
  float u_mediop1;
  
  u_mediop1= (0.5*(u1[i-1]+u1[i])) - (delta*0.5*(f1[i-1]-f1[i]));
  return u_mediop1;
  
}
float hallarf1(float u2) {
  
  float u_mediop1;
  
  u_mediop1= u2;
  return u_mediop1;

}
float hallarf2(float u1, float u2, float u3, float gamma) {
  
  float u_mediop1;
  
  u_mediop1= (u2*u2/u1)+((gamma-1)*(u3 - (0.5*u2*u2/u1)));
  return u_mediop1;

}

float hallarf3(float u1, float u2, float u3, float gamma) {
  
  float u_mediop1;
  
  u_mediop1= u3+((gamma-1)*(u3 - (0.5*u2*u2/u1)));
  
  return u_mediop1;
  
}
float corrector_sol(float* u1, float f_medp, float f_medn, float delta,int i){
  float u_mediop1;
  
  u_mediop1= u1[i] - (delta*(f_medp-f_medn));
  
  return u_mediop1;
  
}
