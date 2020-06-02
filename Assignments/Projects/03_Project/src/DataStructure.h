/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: DataStructure.h                     *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef struct GraphData
{
    int nNodes;
    int nEdges;
    int *offset;
    int *degree;
    int *index;
    double *weight;
} GraphData;

#endif
