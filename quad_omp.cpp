#include <iostream>
#include <cmath>
#include <ctime>
#include <omp.h>
using namespace std;

double f(double x){
  double pi = 3.141592653589793;
  double value;
  value=50.0/(pi*(2500.0*x*x+1.0));
  return value;
}

double cpu_time (){
  double value;
  value = (double)clock()/(double)CLOCKS_PER_SEC;
  return value;
}

int main ()
{
  double a = 0.0,b = 10.0,error;
  double exact = 0.49936338107645674464;
  int i, n = 10000000;
  double total, wtime,wtime1,wtime2, x;

  //timestamp ( );
  printf ( "QUAD_OMP:\n" );
  printf ( "  Integral de f(x) = 50 / (pi*(2500*x*x+1)).\n" );
  printf ( "  A, B, N  = %f, %f, %d\n", a,b,n );
  printf ( "  Valor exacto = %24.16f\n", exact );

  wtime1 = cpu_time();

omp_set_dynamic(0);
omp_set_num_threads(12);
#pragma omp parallel private(i)
{
  total = 0.0;

  #pragma omp for reduction(+:total) private(x)
  for (int i=0; i<n; i++){
    x = ((n-i-1)*a+i*b)/(n-1);
    total=total+f(x);
  }
}

  wtime2 = cpu_time();

  total = (b-a)*total/n;
  error = fabs(total-exact);
  wtime = wtime2 - wtime1;

  printf ( "  Valor estimado = %24.16f\n", exact );
  printf ( "  Error    = %e\n", error );
  printf ( "  Tiempo     = %f\n", wtime );

  return 0;
}


