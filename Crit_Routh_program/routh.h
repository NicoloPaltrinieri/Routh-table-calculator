#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define l 3

struct numero { //più sei a sinistra maggiore è l'esponente
    double num[l];
    double den[l];
}typedef cella;

struct matrix {
    int rows, cols;
    cella* data;
};

extern struct matrix* table_routh(cella* v, int vsize);