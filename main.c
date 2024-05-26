#include <stdio.h>
#include <string.h>
#include "matrix.h"

#define MAX_SIZE 100

typedef struct DomainCount {
    char domain[MAX_SIZE];
    int count;
} DomainCount;

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;


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

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

void task3(int **matrix, int m, int n, int filterSize, int **resultMatrix) {
    int offset = filterSize / 2;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            resultMatrix[i][j] = matrix[i][j];
        }
    }

    for (int i = offset; i < m - offset; ++i) {
        for (int j = offset; j < n - offset; ++j) {
            int window[filterSize * filterSize];
            int k = 0;

            for (int wi = -offset; wi <= offset; ++wi) {
                for (int wj = -offset; wj <= offset; ++wj) {
                    window[k++] = matrix[i + wi][j + wj];
                }
            }
            qsort(window, filterSize * filterSize, sizeof(int), compare);

            resultMatrix[i][j] = window[(filterSize * filterSize) / 2];
        }
    }
}

void addDomainCount(DomainCount *domainCounts, int *domainCountSize, const char *domain, int count) {
    for (int i = 0; i < *domainCountSize; ++i) {
        if (strcmp(domainCounts[i].domain, domain) == 0) {
            domainCounts[i].count += count;
            return;
        }
    }
    strcpy(domainCounts[*domainCountSize].domain, domain);
    domainCounts[*domainCountSize].count = count;
    (*domainCountSize)++;
}

void task4(char *cpdomain, DomainCount *domainCounts, int *domainCountSize) {
    char *space = strchr(cpdomain, ' ');
    int count = atoi(cpdomain);
    char *domain = space + 1;

    addDomainCount(domainCounts, domainCountSize, domain, count);

    while ((domain = strchr(domain, '.')) != NULL) {
        domain++;
        addDomainCount(domainCounts, domainCountSize, domain, count);
    }
}

int task5(int **matrix, int m, int n) {
    int totalCount = 0;
    int heights[m][n];
    for (int j = 0; j < n; ++j) {
        heights[0][j] = matrix[0][j];
    }

    for (int i = 1; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            heights[i][j] = (matrix[i][j] == 0) ? 0 : heights[i - 1][j] + 1;
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int minHeight = heights[i][j];
            for (int k = j; k >= 0 && minHeight > 0; --k) {
                minHeight = (heights[i][k] < minHeight) ? heights[i][k] : minHeight;
                totalCount += minHeight;
            }
        }
    }

    return totalCount;
}


void task6(const char *pattern, char *result) {
    int n = strlen(pattern);
    int index = 0;
    int num = 1;
    int stack[n + 1];
    int top = -1;

    for (int i = 0; i <= n; ++i) {
        stack[++top] = num++;
        if (i == n || pattern[i] == 'I') {
            while (top >= 0) {
                result[index++] = '0' + stack[top--];
            }
        }
    }
    result[index] = '\0';
}

TreeNode *createNode(int val) {
    TreeNode *newNode = (TreeNode *) malloc(sizeof(TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int findMaxIndex(int *nums, int start, int end) {
    int maxIndex = start;
    for (int i = start + 1; i <= end; i++) {
        if (nums[i] > nums[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

TreeNode *constructMaximumBinaryTree(int *nums, int start, int end) {
    if (start > end) {
        return NULL;
    }
    int maxIndex = findMaxIndex(nums, start, end);
    TreeNode *root = createNode(nums[maxIndex]);
    root->left = constructMaximumBinaryTree(nums, start, maxIndex - 1);
    root->right = constructMaximumBinaryTree(nums, maxIndex + 1, end);

    return root;
}

void task7(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    TreeNode **queue = (TreeNode **) malloc(1000 * sizeof(TreeNode *));

    int front = 0, rear = 0;
    queue[rear++] = root;
    hk
    while (front < rear) {
        TreeNode *node = queue[front++];
        if (node) {
            printf("%d ", node->val);
            queue[rear++] = node->left;
            queue[rear++] = node->right;
        } else {
            printf("null ");
        }
    }

    free(queue);
    printf("\n");
}

int main() {

    return 0;
}

