



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>


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

struct element{
	element* suiv;
	int x;
	int y;
	
};
typedef struct element element;

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

void signer(int ** graph,char * signature){
	//not implemented yet
	
	//graph[][]    signature[]
	
	
	
}



