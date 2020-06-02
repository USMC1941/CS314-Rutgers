/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: onewaywrapper.c                     *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#include "oneway.h"
#include "utils.h"

void one_way_handshake_wrapper(int nthreads, GraphData graph, int **res)
{
    int i;

    /* initialize pthread environment */
    /* allocate memory for pthreads */
    pthread_t *threads = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    /* allocate memory for pthread thread data */
    OneWayThreadData *td = (OneWayThreadData *) malloc(nthreads * sizeof(OneWayThreadData));
    /* initialize pthread barrier */
    pthread_barrier_t *barrier = (pthread_barrier_t *) malloc(sizeof(pthread_barrier_t));
    pthread_barrier_init(barrier, NULL, nthreads);

    /* prepare thread data */
    /* allocate memory for matching result for each round */
    int *strongNeighbor = (int *) malloc(graph.nNodes * sizeof(int));
    /* allocate memory for unmatched nodes */
    int *nodeToProcess = (int *) malloc(graph.nNodes * sizeof(int));
    int *newNodeToProcess = (int *) malloc(graph.nNodes * sizeof(int));
    int *nodeCount = (int *) malloc(nthreads * sizeof(int));
    int *startLocations = (int *) malloc((nthreads + 1) * sizeof(int));
    /* initialize with node index */
    for (i = 0; i < graph.nNodes; i++) nodeToProcess[i] = i;

    for (i = 0; i < nthreads; i++) {
        td[i].threadId = i;
        td[i].threadNum = nthreads;
        td[i].barrier = barrier;
        td[i].graph = graph;
        td[i].res = *res;
        td[i].strongNeighbor = strongNeighbor;
        td[i].nodeToProcess = nodeToProcess;
        td[i].nodeCount = nodeCount;
        td[i].startLocations = startLocations;
        td[i].newNodeToProcess = newNodeToProcess;
    }

    fprintf(stderr, "Start One Way Matching ... ");

    struct timeval beginTime, endTime;

    setTime(&beginTime);

    /* graph matching stage */
    int rc;
    long iter = 0;
    for (i = 0; i < nthreads; i++) {
        rc = pthread_create(&threads[i], NULL, one_way_handshake, (void *)&td[i]);
        if (rc) {
            ERROR("pthread_create failed with thread %d with return code %d\n", i, rc);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < nthreads; i++) {
        rc = pthread_join(threads[i], (void **)&iter);
        if (rc) {
            ERROR("pthread_join failed with thread %d with return code %d\n", i, rc);
            exit(EXIT_FAILURE);
        }
    }

    setTime(&endTime);

    fprintf(stderr, "SUCCESS\n");

    fprintf(stderr, "Find a maximal matching after %ld iterations\n", iter);
    fprintf(stderr, "One Way Handshaking Matching Time: %.2f ms\n",
            getTime(&beginTime, &endTime));

    /* clean-up */
    pthread_barrier_destroy(barrier); barrier = NULL;
    free(threads); threads = NULL;
    free(td); td = NULL;
    free(strongNeighbor); strongNeighbor = NULL;
    free(nodeToProcess); nodeToProcess = NULL;
    free(newNodeToProcess); newNodeToProcess = NULL;
    free(nodeCount); nodeCount = NULL;
    free(startLocations); startLocations = NULL;
}


void *one_way_handshake(void *threadArg)
{
    /* get data from threadArg */
    OneWayThreadData *td = (OneWayThreadData *)threadArg;
    int threadId = td->threadId;
    int threadNum = td->threadNum;
    pthread_barrier_t *barrier = td->barrier;
    GraphData graph = td->graph;
    int *res = td->res;
    int *strongNeighbor = td->strongNeighbor;
    int *nodeToProcess = td->nodeToProcess;
    int *nodeCount = td->nodeCount;
    int *startLocations = td->startLocations;
    int *newNodeToProcess = td-> newNodeToProcess;

    int remainNodesNum = graph.nNodes;

    /* start matching */
    long iter;
    for (iter = 0; ; iter++) {

        /* Step 1: Each vertex extends a hand to its strongest neighbor */
        extend_one_hand(threadId, threadNum, graph,
                        remainNodesNum, nodeToProcess,
                        res, strongNeighbor);
        pthread_barrier_wait(barrier);

        /* Step 2: Each vertex checks if there is a handshaking */
        check_handshaking(threadId, threadNum,
                          remainNodesNum, nodeToProcess,
                          strongNeighbor, res);
        pthread_barrier_wait(barrier);


        /* Step 3: Filter matched nodes */
        /* Step 3.1: Each thread counts the number of unmatched vertices */
        count_unmatched_vertices(threadId, threadNum, remainNodesNum,
                                 nodeToProcess, res,
                                 nodeCount);
        pthread_barrier_wait(barrier);

        /* Step 3.2: Perform prefix sum ont nodeCount */
        if (threadId == 0)
            exclusive_prefix_sum(nodeCount, startLocations, threadNum);
        pthread_barrier_wait(barrier);

        /* detect if we have find a maximal matching */
        if (startLocations[threadNum] == 0) break;

        /* Step 3.3: Each thread updates unmatched vertices in newNodeToProcess */
        update_remain_nodes_index(threadId, threadNum,
                                  nodeToProcess, startLocations,
                                  res,
                                  remainNodesNum, newNodeToProcess);
        pthread_barrier_wait(barrier);

        /* update remainNodesNum */
        remainNodesNum = startLocations[threadNum];
        /* swap remainIndex and tmpnodeToProcess */
        swapIntArray(&nodeToProcess, &newNodeToProcess);
    }

    pthread_exit((void *)iter);
}
