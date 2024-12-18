#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#define count_checks

#ifdef count_checks
unsigned long long radixchecks = 0;
unsigned long long mergechecks = 0;
unsigned long long bubblechecks = 0;
unsigned long long shellchecks = 0;
#endif // count_checks
void count(unsigned char* ar, int n, int period, int* cnt, float* ans)
{
#ifdef count_checks
    radixchecks += 256 + n + n;
#endif // count_checks

    int i;
    for (i = 0; i < n; i++) cnt[ar[i * sizeof(float) + period]]++;
    for (i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
    for (i = n - 1; i >= 0; i--) ans[--cnt[ar[i * sizeof(float) + period]]] = ((float*)ar)[i];
    for (i = 0; i < n; i++) ((float*)ar)[i] = ans[i];
}

void radix(float* ar, int n)
{
#ifdef count_checks
    radixchecks += 2 * n + 2 * n + n;
#endif // count_checks
    int* cnt = (int*)malloc(256* sizeof(int));
    float* ans = (float*)malloc(n* sizeof(float));
    int i, period, ptr;
    for (i = 0; i < sizeof(float); i++)
    {
        memset(cnt, 0, 256 * sizeof(int));
        memset(ans, 0, n * sizeof(float));
        count((unsigned char*)ar, n, i, cnt, ans);
    }
    memset(ans, 0, n * sizeof(float));
    ptr = 0;
    for (i = n - 1; i >= 0; i--) { if (ar[i] < 0) { ans[ptr] = ar[i]; ptr++; } }
    for (i = 0; i < n; i++) { if (ar[i] >= 0) { ans[ptr] = ar[i]; ptr++; } }
    for (i = 0; i < n; i++) { ar[i] = ans[i]; }
    free(cnt);
    free(ans);
}

void bubble(float* ar, int n)
{
    int i, haschanged = 1;
    float swap;
    while (haschanged)
    {
        haschanged = 0;
#ifdef count_checks
        bubblechecks += n + n;
#endif // count_checks
        for (i = 0; i < n - 1; i++)
        {
            if (ar[i] > ar[i + 1])
            {
                haschanged = 1;
                swap = ar[i];
                ar[i] = ar[i + 1];
                ar[i + 1] = swap;
            }
        }
    }
}

void shell(float* ar, int n, int* gaps, int gapsize)
{
    int gapi, i, j;
    float swap;
    for (gapi = gapsize - 1; gapi >= 0; gapi--)
    {
        int gap = gaps[gapi];
        for (i = gap; i < n; i++)
        {
            float swap = ar[i];
            for (j = i; (j >= gap) && (ar[j - gap] > swap); j -= gap)
            {
                ar[j] = ar[j - gap];
            }
            ar[j] = swap;
#ifdef count_checks
            shellchecks += 3;
#endif // count_checks
        }
    }

}

void merge(float* ar, int n, int l, int mid, int r, float* copar)
{
    int ptr = mid;
    int coparptr = l;
    int i;
    for (i = l; i < mid; i++)
    {
        while (ptr < r && ar[ptr] < ar[i]) { copar[coparptr++] = ar[ptr++]; }
        copar[coparptr++] = ar[i];
    }
    while (ptr < r) { copar[coparptr++] = ar[ptr++]; }
    for (i = l; i < r; i++) { ar[i] = copar[i]; }
#ifdef count_checks
    mergechecks += (r - l + 1) * 3;
#endif // count_checks
}

void mergesort(float* ar, int n, int l, int r, float* copar)
{
    int mid = r - ((r - l) / 2);
#ifdef count_checks
    mergechecks++;
#endif // count_checks
    if (r - l <= 1) { return; }
    mergesort(ar, n, l, mid, copar);
    mergesort(ar, n, mid, r, copar);
    memset(copar, 0, n * sizeof(float));
    merge(ar, n, l, mid, r, copar);
}

int* smoothgen()
{
    int n = 300;
    int i2 = 0, i3 = 0, ptr = 1;
    int* smooth = (int*)malloc(n * sizeof(int));
    smooth[0] = 1;
    while (ptr < n)
    {
        if (2 * smooth[i2] > 3 * smooth[i3])
        {
            smooth[ptr] = 3 * smooth[i3];
            i3++;
        }
        else
        {
            smooth[ptr] = 2 * smooth[i2];
            i2++;
        }
        if (smooth[ptr] != smooth[ptr - 1]) { ptr++; }
    }
    return smooth;
}

int main()
{
    int n;
    char dobub = 'y', doshell = 'n', domerge = 'n', dorad = 'n', printar = 'n';
    int i;
    float* arbubbl, * arshell, * arradix, * armerge;
    int* smooth;
    int which = 0;
    smooth = smoothgen();
    printf("enter the size of the array to be generated and sorted: ");
    scanf_s("%d", &n);
    arbubbl = (float*)malloc(n * sizeof(float));
    arshell = (float*)malloc(n * sizeof(float));
    arradix = (float*)malloc(n * sizeof(float));
    armerge = (float*)malloc(n * sizeof(float));

    printf("Run bubble sort? (y/n): ");
    scanf_s(" %c", &dobub);
    printf("Run shell sort? (y/n): ");
    scanf_s(" %c", &doshell);
    printf("Run merge sort? (y/n): ");
    scanf_s(" %c", &domerge);
    printf("Run radix sort? (y/n): ");
    scanf_s(" %c", &dorad);
    printf("Print the array to check whether or not the results are correct? (y/n): ");
    scanf_s(" %c", &printar);
    printf("\n");
    if (printar == 'y' || printar == 'Y') { printf("Starting array:\n"); }
    for (i = 0; i < n; i++)
    {
        arbubbl[i] = (((float)(rand()) / (float)(RAND_MAX)) - 0.5) * (float)1000000;
        arshell[i] = armerge[i] = arradix[i] = arbubbl[i];
        if (printar == 'y' || printar == 'Y') { printf("%f ", arbubbl[i]); }
    }
    clock_t start;
    clock_t end;
    printf("\n\n");
    if (dobub == 'y' || dobub == 'Y')
    {
        start = clock();
        bubble(arbubbl, n);
        end = clock();
        printf("bubble time - %d", end - start);
        printf("\n\n");
        if (printar == 'y' || printar == 'Y') { for (i = 0; i < n; i++) { printf("%f ", arbubbl[i]); } printf("\n\n"); }
#ifdef count_checks
        printf("amount of operations used in bubble - %lld\n\n", bubblechecks);
#endif
    }
    if (doshell == 'y' || doshell == 'Y')
    {
        start = clock();
        shell(arshell, n, smooth, 300);
        end = clock();
        printf("shell time - %d", end - start);
        printf("\n\n");
        if (printar == 'y' || printar == 'Y') { for (i = 0; i < n; i++) { printf("%f ", arshell[i]); } printf("\n\n"); }
#ifdef count_checks
        printf("amount of operations used in shell - %lld\n\n", shellchecks);
#endif
    }
    if (domerge == 'y' || domerge == 'Y')
    {
        float* copar = (float*)malloc(n * sizeof(float));
        start = clock();
        mergesort(armerge, n, 0, n, copar);
        end = clock();
        printf("merge time - %d", end - start);
        printf("\n\n");
        if (printar == 'y' || printar == 'Y') { for (i = 0; i < n; i++) { printf("%f ", armerge[i]); } printf("\n\n"); }
#ifdef count_checks
        printf("amount of operations used in merge - %lld\n\n", mergechecks);
#endif
    }
    if (dorad == 'y' || dorad == 'Y')
    {
        start = clock();
        radix(arradix, n);
        end = clock();
        printf("radix time - %d", end - start);
        printf("\n\n");
        if (printar == 'y' || printar == 'Y') { for (i = 0; i < n; i++) { printf("%f ", arradix[i]); } printf("\n\n"); }
#ifdef count_checks
        printf("amount of operations used in radix - %lld\n\n", radixchecks);
#endif
    }


}