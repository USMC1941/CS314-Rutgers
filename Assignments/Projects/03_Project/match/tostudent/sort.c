/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: sort.c                              *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#include "sort.h"

void swap(int *row, int *col, double *val, int x, int y)
{
    int tmpR, tmpC;
    double tmpV;
    tmpR = row[x];
    row[x] = row[y];
    row[y] = tmpR;
    tmpC = col[x];
    col[x] = col[y];
    col[y] = tmpC;
    tmpV = val[x];
    val[x] = val[y];
    val[y] = tmpV;
}

void quickSort_RCW(int *row, int *col, double *val, int left, int right)
{

    int i = left, j = right;
    int pivotRow = row[(left + right) / 2];
    int pivotCol = col[(left + right) / 2];
    double pivotVal = val[(left + right) / 2];

    while (i < j) {
        while (row[i] < pivotRow ||
               (row[i] == pivotRow && col[i] < pivotCol) ||
               (row[i] == pivotRow && col[i] == pivotCol && val[i] > pivotVal)) {
            i++;
        }
        while (row[j] > pivotRow ||
              (row[j] == pivotRow && col[j] > pivotCol) ||
              (row[j] == pivotRow && col[j] == pivotCol && val[j] < pivotVal)) {
            j--;
        }
        if (i <= j) {
            swap(row, col, val, i, j);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort_RCW(row, col, val, left, j);
    if (i < right)
        quickSort_RCW(row, col, val, i, right);
};

void sort_RCW(int *row, int *col, double *val, int nnz)
{
    quickSort_RCW(row, col, val, 0, nnz - 1);
}

void quickSort_RWC(int *row, int *col, double *val, int left, int right)
{

    int i = left, j = right;
    int pivotRow = row[(left + right) / 2];
    int pivotCol = col[(left + right) / 2];
    double pivotVal = val[(left + right) / 2];

    while (i < j) {
        while (row[i] < pivotRow ||
                (row[i] == pivotRow && val[i] > pivotVal) ||
                (row[i] == pivotRow && val[i] == pivotVal && col[i] < pivotCol)) {
            i++;
        }
        while (row[j] > pivotRow ||
                (row[j] == pivotRow && val[j] < pivotVal) ||
                (row[j] == pivotRow && val[j] == pivotVal && col[j] > pivotCol)) {
            j--;
        }
        if (i <= j) {
            swap(row, col, val, i, j);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort_RWC(row, col, val, left, j);
    if (i < right)
        quickSort_RWC(row, col, val, i, right);
};

void sort_RWC(int *row, int *col, double *val, int nnz)
{
    quickSort_RWC(row, col, val, 0, nnz - 1);
}

void quickSort_WRC(int *row, int *col, double *val, int left, int right)
{

    int i = left, j = right;
    int pivotRow = row[(left + right) / 2];
    int pivotCol = col[(left + right) / 2];
    double pivotVal = val[(left + right) / 2];

    while (i < j) {
        while (val[i] > pivotVal ||
               (val[i] == pivotVal && row[i] < pivotRow) ||
               (val[i] == pivotVal && row[i] == pivotRow && col[i] < pivotCol)) {
            i++;
        }
        while (val[j] < pivotVal ||
               (val[j] == pivotVal && row[j] > pivotRow) ||
               (val[j] == pivotVal && row[j] == pivotRow && col[j] > pivotCol)) {
            j--;
        }
        if (i <= j) {
            swap(row, col, val, i, j);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort_WRC(row, col, val, left, j);
    if (i < right)
        quickSort_WRC(row, col, val, i, right);
};

void sort_WRC(int *row, int *col, double *val, int nnz)
{
    quickSort_WRC(row, col, val, 0, nnz - 1);
}
