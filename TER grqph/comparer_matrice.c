
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
//N le nombre  de sommet du graph 
#define N 30 // 30 = presque impossible de trouver des isomorphs , 20 = tres tres rare , 10 = frequent
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
signature* signature_primaire;



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
void afficher_graph_primaire(){
printf("\ngraph primaire----------------------------------------------------\n");
		for (int i=N;i>=0;i--){
	for(int j=0;j<N+1;j++){
		printf("%d|",graph_primaire[i][j]);
		}printf("\n");
}
printf("-------------------------------------------------------------------\n");
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
	for(int brute_force =1;brute_force <N+1;brute_force++){
		
		 
		 if((graph_vide[r][a] != graph_vide[r][brute_force]) && (graph_vide[r][brute_force] != 0)){
			 	int tmp = graph_vide[r][a];
				graph_vide[r][a] = graph_vide[r][brute_force];
				graph_vide[r][brute_force] = tmp;
				brute_force= N+2;
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
	a = first;
	d = first;
	p.p[a-1]= p.p[p.size-1];	p.size--; /// on sort le premier sommet de la pool
	int r;
/// etape 2 vider la pool

	while(p.size > 1){ /// inscrit 2 arc dans la matrice
		
		// d et a = 1 et 2
		d=a;
		r=alea(p.size)-1 ;//alea ne renvoie pas 0
		a=p.p[ r]; // un element dans la pool donc pas 0
	// d et a = 2 et 3
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
	
	
	
		for(int j=0;j< N/2 ;j++){
			//for(int j=0;j< 2 ;j++){
			modifier_un_tout_petit_peut_le_graph();
		}

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
void remplis_paquet_avec_cycle(){

	for (int w=0;w<taille_paquet;w++){
		generer_cycle_simple();
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				paquet_de_graph[w][i][j]  = graph_vide[i][j];
			}
		}
	}

}
void remplis_paquet_avec_arbre(){

	for (int w=0;w<taille_paquet;w++){
		generer_arbre();
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				paquet_de_graph[w][i][j]  = graph_vide[i][j];
			}
		}
	}

}
void copier_graph_vide_dans_paquet(int w){
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				paquet_de_graph[w][i][j]  = graph_vide[i][j];
			}
		}
	
}
void copier_paquet_dans_graph_vide(int w){
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				graph_vide[i][j]= paquet_de_graph[w][i][j]  ;
			}
		}
	
}
void copier_graph_vide_dans_primaire(){
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				graph_primaire[i][j]  = graph_vide[i][j];
			}
		}
}
void copier_primaire_dans_graph_vide(){
	
		for (int i=0;i<N+1;i++){
			for(int j=0;j<N+1;j++){
				graph_vide[i][j]  = graph_primaire[i][j];
			}
		}
}

///////////////////////////////////////////// partie signature ////////////////////////////////////////////////
///////////////////////////////////////////// ///////////////////////////////////////////// ///////////////////
signature* init_signature(){
	signature* s=malloc(sizeof(signature*));
	s->sig =malloc(signature_taille*sizeof(int));
	s->nb_arc=0; 
	return s;  
}
void vider_signature(signature* s){
	for (int i=0;i< s->nb_arc ; i++){
		s->sig[i]=0;
		
	}
	s->nb_arc=0;
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
	
	//printf("degre du sommet %d = %d\n",sommet,degre_sommet);
	/// on empile en partant du plus grand 
	/// comme ca l'arc de poid faible  est  au dessu
	for (int j=degre_sommet;j>=1;j--){
		empiler(p,sommet,j,arcs[j]);

	}
	
	
	free(arcs);
}
}


//operationel
void ecrire_signature(signature* sig, int x,int ordre, int y){
	sig->sig[sig->nb_arc] = x;
	
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc]<<10;
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc] + ordre;
	
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc]<<10;
	sig->sig[sig->nb_arc] = sig->sig[sig->nb_arc] + y;
	
	sig->nb_arc++;
}

