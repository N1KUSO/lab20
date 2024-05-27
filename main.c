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

typedef struct {
    char word[2000001];
} Word;

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

char *task8(char *s, int *indices, int length) {
    char *result = (char *) malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; i++) {
        result[indices[i]] = s[i];
    }

    result[length] = '\0';
    return result;
}

void task9(int N) {
    FILE *input_file = fopen("../input.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *output_file = fopen("../output.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    int num;
    while (fscanf(input_file, "%d", &num) != EOF) {
        if (num < N) {
            fprintf(output_file, "%d\n", num);
        }
    }

    fclose(input_file);
    fclose(output_file);
}

int compareStr(const void *a, const void *b) {
    return strcmp(((Word *)a)->word, ((Word *)b)->word);
}

int task11(Word *dictionary, int n, char *prefix) {
    int left = 0, right = n - 1, result = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (strncmp(dictionary[mid].word, prefix, strlen(prefix)) >= 0) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return result;
}

void test_task1() {
    int n = 3;
    int queries[][4] = {{1, 1, 2, 2}, {0, 0, 1, 1}};

    int **resultMatrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        resultMatrix[i] = (int *)malloc(n * sizeof(int));
    }
    task1(n, queries, sizeof(queries) / sizeof(queries[0]), resultMatrix);

    int expectedMatrix[][3] = {{1, 1, 0}, {1, 2, 1}, {0, 1, 1}};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (resultMatrix[i][j] != expectedMatrix[i][j]) {
                printf("Error! Task 1");
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        free(resultMatrix[i]);
    }
    free(resultMatrix);
}

void test_task2() {
    int m = 3;
    int n = 3;
    int **board = (int **)malloc(m * sizeof(int *));

    for (int i = 0; i < m; ++i) {
        board[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            board[i][j] = 0;
        }
    }
    board[0][1] = 1;
    board[1][0] = 1;
    board[1][1] = 1;

    task2(board, m, n);

    for (int i = 0; i < m; ++i) {
        free(board[i]);
    }
    free(board);
}

void test_task3() {
    int m = 3;
    int n = 3;
    int filterSize = 3;

    int inputMatrix[3][3] = {
            {10, 20, 30},
            {25, 35, 45},
            {15, 25, 35}
    };

    int exceptedMatrix[3][3] = {
            {10, 20, 30},
            {25, 25, 45},
            {15, 25, 35}
    };

    int** matrix = (int**)malloc(m * sizeof(int*));
    int** resultMatrix = (int**)malloc(m * sizeof(int*));

    for (int i = 0; i < m; ++i) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        resultMatrix[i] = (int*)malloc(n * sizeof(int));

        for (int j = 0; j < n; ++j) {
            matrix[i][j] = inputMatrix[i][j];
        }
    }

    task3(matrix, m, n, filterSize, resultMatrix);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (resultMatrix[i][j] != exceptedMatrix[i][j]) {
                printf("Error! Task 3");
            }
        }
    }

    for (int i = 0; i < m; ++i) {
        free(matrix[i]);
        free(resultMatrix[i]);
    }
    free(matrix);
    free(resultMatrix);
}

void test_task4() {
    char* cpdomains[] = {"900 google.mail.com", "50 yahoo.com", "1 intel.mail.com", "5 wiki.org"};
    int cpdomainsSize = sizeof(cpdomains) / sizeof(cpdomains[0]);

    DomainCount domainCounts[MAX_SIZE];
    DomainCount excepted[MAX_SIZE] = {
            {"google.mail.com", 900},
            {"mail.com", 901},
            {"com", 951},
            {"yahoo.com", 50},
            {"intel.mail.com", 1},
            {"wiki.org", 5},
            {"org", 5},
    };
    int domainCountSize = 0;

    for (int i = 0; i < cpdomainsSize; ++i) {
        task4(cpdomains[i], domainCounts, &domainCountSize);
    }

    for (int i = 0; i < domainCountSize; ++i) {
        if(domainCounts[i].count != excepted[i].count || strcmp(domainCounts[i].domain, excepted[i].domain)) {
            printf("Error! Task 4");
        }
    }
}

void test_task5() {
    int m = 3, n = 3;
    int matrixData[3][3] = {
            {1, 0, 1},
            {1, 1, 0},
            {1, 1, 0}
    };

    int** matrix = (int**)malloc(m * sizeof(int*));
    for (int i = 0; i < m; ++i) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = matrixData[i][j];
        }
    }

    int result = task5(matrix, m, n);
    if(result != 13) {
        printf("Error! Task 5");
    }

    for (int i = 0; i < m; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void test_task6() {
    const char* pattern1 = "IIIDIDDD";
    char result1[10];
    task6(pattern1, result1);
    assert(strcmp(result1, "12349876"));

    const char* pattern2 = "DDD";
    char result2[5];
    task6(pattern2, result2);
    assert(strcmp(result1, "4321"));
}

void test_task7() {
    int nums1[] = {3, 2, 1, 6, 0, 5};
    int size1 = sizeof(nums1) / sizeof(nums1[0]);
    struct TreeNode* root1 = constructMaximumBinaryTree(nums1, 0, size1 - 1);
    task7(root1);

    int nums2[] = {3, 2, 1};
    int size2 = sizeof(nums2) / sizeof(nums2[0]);
    struct TreeNode* root2 = constructMaximumBinaryTree(nums2, 0, size2 - 1);
    task7(root2);
}

void test_task8() {
    char s1[] = "abc";
    int indices1[] = {0, 1, 2};
    int length1 = sizeof(indices1) / sizeof(indices1[0]);
    char* result1 = task8(s1, indices1, length1);
    assert(!strcmp(result1, "abc"));
    free(result1);

    char s2[] = "abap";
    int indices2[] = {0, 3, 2, 1};
    int length2 = sizeof(indices2) / sizeof(indices2[0]);
    char* result2 = task8(s2, indices2, length2);
    assert(!strcmp(result2, "apab"));
    free(result2);
}

void test_task9() {
    FILE *output = fopen("../output.txt", "r");

    task9(10);

    int x;
    if(fscanf(output, "%d", &x) != EOF) {
        if(x != 9) {
            printf("Error! Task 9");
        }
    }
}

int main() {
    test_task1();
    test_task2();
    test_task3();
    test_task4();
    test_task5();
    test_task6();
    test_task7();
    test_task8();
    test_task9();
    return 0;
}

