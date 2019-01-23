/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: utils.h                             *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <error.h>
#include <sys/time.h>

#include "DataStructure.h"
#include "sort.h"
#include "match.h"
#include "mmio.h"

#define BUG(...) err("BUG", __func__, __LINE__, __VA_ARGS__)
#define ERROR(...) err("ERROR", __func__, __LINE__, __VA_ARGS__)
#define WARNING(...) err("WARNING", __func__, __LINE__, __VA_ARGS__)
#define DEBUG(...) err("DEBUG", __func__, __LINE__, __VA_ARGS__)

#define RUN_SEQUENTIAL 0

int err(const char *level, const char *func, int line, const char *fmt, ...);

void setTime(struct timeval *time);

double getTime(struct timeval *startTime, struct timeval *endTime);

/* swap two int array */
void swapIntArray(int **arrayA, int **arrayB);

/* exclusive prefix sum on num and output the result to sum */
void exclusive_prefix_sum(int *num, int *sum, int size);

/* inclusive prefix sum on num and output the result to sum */
void inclusive_prefix_sum(int *num, int *sum, int size);

/* print the graph */
void print_graph(GraphData graph);

/* print matching results in human-readable format */
void print_result(int *res, int nNodes);

/* print program usage */
void printUsage();

/* read matrix market format file */
void readmm(char *fileName, GraphData *graph);

/* free graph allocated array */
void graphFree(GraphData *graph);

/* write matching results to output file */
void write_match_result(char *fileName, int *res, int nNodes);

/* read matching result from output file */
void readres(char *fileName, int nNodes, int **res);

void check_result(GraphData graph, int *res);

#endif
