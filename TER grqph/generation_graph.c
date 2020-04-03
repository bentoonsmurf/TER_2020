

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>


#define N 30// le nombre  de sommet du graph 
int  graph_vide[N+1][N+1];

typedef struct pool{
	int p[N];
	int size;
	
}pool;



void init(){
	
  srandom(getpid()+ time(NULL));

}

void afficher(){
	printf("\n");
		for (int i=N;i>=0;i--){
	for(int j=0;j<N+1;j++){
		printf("%d|",graph_vide[i][j]);
		}printf("\n");
}
}
/// renvoie un nombre entre [1 : N]
int alea(int n){
	if (n==0)return 0;// en theorie ca sert a rien mais on est jamais trop prudent
	if (n==1)return 1;
	return (int)(random()% (n-1) +1);// return un sommet 

}
void vider_graph_vide(){
	for (int i=0;i<N+1;i++){
	for(int j=0;j<N+1;j++){
		graph_vide[i][j]=0;
		}
}
}

/// pour generer un arbre et pas une chaine il faut une 2 em pool
/// qui contiendra les sommets de depart
void generer_cycle_simple(){

//rappel//   le sommet 0 n'existe pas 

///etape 1: point de depart 
	vider_graph_vide();
	pool p;
	for(int i=0; i<N; i++){/// on met chaque sommet une fois dans la pool
		p.p[i]=i+1;
	}
	p.size=N;
	
	int d,a,first;
	first = alea(p.size);// 0 existe pas ici 
	a     = first;
	p.p[a-1]= p.p[p.size-1];	p.size--; /// on sort le premier sommet de la pool
	int r;
/// etape 2 vider la pool

	while(p.size > 1){ /// inscrit 2 arc dans la matrice
		d=a;
		r=alea(p.size)-1 ;//alea ne renvoie pas 0
		a=p.p[ r]; // un element dans la pool donc pas 0

		graph_vide[d][0]++;/// d comme depart
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a] =graph_vide[d][0];
		graph_vide[a][d] =graph_vide[a][0];


		// je met le dernier elemnt de la pool a la place de celui qui je vien d'utiliser
		// et je reduit la taille de la pool
		p.p[r]= p.p[p.size-1];
		p.size--;/// p.size = n ----> il y a exactement n sommet dans la pool 
				///  le dernier est donc a la place [N-1]

		
	}
		/// le dernier sommet etait oublié
		d=a;
		a=p.p[ 0]; 
		graph_vide[d][0]++;
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a] =graph_vide[d][0];
		graph_vide[a][d] =graph_vide[a][0];


		
		
		/// relier le dernier sommet et le premier 
		d=a;
		a=first; 
		graph_vide[d][0]++;
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a ]= graph_vide[d][0];
		graph_vide[a][d ]= graph_vide[a][0];
	
}



void generer_arbre(){
	// attention [d][a] != [a][d] c'est parfaitement normal
	// ex "d" est de degre 5 , [d][a] =3  est le 3 arc a traiter de "d"
	// mais "a" est de degre 1 [a][d] =1 est le premier arc a traiter de "a"
	
	vider_graph_vide();
	
	pool depart,arrive;
	for(int i=0; i<N; i++){/// on met chaque sommet une fois dans la pool
		arrive.p[i]=i+1;
		depart.p[i]=0;
	}
	arrive.size=N;
	depart.size=0;
	
	int d,a;
	a = alea(arrive.size);// 0 existe pas ici 

	
	depart.p[0]= a;	depart.size= 1; /// on ajoute le premier element dans la pool des depart
	
	int r;
/// etape 2 vider la pool

	while(arrive.size > 1){ /// inscrit 2 arc dans la matrice

		r=alea(depart.size)-1 ; 
		d=depart.p[ r];

		r=alea(arrive.size)-1 ;
		a=arrive.p[ r]; 

		graph_vide[d][0]++;/// d comme depart
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a] =graph_vide[d][0];
		graph_vide[a][d] =graph_vide[a][0];
			
		/// je met dans les depart potentiel le sommet que je vire des arrivé 
		/// ce qui a pour consequance de favoriser une structure en etoile
		depart.p[depart.size-1]= a;
		depart.size++;
		// je met le dernier elemnt de la pool a la place de celui qui je vien d'utiliser
		// et je reduit la taille de la pool
		arrive.p[r]= arrive.p[arrive.size-1];
		arrive.size--;
		

		
	}
		/// le dernier sommet etait oublié
		//r=alea(depart.size)-1 ;
		//d=depart.p[ r];
		
		d=a;
		a=arrive.p[0]; 
		graph_vide[d][0]++;
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a] =graph_vide[d][0];
		graph_vide[a][d] =graph_vide[a][0];

	
}



void generer_graph(int mode){
	///mode 1 =graph sans contrainte
	///mode 2 =graph 3complet
	// autre fonction
	///mode 3 =graph comprésé 
	///mode 4 =graph comprese 3complet
	


	
	
	
	if (mode == 1){
		generer_cycle_simple();///un cycle pour le moment
		
		int nb_arc_a_ajouter;
		int d,a;
		/// j'ajoute des arcs j'usque a tomber sur 1 qui existe deja
		while(nb_arc_a_ajouter < 10){
			d=alea(N);
			a=alea(N);
			while(graph_vide[d][a] == 0 ){ 
				
				graph_vide[d][0]++;
				graph_vide[0][d]++;
				graph_vide[a][0]++;
				graph_vide[0][a]++;
				graph_vide[d][a]=graph_vide[d][0]; /// si le degre est 4 alors ce nouveau sommet
				graph_vide[a][d]=graph_vide[a][0]; /// est forcement le 4iem sommet dans l'ordre des priorité
				nb_arc_a_ajouter++;
				
				d=alea(N);
				a=alea(N);
				
				
			}
			
		}
	}
	
	
}



void sample(){ // pour tester le sens dans le quel j'affiche
graph_vide[N][N]=9;// n'a d'interet qui si j'affiche dans le terminal
graph_vide[N][1]=1;
graph_vide[N][2]=2;
graph_vide[N][3]=3;
graph_vide[1][N]=11;
graph_vide[2][N]=21;
graph_vide[3][N]=31;
graph_vide[0][0]=-1;
graph_vide[1][0]=-2;
graph_vide[2][0]=-3;
}



int main(){
 init();

//generer_cycle_simple(); //ok



//generer_graph(1); // ok
generer_arbre(); // pas ok 


afficher();

return 0;


}