/// return 0 si les signatures sont differentes 
int comparer_2_signatures(signature* s1,signature* s2){
	if (s1->nb_arc != s2->nb_arc) return 0;
	
	for(int i=0;i<s1->nb_arc;i++){
		if (s1->sig[i] != s2->sig[i]) return 0;
		
	}
	return 1;
}


void signer(int graph_nb, signature* s,int depart){
	vider_signature(s);
	int compteur_sommet=2;
	int new_name[N+1][2];
	for (int i=0;i<N+1;i++){  new_name[i][0]=0;new_name[i][1]=0;} // tableau remplis de 0
	new_name[depart][0]=1;
	// tab[i][0] contien le nouveaux nom/adress du sommet i
	// tab[i][1] contien le degre 
	element arc_courant;
	pile* p=init_pile();
	new_name[depart][1]=paquet_de_graph[graph_nb][depart][0];
			
	metre_arcs_dans_pile(depart,p,graph_nb);/// pile remplis par des arcs partant du point de depart

	while(p->sommet != p->fond){
		arc_courant = depiler(p);

		if(new_name[arc_courant.y][0]==0){// sommet arrive non declarer pour le moment
			
			new_name[arc_courant.y][1]=paquet_de_graph[graph_nb][arc_courant.y][0];// degre
			metre_arcs_dans_pile(arc_courant.y,p,graph_nb);/// on empile tous les arc partants du nouveau sommet
			new_name[arc_courant.y][0]=compteur_sommet;
			compteur_sommet++;
		}
		
		ecrire_signature(s,new_name[ arc_courant.x][0], arc_courant.ordre,  new_name[ arc_courant.y][0]);
		
	}
}




