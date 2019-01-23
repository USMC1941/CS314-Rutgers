/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: sort.h                              *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef SORT_H
#define SORT_H

/* sort the graph on basis of row index, then larger weight and then col index */
void sort_RWC(int *row, int *col, double *val, int nnz);

/* sort the graph on basis of row index, then col index and then larger weight */
void sort_RCW(int *row, int *col, double *val, int nnz);

/* sort the graph on basis of larger weight, then row index, and then col index */
void sort_WRC(int *row, int *col, double *val, int nnz);

#endif
