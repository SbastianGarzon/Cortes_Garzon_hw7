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
  float t_max = atof(argv[1]); //tiempo final en seg
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
  float* x;

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
  x = malloc (n_points*sizeof(float));
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

  //hacer el arreglo del x

  for (i=0;i<n_points/2;i++){
    x[i]= -10 + (dx*i);
  }
  for (i=0;i<n_points/2;i++){
    int a = n_points/2;
    x[a +i]= 0 + (dx*i);
  }
  
  //Aplicar el metodo, para esto se crearon punciones que hicieran todo por aparte

  float* u_mediop1;
  float* u_mediop2;
  float* u_mediop3;
  float* f_mediop1;
  float* f_mediop2;
  float* f_mediop3;
  float* u1_new;
  float* u2_new;
  float* u3_new;
  float* f1_new;
  float* f2_new;
  float* f3_new;

  u_mediop1 = malloc(n_points*sizeof(float));
  u_mediop2 = malloc(n_points*sizeof(float));
  u_mediop3 = malloc(n_points*sizeof(float));

  f_mediop1 = malloc(n_points*sizeof(float));
  f_mediop2 = malloc(n_points*sizeof(float));
  f_mediop3 = malloc(n_points*sizeof(float));

  u1_new = malloc(n_points*sizeof(float));
  u2_new = malloc(n_points*sizeof(float));
  u3_new = malloc(n_points*sizeof(float));

  f1_new = malloc(n_points*sizeof(float));
  f2_new = malloc(n_points*sizeof(float));
  f3_new = malloc(n_points*sizeof(float));
  int j;
  int m;
  int k;
  int tiempo = t_max/dt;
  for(m=0;m<tiempo;m++){
    for(i=0;i<n_points-1;i++){
      u_mediop1[i] = predictor_solp(u1,f1,delta,i);
      u_mediop2[i] = predictor_solp(u2,f2,delta,i);
      u_mediop3[i] = predictor_solp(u3,f3,delta,i);
      
      f_mediop1[i] = hallarf1(u_mediop2[i]);
      f_mediop2[i] = hallarf2(u_mediop1[i],u_mediop2[i],u_mediop3[i],gamma);
      f_mediop3[i] = hallarf3(u_mediop1[i],u_mediop2[i],u_mediop3[i],gamma);
    }
    for(j=1;j<n_points-1;j++){
      u1_new[j] = corrector_sol(u1,f_mediop1[j],f_mediop1[j-1],delta,j);
      u2_new[j] = corrector_sol(u2,f_mediop2[j],f_mediop2[j-1],delta,j);
      u3_new[j] = corrector_sol(u3,f_mediop3[j],f_mediop3[j-1],delta,j);
      
      f1_new[j] = hallarf1(u2_new[j]);
      f2_new[j] = hallarf2(u1_new[j],u2_new[j],u3_new[j],gamma);
      f3_new[j] = hallarf3(u1_new[j],u2_new[j],u3_new[j],gamma);

      rho[j]=u1_new[j];
      v[j]=u2_new[j]/rho[j];
      P[j]=(gamma-1)*(u3_new[j]-u2_new[j]*u2_new[j]/(2.0*u1_new[j]));
    }
    for(k=0;k<n_points;k++){
      u1[k]=u1_new[k];
      u2[k]=u2_new[k];
      u3[k]=u3_new[k];
      f1[k]=f1_new[k];
      f2[k]=f2_new[k];
      f3[k]=f3_new[k];
    } 
  }
  
  
    
  //codigo para guardar en el archivo .dat

  char n[150];
  sprintf(n,"estado_%f.dat",t_max);
  data = fopen(n, "w");
  for(i=0;i<n_points;i++){
    fprintf(data,"%f %f %f %f \n",x[i],v[i],P[i],rho[i]);
  }
  
  return 0;
  
}

float predictor_solp(float* u1, float* f1, float delta,int i) {

//funcion que ejecuta la ecuacion 8 del metodo para i+1/2

  float u_mediop1;
  
  u_mediop1= (0.5*(u1[i+1]+u1[i])) - (delta*0.5*(f1[i+1]-f1[i]));
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
