#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>
//#include <stdlib>
// using namespace std;

double f(double x) {
    double pi = 3.141592653589793;
    double value;
    value = 50.0 / (pi * (2500.0 * x * x + 1.0));
    return value;
}

/*
void timestamp (){
# define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;
  now = time ( NULL );
  tm = localtime ( &now );
  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
  printf ( "%s\n", time_buffer );
  return;
# undef TIME_SIZE
}
*/
int main(int argc, char *argv[]) {
    double a = 0.0, b = 10.0, error;
    double exact = 0.49936338107645674464;
    int i, n = 10000000;
    double total, wtime, wtime1, wtime2, x;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("QUAD_MPI:\n");
        printf("  Integral de f(x) = 50 / (pi*(2500*x*x+1)).\n");
        printf("  A, B, N  = %f, %f, %d\n", a, b, n);
        printf("  Valor exacto = %24.16f\n", exact);
    }

    double time = MPI_Wtime();
    double local = 0.0;

    for (int i = 0; i < n / size; i++) {
        x = ((n - i - 1) * a + i * b) / (n - 1);
        local = local + f(x);
    }

    MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double ftime = MPI_Wtime() - time;

    if (rank == 0) {
        total = (b - a) * total / n;
        error = fabs(total - exact);

        printf("  Valor estimado = %24.16f\n", exact);
        printf("  Error    = %e\n", error);
        printf("  Tiempo     = %f\n", ftime);
    }

    MPI_Finalize();
    return 0;
}
