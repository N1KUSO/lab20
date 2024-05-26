#include <stdio.h>
#include "matrix.h"

void task1(int n, int queries[][4], int queriesSize, int **resultMatrix) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            resultMatrix[i][j] = 0;
        }
    }

    for (int q = 0; q < queriesSize; ++q) {
        int row1 = queries[q][0];
        int col1 = queries[q][1];
        int row2 = queries[q][2];
        int col2 = queries[q][3];

        for (int i = row1; i <= row2; ++i) {
            for (int j = col1; j <= col2; ++j) {
                resultMatrix[i][j] += 1;
            }
        }
    }
}

int countLiveNeighbors(int **board, int m, int n, int x, int y) {
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < m && ny >= 0 && ny < n) {
                liveNeighbors += board[nx][ny];
            }
        }
    }
    return liveNeighbors;
}

void task2(int **board, int m, int n) {
    int **nextBoard = (int **) malloc(m * sizeof(int *));
    for (int i = 0; i < m; ++i) {
        nextBoard[i] = (int *) malloc(n * sizeof(int));
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int liveNeighbors = countLiveNeighbors(board, m, n, i, j);

            if (board[i][j] == 1) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextBoard[i][j] = 0;
                } else {
                    nextBoard[i][j] = 1;
                }
            } else {
                if (liveNeighbors == 3) {
                    nextBoard[i][j] = 1;
                } else {
                    nextBoard[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = nextBoard[i][j];
        }
        free(nextBoard[i]);
    }
    free(nextBoard);
}

int main() {

    return 0;
}

