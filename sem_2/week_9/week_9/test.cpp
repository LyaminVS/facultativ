#include <stdio.h>
#include <time.h>
#include <omp.h>      //for omp_get_thread_num()

int main()
{
    /////////////////////////////////////////////////////////
    printf("zero case : OpenMP Test (max thread:%d)\n ", omp_get_max_threads() );
    int i;
    omp_set_num_threads(4);         //Set Thread Number
#pragma omp parallel for
    for (i = 0; i < 8; i++)
    {
        printf("%d Hello, World! Thread ID: %d\n", i,
            omp_get_thread_num());  //thread id
    }

    /////////////////////////////////////////////////////////
    printf("\nfirst case : no threading! \n ");
    double pi = 0.0;
    const size_t iterationCount = 20000000000;
    clock_t startTime = clock();
    for (int i = 0; i < iterationCount; i++)
    {
        pi += 4 * (i % 2 ? -1 : 1) / (2.0 * i + 1.0);
    }
    printf("Elpase Time : %.3lf sec \n", (clock() - startTime) / 1000.);
    printf("pi = %.8f\n", pi);
    /////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    printf("\nsecond case : OpenMP - threading! \n ");
    pi = 0.0;
    startTime = clock();

    #pragma omp parallel for
    for (size_t i = 0; i < iterationCount; i++)
    {
        pi += 4 * (i % 2 ? -1 : 1) / (2.0 * i + 1.0);
    }
    printf("Elpase Time : %.3lf sec \n", (clock() - startTime) / 1000.);
    printf("pi = %.8f\n", pi);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    printf("\nthird case : OpenMP - reduction set\n");
    pi = 0.0;
    startTime = clock();
    return 0;

//#pragma omp parallel num_threads(40)
    //{
#pragma omp parallel for reduction(+:pi) //num_threads( omp_get_max_threads() )
        for (int i = 0; i < iterationCount; i++)
        {
            pi += 4 * (i % 2 ? -1 : 1) / (2.0 * i + 1.0);
        }
    //}

    printf("Elpase Time : %.3lf sec \n", (clock() - startTime) / 1000.);
    printf("pi = %.8f\n", pi);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    printf("\nthird case : OpenMP - critical set\n");
    pi = 0.0;
    startTime = clock();

#pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < iterationCount; i++)
        {
            #pragma omp critical
            {
                pi += 4 * (i % 2 ? -1 : 1) / (2.0 * i + 1.0);
            }
        }
    }
    printf("Elpase Time : %.3lf sec \n", (clock() - startTime) / 1000.);
    printf("pi = %.8f\n", pi);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    printf("\nfourth case : OpenMP - private and reduction test\n");
    int num_steps = 200000000;
    double x, step, sum = 0.0;
    step = 1.0 / (double)num_steps;
    startTime = clock();
#pragma omp parallel for private(x) reduction(+:sum)
    for (i = 0; i<num_steps; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
    }
    printf("Elpase Time : %.3lf sec \n", (clock() - startTime) / 1000.);
    printf("PI = %.8f (sum = %.8f)\n", step*sum, sum);
}
