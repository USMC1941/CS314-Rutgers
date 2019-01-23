/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: utils.c                             *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#include "utils.h"

void setTime(struct timeval *time)
{
    gettimeofday(time, NULL);
}

double getTime(struct timeval *startTime, struct timeval *endTime)
{
    struct timeval elapsedTime;
    timersub(endTime, startTime, &elapsedTime);
    return (elapsedTime.tv_sec * 1000.0 + elapsedTime.tv_usec / 1000.0);
}

int err(const char *level, const char *func, int line, const char *fmt, ...)
{
	char s[1024];
	va_list va;
	int r;

	va_start(va, fmt);
	vsprintf(s, fmt, va);
	va_end(va);
	r = fprintf(stderr, "\n[%s:%s:%d] %s", level, func, line, s);
	return r;
}

void printUsage(char *argv) {
    fprintf(stderr, "Usage: %s [input file] [output file] [method] [# hands] [# threads]\n"
                    "       e.g. ./match cant.mtx output oneWay 4\n"
                    "            ./match cant.mtx output nWays 2 4\n"
                    "       [input file]: input file (matrix market format)\n"
                    "       [output file]: output file (default: output)\n"
                    "       [method]: oneWay (default): one-way-handshaking method\n"
                    "                 nWays: n-way-handshaking method\n"
                    "       [# hands]: only needed when [method] = multi, should not be smaller than 2\n"
                    "       [# threads]: number of active threads\n", argv);
}

void swapIntArray(int **arrayA, int **arrayB) {
    int *tmp = *arrayA;
    *arrayA = *arrayB;
    *arrayB = tmp;
}

void exclusive_prefix_sum(int *num, int *sum, int size)
{
    int i;
    sum[0] = 0;
    for (i = 1; i <=size; i++) {
        sum[i] = sum[i - 1] + num[i - 1];
    }
}

void inclusive_prefix_sum(int *num, int *sum, int size)
{
    int i;
    sum[0] = num[0];
    for (i = 1; i < size; i++) {
        sum[i] = sum[i - 1] + num[i];
    }
}

void print_graph(GraphData graph)
{
    int i, j;
    fprintf(stderr, "%d Nodes %d Edges\n", graph.nNodes, graph.nEdges);
    for (i = 0; i < graph.nNodes; i++) {
        fprintf(stderr, "Node %d | Degree: %d | ", i, graph.degree[i]);
        for (j = 0; j < graph.offset[i + 1] - graph.offset[i]; j++)
            fprintf(stderr, "(%d, %.4f) ", graph.index[graph.offset[i] + j], graph.weight[graph.offset[i] + j]);
        fprintf(stderr, "\n");
    }
}

void print_result(int *res, int nNodes)
{
    int i;
    for (i = 0; i < nNodes; i++) {
        if (res[i] == NO_MATCHED_NODES) {
            fprintf(stderr, "%d -> NO MATCHED NODE\n", i);
        }
        else if (res[i] == UNMATCHED) {
            fprintf(stderr, "%d -> UNMATCHED\n", i);
        }
        else {
            fprintf(stderr, "%d -> %d\n", i, res[i]);
        }
    }
}

void write_match_result(char *fileName, int *res, int nNodes)
{
    FILE *f;
    if ((f = fopen(fileName, "w")) == NULL) {
        ERROR("Could not open the output file.\n");
        exit(EXIT_FAILURE);
    }

    int i;

    fprintf(stderr, "Start Writing Matching Results to %s ... ", fileName);

    for (i = 0; i < nNodes; i++) {
/*
        if (res[i] == NO_MATCHED_NODES) {
            fprintf(f, "%d -> NO MATCHED NODES\n", i);
        }
        else if (res[i] == UNMATCHED) {
            fprintf(f, "%d -> UNMATCHED\n", i);
        }
        else {
            fprintf(f, "%d -> %d\n", i, res[i]);
        }
*/
        fprintf(f, "%d\n", res[i]);
    }

    fclose(f);

    fprintf(stderr, "SUCCESS\n");

}

