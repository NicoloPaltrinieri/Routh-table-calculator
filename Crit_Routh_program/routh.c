#include "routh.h"
#include<float.h>

//--- funzioni semplici ---
bool is_zero_vettore(double* v) {
	for (int i = 0; i < l; i++) {
		if (v[i] != 0) {
			return false;
		}
	}

	return true;
}

bool is_zero_cella(cella c) {

	return is_zero_vettore(c.num);
}

void make_zero_vettore(double* v) {
	for (int i = 0; i < l; i++) {
		v[i] = 0;
	}

	return;
}

void make_zero_cella(cella* c) {
	make_zero_vettore(c->num);
	make_zero_vettore(c->den);

	return;
}

void copia_cella(cella* to, cella from) {
	for (int i = 0; i < l; i++) {
		(to->num)[i] = from.num[i];
		(to->den)[i] = from.den[i];
	}
}

void inverti_cella(cella* to, cella from) {
	for (int i = 0; i < l; i++) {
		(to->den)[i] = from.num[i];
		(to->num)[i] = from.den[i];
	}
}

int min_vettore(double* v, double* ris) {
	//GUIDA ALLA FUNZIONE :
	//ritorna				0: se ho trovato un minimo valido
	//					   -1: se non ho trovato un minimo valido					


	if (is_zero_vettore(v)) {
		return -1;
	}


	double min = DBL_MAX;
	for (int i = 0; i < l; i++) {
		if ((min > v[i]) && (v[i] != 0)) { //il minimo che non sia 0
			min = v[i];
		}
	}

	*ris = min;
	return 0;
}

double correct_output(double n) {
	if (n == 0) {
		return 0;
	}

	return n;
}

void trasla_vettore(double* v) {
	for (int i = l - 1; i > 0; i--) {
		v[i] = v[i - 1];
	}
	v[0] = 0;

	return;
}

void trasla_cella(cella* c) {
	trasla_vettore(c->num);
	trasla_vettore(c->den);

	return;
}


//--- funzioni di stampa ---
void printf_array(cella* v, int vsize) {
	//numeratori
	for (int i = 0; i < vsize; i++) {
		for (int j = 0; j < l; j++) {
			printf("[%.2lf] ", correct_output(v[i].num[j]));
		}
		printf("   ");//3 spazi
	}

	printf("\n");

	//denominatori
	for (int i = 0; i < vsize; i++) {
		for (int j = 0; j < l; j++) {
			printf("[%.2lf] ", correct_output(v[i].den[j]));
		}
		printf("   ");//3 spazi
	}
	printf("\n");

	return;
}

void printf_matrice(struct matrix* m) {
	printf("-------- MATRICE -------------------------------------------------------\n");

	for (int r = 0; r < m->rows; r++) {
		printf_array(&m->data[r * m->cols], m->cols);
		printf("\n\n");
	}
	printf("------------------------------------------------------------------------\n");

	printf("\n\n");

	return;
}


//--- funzioni complesse ---
void semplifica_cella(cella* c) { //ha senso semplificare un double ? poi ha senso farlo con il min?
	if (is_zero_vettore(c->num)) {
		make_zero_cella(c);
	}

	if (is_zero_vettore(c->den)) { 
		//semplifica cella è chiamata solo quando una cella è coinvolta in un calcolo. A fini matematici se il denominatore è 0 mattilo a 1 
		// (semplifica i calcoli e non fa scrivere casi particolari)
		c->den[l - 1] = 1;
	}
	
	bool check = true;
	for (int i = 0; i < l; i++) { //semplificazione: se hai (2k^2+k)/k --> (2k+1)/1
		if (!check) {
			break;
		}
		if (c->num[l - 1] == 0 && c->den[l - 1] == 0) {
			trasla_cella(c);
		}
		else {
			check = false;
		}
	}

	double min = 0; //semplifica per il valore minore del denominatore
	if (min_vettore(c->den, &min) == 0 && min != 1) {
		for (int i = 0; i < l; i++) {
			c->num[i] /= min;
			c->den[i] /= min;
		}
	}

	return;
}

void moltiplica_vettori(double* ritorno, double* v1, double* v2) {
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {

			double t = 0;

			if (i + j - l + 1 >= 0) {
				ritorno[i + j - l + 1] += v1[i] * v2[j];

			}
			else {
				t += v1[i] * v2[j];

			}

			if (t != 0) {
				printf("\nERRORE ERRORE -- OVERFLOW -- ERRORE ERRORE\n");
			}

		}
	}

	return;
}

