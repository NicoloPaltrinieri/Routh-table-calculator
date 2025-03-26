#include "routh.h"

int main(void) {
	cella c1 = { {0,0,1},{0,0,1} };
	cella c2 = { {0,2,0},{0,0,1} };
	cella c3 = { {0,3,3},{0,0,1} };
	cella c4 = { {0,4,0},{0,1,1} };
	cella c5 = { {5,0,5},{0,1,1} };

	cella v[] = { c1,c2,c3,c4 ,c5 };
	
	struct matrix* matrice = table_routh(v, 5);
	
	free(matrice);

	return 0;
}