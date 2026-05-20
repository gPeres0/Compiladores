// N é o tamanho do vetor (1.000.000)
void soma_vetor_serial(double A[], double B[], double C[], int N) {
    for (int i = 0; i < N; i++) {
        // Esta operação leva 1 unidade de tempo
        C[i] = A[i] + B[i];
    }
}
// Tempo Total = N (1.000.000)


void soma_vetor_paralelo(double A[], double B[], double C[], int N) {
    // Diretiva OpenMP: O compilador irá paralelizar este 'for' loop
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        // Esta operação é independente de todas as outras
        C[i] = A[i] + B[i];
    }
}
// Tempo total = N / 4 (250.000)

