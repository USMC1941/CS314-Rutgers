/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: nways.h                             *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef N_WAYS_H
#define N_WAYS_H

#include "match.h"
#include "oneway.h"
#include "filter.h"
#include "utils.h"

typedef struct NWayThreadData
{
    int threadId;
    int threadNum;
    pthread_barrier_t *barrier;
    int nWays;
    GraphData graph;
    int *res;
    int *strongNeighbor;
    int *nWayGraph;
    int *nWayGraphDegree;
    int *nodeToProcess;
    int *nodeCount;
    int *startLocations;
    int *newNodeToProcess;
} NWayThreadData;

void *n_ways_handshake(void *threadArg);

/*
 * INPUT:
 * threadId: Thread index of one active context.
 * threadNum: Total thread number.
 * graph: The graph object.
 * nWays: N-way handshaking which means the number of hands extend.
 * nodeNum: The number of current unmatched vertices.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * res: The array that stores the matching status for each vertex.
 *      The size of the array is the number of vertices.
 * OUTPUT:
 * nWayGraphDegree: The array that stores the degree of each vertex in the N-way graph.
 *                  The size of the array is the number of vertices.
 * nWayGraph: The array that stores the adjacency list of the nodes in the N-way graph.
 *            The size of the array is the number of vertices multiplied by N.
 */
void generate_n_way_graph(int threadId, int threadNum,
                          GraphData graph, int nWays,
                          int nodeNum, int *nodeToProcess,
                          int *res,
                          int *nWayGraphDegree, int *nWayGraph);
/*
 * INPUT:
 * threadId: Thread index of one active context.
 * threadNum: Total thread number.
 * nWays: N-way handshaking which means the number of hands extend.
 * nodeNum: The number of current unmatched vertices.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * nWayGraphDegree: The array that stores the degree of each vertex in the N-way graph.
 *                  The size of the array is the number of vertices.
 * nWayGraph: The array that stores the adjacency list of the nodes in the N-way graph.
 *            The size of the array is N * the number of vertices.
 * OUTPUT:
 * strongNeighbor: The array that stores the indices of the strongest neighbors.
 *                 The size of the array is the number of vertices.
 */
void prune_n_way_graph(int threadId, int threadNum, int nWays,
                                  int nodeNum, int *nodeToProcess,
                                  int *nWayGraphDegree, int *nWayGraph,
                                  int *strongNeighbor);
#endif
