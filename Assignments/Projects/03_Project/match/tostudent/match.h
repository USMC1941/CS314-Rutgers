/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: match.h                             *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef MATCH_H
#define MATCH_H

#include "utils.h"

#define UNMATCHED -1
#define NO_MATCHED_NODES -2
#define INITIAL -3

void one_way_handshake_wrapper(int nthreads, GraphData graph, int ** res);

void n_ways_handshake_wrapper(int nthreads, int nWays, GraphData graph, int ** res);

#endif
