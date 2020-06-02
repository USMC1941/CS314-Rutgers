/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: oneway.h                            *
 *  Date: 11/25/2018                          *
 **********************************************
 */

#ifndef ONE_WAY_H
#define ONE_WAY_H

#include "match.h"
#include "filter.h"
#include "utils.h"

typedef struct OneWayThreadData
{
    int threadId;
    int threadNum;
    pthread_barrier_t *barrier;
    GraphData graph;
    int *res;
    int *strongNeighbor;
    int *nodeToProcess;
    int *nodeCount;
    int *startLocations;
    int *newNodeToProcess;
} OneWayThreadData;

void *one_way_handshake(void *threadArg);


/*
 * INPUT:
 * threadId: Thread index of one active context
 * threadNum: Total thread number.
 * graph: The graph object.
 * nodeNum: The number of current unmatched vertices.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * res: The array that stores the matching status for each vertex.
 *      The size of the array is the number of vertices.
 * OUTPUT:
 * strongNeighbor: The array that stores the indices of the strongest neighbors.
 *                 The size of the array is the number of vertices.
 */
void extend_one_hand(int threadId, int threadNum, GraphData graph,
                     int nodeNum, int *nodeToProcess,
                     int *res, int*strongNeighbor);

/*
 * INPUT:
 * threadId: Thread index of one active context
 * threadNum: Total thread number.
 * nodeNum: The number of current unmatched vertices.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * strongNeighbor: The array that stores the indices of the strongest neighbors.
 *                 The size of the array is the number of vertices.
 * OUTPUT:
 * res: The array that stores the matching status for each vertex.
 *      The size of the array is the number of vertices.
 */
void check_handshaking(int threadId, int threadNum,
                       int nodeNum, int *nodeToProcess,
                       int *strongNeighbor, int *res);

#endif
