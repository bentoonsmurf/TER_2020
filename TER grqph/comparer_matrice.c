
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define N 30// le nombre  de sommet du graph 
#define taille_paquet 1000
#define signature_taille 1000


typedef struct pool{
	int p[N];
	int size;
	
}pool;


typedef struct element{
	struct element* precedent;
	int x;
	int ordre;
	int y;
	
}element;
typedef struct pile pile;
struct pile{
	element* fond;
	element* sommet;
	
};


struct signature{
	int * sig;
	int nb_arc;
	
};
typedef struct signature signature;



int graph_vide[N+1][N+1];
int paquet_de_graph[taille_paquet][N+1][N+1];
int graph_primaire[N+1][N+1];
signature signature_primaire;
element fond_de_pile;



void init_random(){
	
  srandom(getpid()+ time(NULL));

}


void afficher_graph_vide(){
	printf("\n");
		for (int i=N;i>=0;i--){
	for(int j=0;j<N+1;j++){
		printf("%d|",graph_vide[i][j]);
		}printf("\n");
}
}
void afficher_graph_du_paquet(int w){
	printf("\n");
		for (int i=N;i>=0;i--){
	for(int j=0;j<N+1;j++){
		printf("%d|",paquet_de_graph[w][i][j]);
		}printf("\n");
}
}
void afficher_pile(pile* p){
	if(p != NULL){
	printf("affiche pile \n");
	    element *actuel = p->sommet;
		//while(p.sommet != &fond_de_pile){
		    while (actuel != NULL)
			{
				printf("element (%d,%d,%d)\n",actuel->x,actuel->ordre,actuel->y);
				//printf("%d -> ", actuel->nombre);
				actuel = actuel->precedent;
			}
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

					//si il manque un arc par raport au degré d'un sommet ---> return le sommet
int missing_no(){ 	//si le graph est conforme return, 0
	int objectif, somme;
	for (int i=1;i<N+1;i++){
		objectif=( graph_vide[i][0]*  (graph_vide[i][0] +1) )/2;
		somme=0;
		for(int j=1;j<N+1;j++){
			if(graph_vide[i][j]!=0) somme=somme+graph_vide[i][j];
			}
		if(somme != objectif ) return i;
}
return 0;	
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
	a = alea(arrive.size);// 0 n'existe pas ici 

	
	depart.p[0]= a;	depart.size= 1; /// on ajoute le premier element dans la pool des depart
	
	int r;
/// etape 2 vider la pool

//	printf("point de depart %d\n",a);

	while(arrive.size > 1){ /// inscrit 2 arc dans la matrice

		r=alea(depart.size)-1 ; 
		d=depart.p[ r];

		r=alea(arrive.size)-1 ;
		a=arrive.p[ r]; 


		graph_vide[d][0]++;/// j'augmente le degre du sommet
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
		
		d=a; /// a etant le dernier sommet de la boucle
		a=arrive.p[0]; 
		graph_vide[d][0]++;
		graph_vide[0][d]++;
		graph_vide[a][0]++;
		graph_vide[0][a]++;
		graph_vide[d][a] =graph_vide[d][0];
		graph_vide[a][d] =graph_vide[a][0];
		
	if(missing_no() !=0){
		generer_arbre();
	}/// statisiquement pas rare 
}

void generer_3_connexe(){
	if ((N*3) %2 == 0){
	 generer_cycle_simple();// erreur missing_no impossible
	
	
	pool p;
	for(int i=0; i<N; i++){/// on met chaque sommet une fois dans la pool
		p.p[i]=i+1;
	}
	p.size=N;
	
	int d,a;
	int r;
	int anti_boucle_infini = 25;
/// etape 2 vider la pool

	while(p.size > 1){ /// inscrit 2 arc dans la matrice
		r=alea(p.size)-1 ;
		d=p.p[ r];
		p.p[r]= p.p[p.size-1];
		p.size--;
			


		
		r=alea(p.size)-1 ;//alea ne renvoie pas 0
		a=p.p[ r]; // un element dans la pool donc pas 0
		p.p[r]= p.p[p.size-1];
		p.size--;
			
			
		if (graph_vide[d][a] ==0){
			
			graph_vide[d][0]++;/// d comme depart
			graph_vide[0][d]++;
			graph_vide[a][0]++;
			graph_vide[0][a]++;
			graph_vide[d][a] =graph_vide[d][0];
			graph_vide[a][d] =graph_vide[a][0];
			//printf("arc [%d;%d] = %d\n",d,a,graph_vide[d][a]);
		
		}
		else{
			if(anti_boucle_infini> 0){
			//	printf("ALREADY EXIST arc [%d;%d] = %d\n",d,a,graph_vide[d][a]);
			
				p.size++;
				p.p[p.size-1]=d;
				p.size++;
				p.p[p.size-1]=a;
				anti_boucle_infini--;
			}
			if(anti_boucle_infini <= 0){// making sure missing_no catches it
						
				graph_vide[d][0]++;
				graph_vide[0][d]++;
				graph_vide[a][0]++;
				graph_vide[0][a]++;
				graph_vide[d][a] =graph_vide[d][0];
				graph_vide[a][d] =graph_vide[a][0];
			//	printf("ERROR arc [%d;%d] = %d\n",d,a,graph_vide[d][a]);
			
			
			}
		
		}
			

		
	}
	if(missing_no() !=0){
		generer_3_connexe();
	}
	
	}
}

// swap deux poids
void modifier_un_tout_petit_peut_le_graph(){
	/// rappel : graph_vide[r][0] = degre de R 
	int r=0;
	while(graph_vide[r][0]<= 1	){
		r=alea(N) ;
	}
	/// j'ai r un sommet de degré 2 ou plus
	/// je vais swap deux valeures d'arc , c'est tout
	int a=alea(N) ;
	int b=alea(N) ;
	
	while(graph_vide[r][a] == 0	){
		 a=alea(N) ;
	}
	while(graph_vide[r][b] == 0	|| b==a){
		 b=alea(N) ;
	}
	int tmp = graph_vide[r][a];
	
	graph_vide[r][a] = graph_vide[r][b];
	graph_vide[r][b] = tmp;
	//graph_vide[r][0] = 0; // pour trouver la ligne 
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

/// remplis le paquet de graph avec des 3connexe
void remplis_paquet_avec_3_connexe(){

	for (int w=0;w<taille_paquet;w++){
		generer_3_connexe();
		
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				paquet_de_graph[w][i][j]  = graph_vide[i][j];
			}
		}
	}

}
///////////////////////////////////////////// partie signature ////////////////////////////////////////////////

signature* init_signature(){
	signature* s=malloc(sizeof(signature*));
	s->sig =malloc(signature_taille*sizeof(int));
	s->nb_arc=0; // marche pas ? c pas du java ?
	return s;  // du coup il faut return
}
//operationel
pile empiler_deprecated(pile p,element* e){// p.sommet <---- &e c'est ca que je veux
	//e.precedent=p.sommet;
	
	element* tmp=p.sommet;
	p.sommet =e;
	p.sommet->precedent=tmp;
	return p;
}

void empiler(pile* p, int x,int ordre,int y)
{
    element *nouveau = malloc(sizeof(*nouveau));
    if (p == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->x = x;
    nouveau->ordre = ordre;
    nouveau->y = y;
    nouveau->precedent = p->sommet;
    p->sommet = nouveau;
}
void init_pile_deprecated(pile* p){
	fond_de_pile.x=0;
	fond_de_pile.y=0;
	fond_de_pile.ordre=0;
	fond_de_pile.precedent=&fond_de_pile;
	

	empiler(p,0,0,0);
	p->sommet=&fond_de_pile;
	p->fond=&fond_de_pile;
	//return p;
}
pile *init_pile()
{
    pile * p = malloc(sizeof(pile*));
    element* e= malloc(sizeof(element*));

    if (p == NULL || e == NULL)
    {
        exit(EXIT_FAILURE);
    }
	e->x = 0;
    e->ordre = 0;
	e->y = 0;
    e->precedent = NULL;
    p->sommet = e;
	p->fond = e;
    return p;
}


element depiler_deprecated(pile p){
	element e=*p.sommet;
	p.sommet= p.sommet->precedent;
	return e;
}

element depiler(pile* p)
{
    if (p == NULL)
    {	
		//printf("MMMMMMMMMMMMMMMMMMMMMMMM");
        exit(EXIT_FAILURE);
    }

    element valueDepile ;
    element* elementDepile = p->sommet;

    if (p != NULL && p->sommet != NULL)
    {
        valueDepile.x = elementDepile->x;
         valueDepile.ordre = elementDepile->ordre;
          valueDepile.y = elementDepile->y;
         p->sommet = elementDepile->precedent;
        free(elementDepile);
    }

    return valueDepile;
}


// operationel
///met toutes les aretes sortantes du sommet sur la pile
/// arrete de valeure 1 = sommet de la pile  
///return degre du sommet
void metre_arcs_dans_pile(int sommet,pile* p,int matrice_nb){
	if (sommet != 0){
	int degre_sommet = paquet_de_graph[matrice_nb][sommet][0];
    int* arcs =malloc((degre_sommet+1)*sizeof(int));// presque toutes les info sont implicites 
    
    /// je lis toute la ligne de la matrice 
    /// pour chaque cases : arc[poid] = i
    /// arc[0] va souvent etre reecrit mais on s'en fiche
	for (int i=0;i<N+1;i++){
		arcs[ paquet_de_graph[matrice_nb][sommet][i] ]= i;
		// si  matrice[sommet][i] = 0 on remplis arc[0]
		//sinon on remplis une case significative de arc[]
	}
	arcs[0]=degre_sommet;
	
	printf("degre du sommet %d = %d\n",sommet,degre_sommet);
	/// on empile en partant du plus grand 
	/// comme ca l'arc de poid faible  est  au dessu
	for (int j=degre_sommet;j>=1;j--){
		empiler(p,sommet,j,arcs[j]);

	}
	
	
	free(arcs);
	printf("fin de metre dans pile \n");
}
}


//operationel
void ecrire_signature(signature* sig, int x,int ordre, int y){
	sig->sig[sig->nb_arc] = x;
	
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc]<<10;
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc] + ordre;
	
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc]<<10;
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc] + y;
	
	sig->nb_arc++;//  =	sig->nb_arc + 1;
	//return sig;
}