void readmm(char *fileName, GraphData *graph)
{
    FILE *f;
    MM_typecode matCode;
    int retCode;

    int i;

    fprintf(stderr, "Start Reading Matrix %s ... ", fileName);

    if ((f = fopen(fileName, "r")) == NULL) {
        ERROR("Could not open the input Matrix Market file.\n");
        exit(EXIT_FAILURE);
    }

    if (mm_read_banner(f, &matCode) != 0) {
        fprintf(stderr, "Matrix Market type: %s\n",
                mm_typecode_to_str(matCode));
        ERROR("Could not process Matrix Market banner.\n");
        exit(EXIT_FAILURE);
    }

    /* Read the size of the sparse matrix */
    int nrow, ncol, nnz;
    if ((retCode = mm_read_mtx_crd_size(f, &nrow, &ncol, &nnz)) != 0) {
        ERROR("Could not read the size of the sparse matrix.\n");
        exit(EXIT_FAILURE);
    }
/*
    if (nrow != ncol) {
        ERROR("The input matrix must be square.\n");
        exit(EXIT_FAILURE);
    }
*/

    /* Read the matrix and preprocess it*/
    int *_row = (int *) malloc(sizeof(int) * nnz);
    int *_col = (int *) malloc(sizeof(int) * nnz);
    double *_weight = (double *) malloc(sizeof(double) * nnz);
    for (i = 0; i < nnz; i++) {
        fscanf(f, "%d %d", &_row[i], &_col[i]);
        if (_row[i] == _col[i]) {
            // remove self circle
            if (fgetc(f) != '\n') fscanf(f, "%lg\n", &_weight[i]);
            nnz--;
            i--;
            continue;
        }
        else if (_row[i] > _col[i]) {
            int tmp = _row[i];
            _row[i] = _col[i];
            _col[i] = tmp;
        }
        _row[i]--;
        _col[i]--;
        if (fgetc(f) == '\n') {
            _weight[i] = 1;
        }
        else {
            fscanf(f, "%lg\n", &_weight[i]);
        }
    }


    sort_RCW(_row, _col, _weight, nnz);

    /* Reserve memory for the matrix */
    int *row = (int *) malloc(sizeof(int) * nnz * 2);
    graph->index = (int *) malloc(sizeof(int) * nnz * 2);
    graph->weight = (double *) malloc(sizeof(double) * nnz * 2);

    /* Read the matrix */
    int j = 0;
    for (i = 0; i < nnz; i++) {
        if (i != 0 && _row[i] == _row[i - 1] && _col[i] == _col[i - 1]) {
            continue;
        }

        row[j] = _row[i];
        graph->index[j] = _col[i];
        graph->weight[j] = _weight[i];
        j++;
        row[j] = _col[i];
        graph->index[j] = _row[i];
        graph->weight[j] = _weight[i];
        j++;
    }
    graph->nEdges = j;
    graph->nNodes = nrow >= ncol ? nrow : ncol;
    if (f != stdin) fclose(f);

    if (RUN_SEQUENTIAL) {
        fprintf(stderr, "\nEnable Sequential Matching\n");
        sort_WRC(row, graph->index, graph->weight, graph->nEdges);
        FILE *ff;
        if ((ff = fopen("seq.out", "w")) == NULL) {
            ERROR("Could not open the seq output file.\n");
            exit(EXIT_FAILURE);
        }
        int i;
        int *seqRes = (int *) malloc(graph->nNodes * sizeof(int));
        for (i = 0; i < graph->nNodes; i++) seqRes[i] = NO_MATCHED_NODES;
        for (i = 0; i < graph->nEdges; i++) {
            if (seqRes[row[i]] == NO_MATCHED_NODES && seqRes[graph->index[i]] == NO_MATCHED_NODES) {
                seqRes[row[i]] = graph->index[i];
                seqRes[graph->index[i]] = row[i];
            }
        }
        for (i = 0; i < graph->nNodes; i++) {
            fprintf(ff, "%d\n", seqRes[i]);
        }
        fclose(ff);
        free(seqRes); seqRes = NULL;
        exit(EXIT_SUCCESS);
    }
    sort_RWC(row, graph->index, graph->weight, graph->nEdges);


    graph->offset = (int *) calloc((graph->nNodes + 1), sizeof(int));
    graph->degree = (int *) calloc(graph->nNodes, sizeof(int));

    for (i = 0; i < graph->nEdges; i++) {
        graph->degree[row[i]]++;
    }

    exclusive_prefix_sum(graph->degree, graph->offset, graph->nNodes);

    free(row); row = NULL;
    free(_row); _row = NULL;
    free(_col); _col = NULL;
    free(_weight); _weight = NULL;

    fprintf(stderr, "SUCCESS\n");
}

void readres(char *fileName, int nNodes, int **res)
{
    FILE *f;
    if ((f = fopen(fileName, "r")) == NULL) {
        ERROR("Could not open result file.\n");
        exit(EXIT_FAILURE);
    }

    int i;

    for (i = 0; i < nNodes; i++) {
        fscanf(f, "%d\n", &(*res)[i]);
    }

    fclose(f);

}

void graphFree(GraphData *graph) {
    free(graph->offset); graph->offset = NULL;
    free(graph->index); graph->index = NULL;
    free(graph->weight); graph->weight = NULL;
    free(graph->degree); graph->degree = NULL;
}

void check_result(GraphData graph, int *res)
{
    // TODO write results in 1-base format
    int i, j;
    int *resMarker = (int *) malloc(graph.nNodes * sizeof(int));
    for (i = 0; i < graph.nNodes; i++) resMarker[i] = INITIAL;

    fprintf(stderr, "Start Check Matching Results ... ");
    for (i = 0; i < graph.nNodes; i++) {
        if (resMarker[i] != INITIAL) continue;
        int resNode = res[i];
        /* first check if a->b and b->a also both a and b are not matched*/
        if (resNode == UNMATCHED) {
            resMarker[i] = UNMATCHED;
            continue;
        }
        else if (resNode == NO_MATCHED_NODES) {
            resMarker[i] = NO_MATCHED_NODES;
            continue;
        }
        else if (resMarker[resNode] != INITIAL) {
            fprintf(stderr, "\nERROR: A node is matched to multiple nodes or the matched pair is not legal\n");
            fprintf(stderr, "%d -> %d\n", i, resNode);
            fprintf(stderr, "%d -> %d\n", resMarker[resNode], resNode);
            exit(EXIT_FAILURE);
        }
        else if (i != res[resNode]) {
            fprintf(stderr, "\nERROR: Unmatched Pair [%d, %d] is found\n", i, resNode);
            fprintf(stderr, "%d -> %d | %d -> %d\n",
                    i, resNode, resNode, res[resNode]);
            exit(EXIT_FAILURE);
        }
        else {
            resMarker[i] = resNode;
            resMarker[resNode] = i;
        }
        /* second check if b is a's neighbor */
        int neighborNum = graph.degree[i];
        int neighborOffset = graph.offset[i];
        bool flag = false;
        for (j = 0; j < neighborNum; j++) {
            if (graph.index[neighborOffset + j] == resNode) {
                flag = true;
                break;
            }
        }
        if (flag == false) {
            fprintf(stderr,
                    "\nERROR: Matched Node %d is not one of the neighbor nodes of %d\n",
                    resNode, i);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "SUCCESS\n");

    free(resMarker);
    resMarker = NULL;
}