int signer_intelligemment(int graph_nb, signature* s,int depart){
	vider_signature(s);
	int compteur_sommet=2;
	int new_name[N+1][2];
	for (int i=0;i<N+1;i++){  new_name[i][0]=0;new_name[i][1]=0;} // tableau remplis de 0
	new_name[depart][0]=1;
	// tab[i][0] contien le nouveaux nom/adress du sommet i
	// tab[i][1] contien le degre 
	element arc_courant;
	pile* p=init_pile();
	new_name[depart][1]=paquet_de_graph[graph_nb][depart][0];
			
	metre_arcs_dans_pile(depart,p,graph_nb);/// pile remplis par des arcs partant du point de depart

	while(p->sommet != p->fond){
		arc_courant = depiler(p);

		if(new_name[arc_courant.y][0]==0){// sommet arrive non declarer pour le moment
			
			new_name[arc_courant.y][1]=paquet_de_graph[graph_nb][arc_courant.y][0];// degre
			metre_arcs_dans_pile(arc_courant.y,p,graph_nb);/// on empile tous les arc partants du nouveau sommet
			new_name[arc_courant.y][0]=compteur_sommet;
			compteur_sommet++;
		}
		
		ecrire_signature(s,new_name[ arc_courant.x][0], arc_courant.ordre,  new_name[ arc_courant.y][0]);
		
		if(s->sig[s->nb_arc-1] ==  signature_primaire->sig[s->nb_arc-1] ){
			// do nothing
		}else{
			return 0;
		}
		
	}

	return 1;
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

// le resultat est convaincant 
void litle_cup_test(signature* s){
	vider_graph_vide();
		graph_vide[0][0] = 0;
		graph_vide[0][1] = 3;
		graph_vide[0][2] = 1;
		graph_vide[0][3] = 1;
		graph_vide[0][4] = 1;
	
		graph_vide[1][0] = 3;
		graph_vide[1][1] = 0;
		graph_vide[1][2] = 1;
		graph_vide[1][3] = 2;
		graph_vide[1][4] = 3;
		
		graph_vide[2][0] = 1;
		graph_vide[2][1] = 1;
		graph_vide[2][2] = 0;
		graph_vide[2][3] = 0;
		graph_vide[2][4] = 0;
		
		graph_vide[3][0] = 1;
		graph_vide[3][1] = 1;
		graph_vide[3][2] = 0;
		graph_vide[3][3] = 0;
		graph_vide[3][4] = 0;
		
		graph_vide[4][0] = 1;
		graph_vide[4][1] = 1;
		graph_vide[4][2] = 0;
		graph_vide[4][3] = 0;
		graph_vide[4][4] = 0;
		
		copier_graph_vide_dans_paquet(1);
		
		afficher_graph_du_paquet(1);
signer(1,s , 1);
afficher_signature(s);
signer(1,s , 2);
afficher_signature(s);

	
}

int tout_comparer(signature* s){
	int graph_isomorphe = -1;
	for (int w=0;w < taille_paquet;w++){
		
		for (int depart=1;depart<N+1 ;depart++){
			signer(w,s,depart);
			if (comparer_2_signatures(signature_primaire,s) == 1){
				graph_isomorphe=w;
				printf("le graph %d est isomorph avec le graph primaire\n",w);
				afficher_graph_du_paquet(w);
			}
			
		}
		
	}
	return graph_isomorphe;
}
int tout_comparer_intelligemment(signature* s){
	int graph_isomorphe = -1;
	for (int w=0;w < taille_paquet;w++){
		
		for (int depart=1;depart<N+1 ;depart++){
			signer_intelligemment(w,s,depart);
			if (comparer_2_signatures(signature_primaire,s) == 1){
				graph_isomorphe=w;
				printf("le graph %d est isomorph avec le graph primaire\n",w);
				afficher_graph_du_paquet(w);
			}
			
		}
		
	}
	return graph_isomorphe;
}
int main(){
	/// inits 
init_random();
signature* s=init_signature();
signature_primaire=init_signature();
pile* p=init_pile();



clock_t temps_depart; /* Temps initial en micro-secondes */
clock_t temps_fin; /* Temps final en micro-secondes */
float temps_cpu; /* Temps total en secondes */
temps_depart = clock ();

//remplis_paquet_avec_3_connexe();

generer_cycle_simple();
copier_graph_vide_dans_primaire();
copier_graph_vide_dans_paquet(1);
signer(1,signature_primaire,1);

remplis_paquet_avec_cycle();

afficher_graph_primaire();

//copier_primaire_dans_graph_vide();
//copier_graph_vide_dans_paquet(400);

temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps construction des graphs_cycle = %Lf sec\n",(long double)temps_cpu);

temps_depart =temps_fin;

tout_comparer(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison bete et mechante des graphs_cycle = %Lf sec\n",(long double)temps_cpu);////////////////////////////////////////

temps_depart =temps_fin;

tout_comparer_intelligemment(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison intelligente des graphs_cycle = %Lf sec\n",(long double)temps_cpu);

printf("--------------------------------------------------------------------------\n");
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

generer_arbre();
copier_graph_vide_dans_primaire();
copier_graph_vide_dans_paquet(1);
signer(1,signature_primaire,1);

remplis_paquet_avec_arbre();



temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps construction des graphs_arbres = %Lf sec\n",(long double)temps_cpu);

temps_depart =temps_fin;

tout_comparer(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison bete et mechante des graphs_arbres = %Lf sec\n",(long double)temps_cpu);////////////////////////////////////////

temps_depart =temps_fin;

tout_comparer_intelligemment(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison intelligente des graphs_arbres = %Lf sec\n",(long double)temps_cpu);


printf("--------------------------------------------------------------------------\n");
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

generer_3_connexe();
copier_graph_vide_dans_primaire();
copier_graph_vide_dans_paquet(1);
signer(1,signature_primaire,1);

remplis_paquet_avec_3_connexe();



temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps construction des graphs_3_connexe = %Lf sec\n",(long double)temps_cpu);

temps_depart =temps_fin;

tout_comparer(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison bete et mechante des graphs_3_connexe = %Lf sec\n",(long double)temps_cpu);////////////////////////////////////////

temps_depart =temps_fin;

tout_comparer_intelligemment(s);


temps_fin = clock ();
temps_cpu = (temps_fin - temps_depart) * 1e-6;
printf(" temps comparaison intelligente des graphs_3_connexe = %Lf sec\n",(long double)temps_cpu);







return 0;


}
