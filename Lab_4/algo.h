#pragma once
int** or_degrees(int** matrix, int vertices);
int or_reg_degrees(int** degrees, int vertices);
int* nor_degrees(int** matrix, int vertices);
int nor_reg_degree(int* degrees, int vertices);
int** access_M(int** matrix, int vertices);
int** multiply(int** m1, int** m2, int vertices);
int** transpone(int** m, int vertices);
int** strong_connected_M(int** matrix, int vertices);
int** strong_components(int** strongConnectionMatrix, int vertices, int* componentsAmount);
int** condensed_M(int** strongConnectionComponents, int componentsAmount, int** accessMatrix, int vertices);
int** pathes_2(int** matrix, int vertices, int* pathesAmount);
int** pathes_3(int** matrix, int vertices, int* pathesAmount);
