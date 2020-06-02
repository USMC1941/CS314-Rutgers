/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: filter.h                            *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#ifndef FILTER_H
#define FILTER_H

#include "match.h"

/*
 * INPUT:
 * threadId: Thread index of one active context.
 * threadNum: Total thread number.
 * nodeNum: The number of current unmatched vertices.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * res: The array that stores the matching status for each vertex.
 *      The size of the array is the number of vertices.
 * OUTPUT:
 * nodeCount: Each element of the array stores the number of unmatched vertices in the corresponding thread's range.
 *            The size of the array is the number of thread number.
 */
void count_unmatched_vertices(int threadId, int threadNum, int nodeNum,
                              int *nodeToProcess, int *res,
                              int *nodeCount);

/*
 * INPUT:
 * threadId: Thread index of one active context.
 * threadNum: Total thread number.
 * nodeToProcess: Each element is a vertex ID, and this array is usually used to pass the unmatched vertices to the processing function.
 *                   The size of the array is the number of vertices.
 * startLocations: The array that stores the start location of each thread in newReaminNodesIndex.
 *                    The size of the array is the number of threads plus one.
 * res: The array that stores the matching status for each vertex.
 *      The size of the array is the number of vertices.
 * nodeNum: The number of current unmatched vertices.
 * OUTPUT:
 * newNodeToProcess: Each element is a vertex ID, and this array is usually used to pass the remaining unmatched vertices to the processing function for the next matching pass.
 *                   The size of the array is the number of vertices.
 */
void update_remain_nodes_index(int threadId, int threadNum,
                               int *nodeToProcess, int *startLocations,
                               int *res,
                               int nodeNum, int *newNodeToProcess);

#endif
