#pragma once
int** getOrientedDegrees(int** matrix, int vertices);
int getOrientedRegularityDegree(int** degrees, int vertices);
int* getNonOrientedDegrees(int** matrix, int vertices);
int getNonOrientedRegularityDegree(int* degrees, int vertices);
int** getAccessibilityMatrix(int** matrix, int vertices);
int** multiplyMatrices(int** m1, int** m2, int vertices);
int** transponateMatrix(int** m, int vertices);
int** getStrongConnectionMatrix(int** matrix, int vertices);
int** getStrongConnectionComponents(int** strongConnectionMatrix, int vertices, int* componentsAmount);
int** getCondensedMatrix(int** strongConnectionComponents, int componentsAmount, int** accessMatrix, int vertices);
int** getPathesOfLength2(int** matrix, int vertices, int* pathesAmount, int chainsOnly);
int** getPathesOfLength3(int** matrix, int vertices, int* pathesAmount, int chainsOnly);