void moltiplica_celle(cella* ritorno, cella c1, cella c2) {
	moltiplica_vettori(ritorno->num, c1.num, c2.num);
	moltiplica_vettori(ritorno->den, c1.den, c2.den);

	return;
}

void dividi_celle(cella* ritorno, cella dividendo, cella divisore) {
	cella tmp;
	make_zero_cella(&tmp);
	inverti_cella(&tmp, divisore);

	moltiplica_celle(ritorno, dividendo, tmp);

	return;
}

void sottrai_celle(cella* ritorno, cella minuendo, cella sottraendo) {
	/*
	1)	semplifica minuendo e sottraendo
	2)	mcm dei denominatori
	3)	calcolo dei numeratori
	4)	sottrazione dei numeratori
	5)	semplifico il risultato
	*/

	//1
	semplifica_cella(&minuendo);
	semplifica_cella(&sottraendo);

	//2
	double denominatore[l];
	make_zero_vettore(denominatore);
	moltiplica_vettori(denominatore, minuendo.den, sottraendo.den);

	//3
	double n1[l];
	make_zero_vettore(n1);
	moltiplica_vettori(n1, minuendo.num, sottraendo.den);
	double n2[l];
	make_zero_vettore(n2);
	moltiplica_vettori(n2, minuendo.den, sottraendo.num);

	//4
	for (int i = 0; i < l; i++) {
		ritorno->num[i] = n1[i] - n2[i];	
		ritorno->den[i] = denominatore[i];
	}

	//5
	semplifica_cella(ritorno);

	return;
}

void calcola_cella(struct matrix* m, int r, int c) {
	cella bs;
	copia_cella(&bs, m->data[(r - 1) * m->rows + 0]);
	if (is_zero_cella(bs)) {
		return;
	}

	cella bd;
	copia_cella(&bd, m->data[(r - 1) * m->rows + c + 1]);

	cella as;
	copia_cella(&as, m->data[(r - 2) * m->rows + 0]);

	cella ad;
	copia_cella(&ad, m->data[(r - 2) * m->rows + c + 1]);


	//---casi particolari---
	if (is_zero_cella(bd) && is_zero_cella(ad)) {
		make_zero_cella(&m->data[r * m->cols + c]);

		return;
	}

	if (is_zero_cella(bd)) {
		copia_cella(&m->data[r * m->cols + c], ad);

		return;
	}
	//-----------------------


	cella c1;	//alto destra * basso sinistra
	make_zero_cella(&c1);
	moltiplica_celle(&c1, bs, ad);

	cella c2;	//alto sinistra * basso destra
	make_zero_cella(&c2);
	moltiplica_celle(&c2, as, bd);

	cella t;
	make_zero_cella(&t);
	sottrai_celle(&t, c1, c2);

	dividi_celle(&m->data[r * m->cols + c], t, bs);

	semplifica_cella(&m->data[r * m->cols + c]);

	return;
}


//--- funzione principale ---
struct matrix* table_routh(cella* v, int vsize) {

	//gruppo di controllo
	printf("-------- VETTORE INIZIALE --------\n");
	printf_array(v, vsize);
	printf("----------------------------------\n\n");

	//creazione matrice di ritorno
	struct matrix* m = calloc(1, sizeof(struct matrix));
	m->rows = vsize;
	m->cols = vsize;
	int msize = vsize * vsize;
	m->data = realloc(m->data, msize * sizeof(cella));
	for (int i = 0; i < msize; i++) {
		make_zero_cella(&m->data[i]);
	}

	//metto le prime 2 righe
	for (int i = 0; i < vsize; i++) {
		if (i % 2 == 0) {
			copia_cella(&m->data[i / 2], v[i]);
		}
		else {
			copia_cella(&m->data[i / 2 + vsize], v[i]);
		}
	}
	//printf_matrice(m); //<-- superfluo

	//l'effettivo calcolo
	for (int r = 2; r < m->rows; r++) {
		for (int c = 0; c < m->cols; c++) {
			calcola_cella(m, r, c);

			//printf_matrice(m); //<-- superfluo

			if (is_zero_cella(m->data[r * m->cols + c]) == true) {//se cominci a scrivere 0 skippa il resto della riga
				c = m->cols;
			}
		}
	}

	printf_matrice(m);
	return m;
}

