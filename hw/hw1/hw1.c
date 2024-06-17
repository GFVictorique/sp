#include <stdio.h>
#include <stdlib.h>

// 动态分配记忆体大小
double *allocateMatrix(int rows, int cols) {
    return (double *)malloc(rows * cols * sizeof(double));
}

// 打印矩阵
void dump(double *mat, int rows, int cols){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%0.lf ", mat[i * cols + j]);
        }
        printf("\n");
    }
}

// 矩阵转置
double *transpose(double *mat, int rows, int cols){
    double *temp = allocateMatrix(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[j * rows + i] = mat[i * cols + j];
        }
    }
    return temp;
}

// 矩阵加法
double *add(double *matA, double *matB, int rows, int cols) {
    double *temp = allocateMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[i * cols + j] = matA[i * cols + j] + matB[i * cols + j];
        }
    }
    return temp;
}

// 矩阵乘法
double *multiply(double *matA, double *matB, int rowsA, int colsA, int colsB) {
    double *temp = allocateMatrix(rowsA, colsB);
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            temp[i * colsB + j] = 0;
            for (int k = 0; k < colsA; k++) {
                temp[i * colsB + j] += matA[i * colsA + k] * matB[k * colsB + j];
            }
        }
    }
    return temp;
}

int main(){
    int rowsA, colsA, rowsB, colsB;
    printf("输入矩阵A的行数和列数，矩阵B的行数和列数（例如：2 3 3 2）：\n");
    scanf("%d %d %d %d", &rowsA, &colsA, &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("矩阵A的列数必须等于矩阵B的行数才能进行乘法运算。\n");
        return 1;
    }

    double *matA = allocateMatrix(rowsA, colsA);
    double *matB = allocateMatrix(rowsB, colsB);

    printf("输入矩阵A的元素：\n");
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            scanf("%lf", &matA[i * colsA + j]);
        }
    }

    printf("输入矩阵B的元素：\n");
    for (int i = 0; i < rowsB; i++) {
        for (int j = 0; j < colsB; j++) {
            scanf("%lf", &matB[i * colsB + j]);
        }
    }

    printf("矩阵A的转置：\n");
    double *transposedA = transpose(matA, rowsA, colsA);
    dump(transposedA, colsA, rowsA);

    printf("矩阵B的转置：\n");
    double *transposedB = transpose(matB, rowsB, colsB);
    dump(transposedB, colsB, rowsB);

    if (rowsA == rowsB && colsA == colsB) {
        printf("矩阵A和B的和：\n");
        double *sum = add(matA, matB, rowsA, colsA);
        dump(sum, rowsA, colsA);
        free(sum);
    } else {
        printf("矩阵A和B的大小不匹配，无法相加。\n");
    }

    printf("矩阵A和B的积：\n");
    double *product = multiply(matA, matB, rowsA, colsA, colsB);
    dump(product, rowsA, colsB);

    // 释放内存
    free(matA);
    free(matB);
    free(transposedA);
    free(transposedB);
    free(product);

    return 0;
}
