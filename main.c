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

int main() {

    return 0;
}
