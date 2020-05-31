

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
/// 1 int =  2 bits vides +10 x 3bit qui on du sens [somme_1  , arc ,  sommet_2]

typedef struct element{
	struct element* precedent;
	int x;
	int ordre;
	int y;
	
}element;

struct pile{
	element* fond;
	element* sommet;
	
};
typedef struct pile pile;

struct signature{
	int * sig;
	int nb_arc;
	
};
typedef struct signature signature;


/// pour l'exploration de graph j'utilise la matrice de base
/// pour ecrire la signature j'utilise la forme modifié 
/// le nom des sommets change juste pour l'ecriture de la signature



int etalon[N+1][N+1]; // la case 0 contien le degré du sommet 
int potentielle_isomorphe[N+1][N+1];
int signature_etalon[signature_taille];
int signature_isomorph[signature_taille];
element fond_de_pile;


signature init_signature(signature s){
	s.sig =malloc(signature_taille*sizeof(int));
	s.nb_arc=0; // marche pas ? c pas du java 
	return s;  // du coup il faut return
}

pile empiler(pile p,element* e){// p.sommet =&e c'est ca que je veux
	//e.precedent=p.sommet;
	
	element* tmp=p.sommet;
	p.sommet =e;
	p.sommet->precedent=tmp;
	return p;
}
pile init_pile(pile p){
	fond_de_pile.x=0;
	fond_de_pile.y=0;
	fond_de_pile.ordre=0;
	fond_de_pile.precedent=&fond_de_pile;
	
	p.sommet=&fond_de_pile;
	p.fond=&fond_de_pile;
	empiler(p,&fond_de_pile);
	return p;
}

element depiler(pile p){
	element e=*p.sommet;
	p.sommet= p.sommet->precedent;
	return e;
}

// operationel
///met toutes les aretes sortantes du sommet sur la pile
/// arrete de valeure 1 = sommet de la pile  
///return degre du sommet
int metre_arcs_dans_pile(int sommet,pile p,int** matrice){
	int degre_sommet = matrice[sommet][0];
    int* arcs =malloc((degre_sommet+1)*sizeof(int));// presque toutes les info sont implicites 
    
    /// je lis toute la ligne de la matrice 
    /// pour chaque cases : arc[poid] = i
    /// arc[0] va souvent etre reecrit mais on s'en fiche
	for (int i=1;i<N+1;i++){
		arcs[ matrice[sommet][i] ]= i;
		// si  matrice[sommet][i] = 0 on remplis arc[0]
		//sinon on remplis une case significative de arc[]
	}
	arcs[0]=degre_sommet;
	/// on empile en partant du plus grand 
	/// comme ca l'arc de poid faible  est  au dessu
	for (int j=degre_sommet;j>1;j--){
		element e;
		e.precedent=p.sommet;
		e.x=sommet;
		e.ordre=j;//<------------------------------------
		e.y=arcs[j];
		empiler(p,&e);
	}
	
	
	free(arcs);
	return degre_sommet;
}


//operationel
signature ecrire_signature(signature sig, int x,int ordre, int y){
	sig.sig[sig.nb_arc] = x;
	
	sig.sig[sig.nb_arc] = sig.sig[sig.nb_arc]<<10;
	sig.sig[sig.nb_arc] = sig.sig[sig.nb_arc] + ordre;
	
	sig.sig[sig.nb_arc] = sig.sig[sig.nb_arc]<<10;
	sig.sig[sig.nb_arc] = sig.sig[sig.nb_arc] + y;
	
	sig.nb_arc=	sig.nb_arc + 1;
	return sig;
}

signature signer(int ** graph, signature s,int depart){
	int compteur_sommet=2;
	int new_name[N+1][2];
	for (int i=0;i<N+1;i++){  new_name[i][0]=0;new_name[i][1]=0;} // tableau remplis de 0
	new_name[depart][0]=1;
	// tab[i][0] contien le nouveaux nom/adress du sommet i
	// tab[i][1] contien le degre 
	
	pile p=	init_pile(p);
	new_name[depart][1]=metre_arcs_dans_pile(depart,p,graph);// pile remplis par des arcs partant du point de depart
	element arc_courant;
	
	while(p.sommet != &fond_de_pile){
		arc_courant=depiler(p);

		if(new_name[arc_courant.y][0]==0){// sommet arrive non declarer pour le moment
			new_name[arc_courant.y][1]=metre_arcs_dans_pile(arc_courant.y,p,graph);/// on empile tous les arc partants du nouveau sommet
			new_name[arc_courant.y][0]=compteur_sommet;
			compteur_sommet++;
		}
		
		s=ecrire_signature(s,new_name[ arc_courant.x][0], arc_courant.ordre,  new_name[ arc_courant.y][0]); // ecrire avec new name pas encore fait
		
		
	}

	
	
	return s;
}


int signature_test(signature s){
	return  s.nb_arc + 1;
}

int main(){
	signature s=init_signature(s);
	element e;
	e.x=1; e.y = 2; e.ordre=1;  e.precedent=NULL;
	
	s=ecrire_signature(s, e.x,e.ordre,e.y);
	printf("\n signature size  = %d\n",s.nb_arc); //ok
	printf("int = %d",s.sig[0]);
	
	
	pile p=	init_pile(p); //p.sommet=&e;
	
	if(p.sommet == &fond_de_pile){printf("\nFOND DE PILE ICI\n"); }
	printf("\navant EMPILAGE p.sommet.y = %d  \np.sommet.ordre= %d\n",p.sommet->y,p.sommet->ordre);
	p=empiler(p,&e);
	
	printf("\n APRES p.sommet.y = %d  \np.sommet.ordre= %d\n",p.sommet->y,p.sommet->ordre);
	
	printf("\n avant test %d",s.nb_arc);
	s.nb_arc= signature_test(s);

	
	printf("\n apres test %d \n",s.nb_arc);
	return 0;
}



