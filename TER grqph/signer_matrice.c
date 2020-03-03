

/// suppositions :
/*
le graph est de cette forme 
graph[0][j] est le degre du sommet j
graph[i][0] est le degre du sommet i
il y a repetition d'information sur le sommet A:  [0][A] et [A][0]
si graph[i][j] = 0 il n'y a pas d'arretes entre i et j
sinon !=0 alors graph[i][j] est l'ordre dans le quel l'arete doit etre traité 

ce qui est la particularité des cartes : il y a un ordre dans le quel lire les arretes


*/




#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define N 30// le nombre  de sommet du graph 
#define signature_taille 1000

typedef struct element{
	struct element* precedent;
	int x;
	int y;
	
}element;

struct pile{
	element* fond;
	element* sommet;
	
};
typedef struct pile pile;

/// pour l'exploration de graph j'utilise la matrice de base
/// pour ecrire la signature j'utilise la forme modifié 
/// le nom des sommets change juste pour l'ecriture de la signature
int tab[5][N+1];// tab[0][i] contien le nouveaux nom/adress du sommet i
				// 1 contien le degre 

int etalon[N+1][N+1]; // la case 0 contien le degré du sommet 
int potentielle_isomorphe[N+1][N+1];
char signature_etalon[signature_taille];
char signature_isomorph[signature_taille];


void init_pile(pile p){
	p.sommet=NULL;
	p.fond=NULL;
}

void empiler(pile p,element e){
	p.sommet =&e;
	e.precedent=p.sommet;
}


element depiler(pile p){
	element e=*p.sommet;
	p.sommet= p.sommet->precedent;
	return e;
}

///met toutes les aretes sortantes du sommet sur la pile
/// l'arrete de valeure 1 est donc au sommet de la pile
void remplir_la_pile(int sommet,pile p){
	int degre_sommet = potentielle_isomorphe[sommet][0];
	
    int* arcs =malloc(degre_sommet*sizeof(int));// presque toutes les info sont implicites 
    
	for (int i=1;i<N;i++){
		// point de depart = sommet ----> connu 
		// arrive =i  				----> reseigné dans arcs[ordre]
		// poid = tab[sommet][i]	----> indice de arcs
		arcs[ potentielle_isomorphe[sommet][i] ]= i;
	}
	arcs[0]=degre_sommet;
	/// on empile en partant du plus grand 
	/// comme ca l'arc de poid est  au dessu
	for (int j=degre_sommet;j>1;j--){
		element e;
		e.x=sommet;
		e.y=arcs[j];
		e.precedent=NULL;
		empiler(p,e);
	}
	
	
	free(arcs);
}

void signer(int ** graph,char * signature,int depart){
	pile p;
	init_pile(p);
	remplir_la_pile(depart,p);// pile remplis par des arcs partant du point de depart
	
	while(p.sommet != NULL){
		
	}
	
	//not implemented yet
	
	//graph[][]    signature[]
	
	
	
}



