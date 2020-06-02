/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: main.c                              *
 *  Date: 11/25/2018                          *
 **********************************************
 */

#include "utils.h"
#include "match.h"
#include "DataStructure.h"

typedef enum {
    ONEWAY = 1,
    NWAYS = 2,
} methodType;


int main(int argc, char *argv[])
{
    char *inputFile, *outputFile;
    methodType method;
    int nWays = 1;
    int nthreads;

    if (argc != 5 && argc != 6) {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int i = 1;

    inputFile = argv[i++];
    outputFile = argv[i++];
    if (!strcmp(argv[i], "oneWay")) {
        if (argc != 5) {
            printUsage(argv[0]);
            exit(EXIT_FAILURE);
        }
        i++;
        method = ONEWAY;
        nWays = 1;
    }
    else if (!strcmp(argv[i], "nWays")) {
        if (argc != 6) {
            printUsage(argv[0]);
            exit(EXIT_FAILURE);
        }
        i++;
        method = NWAYS;
        nWays = atoi(argv[i++]);
        if (nWays < 2) {
            printUsage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else {
        ERROR("Undefined method.\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    nthreads = atoi(argv[i++]);

    GraphData graph;

    /* read the matrix/graph from the matrix market format file(.mtx) and sort it*/
    readmm(inputFile, &graph);

    /* allocate memory for matching result */
    int *res = (int *) malloc(graph.nNodes * sizeof(int));
    /* initialize res to UNMATCHED */
    for (i = 0 ; i < graph.nNodes; i++) res[i] = UNMATCHED;

    if (method == ONEWAY) {
        one_way_handshake_wrapper(nthreads, graph, &res);
    }
    else if (method == NWAYS) {
        n_ways_handshake_wrapper(nthreads, nWays, graph, &res);
    }

    /* output matching result to output file */
    write_match_result(outputFile, res, graph.nNodes);

    /* clean allocated memory */
    graphFree(&graph);
    free(res); res = NULL;

    return 0;
}