void signer(int graph_nb, signature* s,int depart){
	int compteur_sommet=2;
	int new_name[N+1][2];
	for (int i=0;i<N+1;i++){  new_name[i][0]=0;new_name[i][1]=0;} // tableau remplis de 0
	new_name[depart][0]=1;
	// tab[i][0] contien le nouveaux nom/adress du sommet i
	// tab[i][1] contien le degre 
	element arc_courant;
	pile* p=init_pile();
	new_name[depart][1]=paquet_de_graph[graph_nb][depart][0];
	//printf("degre de new name = %d au debut de la fonction\n",new_name[depart][1]);//degre ok
			
	metre_arcs_dans_pile(depart,p,graph_nb);/// pile remplis par des arcs partant du point de depart

//afficher_pile(p);// pile non remplis

	while(p->sommet != p->fond){
		arc_courant = depiler(p);

		if(new_name[arc_courant.y][0]==0){// sommet arrive non declarer pour le moment
			
			new_name[arc_courant.y][1]=paquet_de_graph[graph_nb][arc_courant.y][0];// degre
			metre_arcs_dans_pile(arc_courant.y,p,graph_nb);/// on empile tous les arc partants du nouveau sommet
			new_name[arc_courant.y][0]=compteur_sommet;
			compteur_sommet++;
		}
		
		ecrire_signature(s,new_name[ arc_courant.x][0], arc_courant.ordre,  new_name[ arc_courant.y][0]); // ecrire avec new name pas encore fait
		printf("signature a %d arcs a la fin de lercriture\n",s->nb_arc);
		afficher_pile(p);
		
	}

	
			printf("signature a %d arcs a la fin de la signature\n",s->nb_arc);
	//return s;
}

void afficher_signature(signature* sig){
	int a,b,c;
		printf("signature a %d arcs \n",sig->nb_arc);
	for (int i=0;i<sig->nb_arc ;i++){
		a=sig->sig[i]>>20;
		b= (sig->sig[i]-((sig->sig[i]>>20)<<20))>>10;
		c=(sig->sig[i]) -(a<<20) - (b<<10) ;
		printf("(%d,%d,%d)",a,b,c);
	}
	
	printf("\n");
}


int main(){
	/// inits 
init_random();
signature* s=init_signature();
pile* p=init_pile();



clock_t temps_initial; /* Temps initial en micro-secondes */
clock_t temps_final; /* Temps final en micro-secondes */
float temps_cpu; /* Temps total en secondes */
temps_initial = clock ();


remplis_paquet_avec_3_connexe();



afficher_graph_du_paquet(1);
signer(1,s , 1);
afficher_signature(s);

temps_final = clock ();
temps_cpu = (temps_final - temps_initial) * 1e-6;
printf(" temps construction des graphs = %Lf sec\n",(long double)temps_cpu);

//printf("%Lf", (long double)(temps_final - temps_initial));

return 0;


}
