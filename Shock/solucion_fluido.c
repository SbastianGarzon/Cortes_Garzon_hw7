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

  //esto revisa la cantidad de parametros que se recojen por consola
  
  if(argc!=2){
    printf("debe introducir el parametro de tiempo");
    exit(1);
  }
  //declaracion de condiciones inciales y condiciones de frontera
  
  float v_iI = 0.0; //velocidad incial del lado izquierdo m/s
  float rho_iI =1.0; //densidad inicial del lado izquierdo kg/m3
  float P_iI =100000.0; //presion inicial del lado izquierdo N/m2
  float v_iD =0.0;  //velocidad incial del lado derecho m/s
  float rho_iD =0.125; //densidad inicial del lado derecho kg/m3
  float P_iD =10000.0; //presion inicial del lado derecho N/m2
  float t_min =0.0; //tiempo incial en seg
  float t_max =0.1; //tiempo final en seg
  float min_long =-10.0; //condicion de frontera izquierda
  float max_long =10.0; //condicion de frontera derecha
  float n_points =1000;
  float dt =0.00001;
  
  //creacion de punteros que representan las componentes de los vectores u y f, tambien se crean punteros para abrir un archivo y los datos de velocidad, densidad y presion para cada punto luego de ejecutar el metodo

  float* u1;
  float* u2;
  float* u3;
  float* f1;
  float* f2;
  float* f3;
  FILE* data;
  float* v;
  float* rho;
  float* P;

  float dx= (max_long-min_long)/1000;

  //otras variables importantes

  float gamma = 1.4;
  float delta = dt/dx;
  
  //dimensionar los punteros
  
  u1 = malloc(n_points*sizeof(float));
  u2 = malloc(n_points*sizeof(float));
  u3 = malloc(n_points*sizeof(float));
  f1 = malloc(n_points*sizeof(float));
  f2 = malloc(n_points*sizeof(float));
  f3 = malloc(n_points*sizeof(float));
  v=malloc(n_points*sizeof(float));
  rho=malloc(n_points*sizeof(float));  
  P=malloc(n_points*sizeof(float));
   
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
 
  
  //Aplicar el metodo, para esto se crearon punciones que hicieran todo por aparte

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
    rho[i]= u1[i];
    v[i] = u2[i]/u1[i];
    printf("%f %f \n",rho[i],v[i]); //aun no he sacado presion de cada punto esto es una prueba los datos estan dando mal
    
    

  }
  //codigo para guardar en el archivo .dat, sin embargo todavia no es necesario hasta que no funcione todo bien

  /*
  char n[150];
  sprintf(n,"estado_%.0f.dat",t_max);
  data = fopen(n, "w");
  for(i=0;i<n_points;i++){
    fprintf(data,"%f %f %f \n",u1[i],u2[i],u3[i]);
  }
  */
  return 0;
  
}

float predictor_solp(float* u1, float* f1, float delta,int i) {

//funcion que ejecuta la ecuacion 8 del metodo para i+1/2

  float u_mediop1;
  
  u_mediop1= (0.5*(u1[i+1]+u1[i])) - (delta*0.5*(f1[i+1]-f1[i]));
  return u_mediop1;
}
float predictor_soln(float* u1, float* f1, float delta,int i) {
  
//funcion que ejecuta la ecuacion 8 del metodo pero para i-1/2

  float u_mediop1;
  
  u_mediop1= (0.5*(u1[i-1]+u1[i])) - (delta*0.5*(f1[i-1]-f1[i]));
  return u_mediop1;
  
}
float hallarf1(float u2) {
  
//funcion que ejecuta halla la componente 1 de f

  float u_mediop1;
  
  u_mediop1= u2;
  return u_mediop1;

}
float hallarf2(float u1, float u2, float u3, float gamma) {

//funcion que ejecuta halla la componente 2 de f
  
  float u_mediop1;
  
  u_mediop1= (u2*u2/u1)+((gamma-1)*(u3 - (0.5*u2*u2/u1)));
  return u_mediop1;

}

float hallarf3(float u1, float u2, float u3, float gamma) {
  
//funcion que ejecuta halla la componente 3 de f

  float u_mediop1;
  
  u_mediop1= u3+((gamma-1)*(u3 - (0.5*u2*u2/u1)));
  
  return u_mediop1;
  
}
float corrector_sol(float* u1, float f_medp, float f_medn, float delta,int i){
  float u_mediop1;
 
  //funcion que ejecuta la ecuacion 9 del metodo

  u_mediop1= u1[i] - (delta*(f_medp-f_medn));
  
  return u_mediop1;
  
}
