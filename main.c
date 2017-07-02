#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **allocate(int n);
void  deallocate(float **A, int n);

float **fill(float **A, int n);

float **swipeRows(float **A, int n, int r1, int r2);
float **swipeCols(float **A, int n, int c1, int c2);

float **rotate(float **A, int n);

float **pivot(float **A, int n, int p);

float **reduce(float **A, int n);
float **reduce2(float **A, int n);

float  *result(float **A, float *X, int n);

void echo(float **A, int n);

int main()
{
    int i, j, n = 1;
    float **A = fill(allocate(n+1), n);
    float  *X = (float *) malloc(sizeof(float) * n);

    n++;
    
    A = reduce(A, n);
    
    echo(A, n);
    
    A = rotate(A, n);
    
    echo(A, n);
    
    X = result(A, X, n);

    for (i = 0, j = n-1; i < n, j >= 0; i++, j--)
        printf("%f*x^%d + ", X[j], j);
    
    printf("\n");

    deallocate(A, n);

    return 0;
}

float **allocate(int n)
{
    float **A = (float **) malloc(sizeof(float *) * n);

    int i, j;

    for (i = 0; i < n; i++)
        A[i] = (float *) malloc(sizeof(float) * (n + 1));

    for (i = 0; i < n; i++)
        for (j = 0; j < n + 1; j++)
            A[i][j] = 0;

    return A;
}

void deallocate(float **A, int n)
{
    int i;
    for (i = 0; i < n; i++)
        free(A[i]);
    free(A);
}

float **fill(float **A, int n)
{
    FILE *f;
    int pts_exp = 11;
    int i, j, k;
    
    float test = 0;

    float *temp =  (float *) malloc(sizeof(float) * (pts_exp));
    float *temp2 = (float *) malloc(sizeof(float) * (pts_exp));

    if ((f = fopen("x.txt", "r")) == NULL) {
        perror("Error: fopen");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < pts_exp; i++)
        fscanf(f, "%f ", &temp[i]);

    fclose(f);

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            for (k = 0; k < pts_exp; k++)
                A[i][j] = A[i][j] + pow(temp[k], (2*n)-i-j);

    if ((f = fopen("y.txt", "r")) == NULL) {
        perror("Error: fopen");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < pts_exp; i++)
        fscanf(f, "%f ", &temp2[i]);

    fclose(f);

    for (i = 0; i <= n; i++)
        for (k = 0; k < pts_exp; k++)
            A[i][n+1] += pow(temp[k], n+1-i) * temp2[k];

    free(temp);
    free(temp2);

    return A;
}

float **swipeRows(float **A, int n, int r1, int r2)
{
    int i;

    float *temp = (float *) malloc(sizeof(float) * (n + 1));

    for (i = 0; i < n + 1; i++)
        temp[i] = A[r1][i];

    for (i = 0; i < n + 1; i++) {
        A[r1][i] = A[r2][i];
        A[r2][i] = temp[i];
    }

    free(temp);

    return A;
}

float **swipeCols(float **A, int n, int c1, int c2)
{
    int i;

    float *temp = (float *) malloc(sizeof(float) * (n));

    for (i = 0; i < n; i++)
        temp[i] = A[i][c1];

    for (i = 0; i < n; i++) {
        A[i][c1] = A[i][c2];
        A[i][c2] = temp[i];
    }

    free(temp);

    return A;
}

float **rotate(float **A, int n)
{
    int i, j;

    for (i = 0, j = n-1; i < n-2, j > 1; i++, j--) {
        A = swipeCols(A, n, i, j);
        A = swipeRows(A, n, i, j);
    }

    return A;
}

float **pivot(float **A, int n, int p)
{
    int i, j, temp = 0;
    float maior = A[p][p];

    for (j = p; j < n; j++) {
        for (i = j; i < n; i++) {
            if (A[i][p] > maior) {
                maior = A[i][p];
                temp = i;
            }
        }
        if (temp != 0)
            A = swipeRows(A, n, j, temp);
    }

    return A;
}

float **reduce(float **A, int n)
{
    int i, j, k;

    float temp;

    for (k = 0; k < n; k++) {
        A = pivot(A, n, k);
        for (i = k; i < n - 1; i++) {
            temp = A[i+1][k]/A[k][k];
            for (j = 0; j < n + 1; j++)
                A[i+1][j] = A[i+1][j] - (temp * A[k][j]);
        }
    }

    return A;
}

float **reduce2(float **A, int n)
{
    int i, j, k;

    float temp;

    for (k = 0; k < n; k++) {
        for (i = k; i < n - 1; i++) {
            temp = A[i+1][k]/A[k][k];
            for (j = 0; j < n + 1; j++)
                A[i+1][j] = A[i+1][j] - (temp * A[k][j]);
        }
    }

    return A;
}

float *result(float **A, float *X, int n)
{
    int i;

    for (i = 0; i < n; i++)
        X[i] = A[i][n]/A[i][i];

    return X;
}

void echo(float **A, int n)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n + 1; j++) {
            printf("%f | ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
