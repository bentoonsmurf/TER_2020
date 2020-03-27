
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define N 30// le nombre  de sommet du graph 
#define signature_taille 1000
/// ce sont des graphs non orienté 
/// la valeur dans les case indique l'ordre dans le quel il faut etre lu 
/// soit si graph[4][1]=2 ------> pour le sommet 4 l'arc de 4 vers 1 est a traiter en deuxieme

int etalon[N+1][N+1]; // la case 0 contien le degré du sommet 
int potentielle_isomorphe[N+1][N+1];
char signature_etalon[signature_taille];
char signature_isomorph[signature_taille];




void reset_mat(int ** matrice,int size){
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			matrice[i][j]=0;
		}
		
	}
	
}

int comparer(char * signature1, char* signature2 ){
		for(int i=0;i<signature_taille;i++){
			if (signature1[i] != signature2[i]) return 0;
		
		
		}
return 1;
}


void signer(int ** graph,char * signature){
	//not implemented yet
}



int main(){

/*
signer(etalon,signature_etalon);
signer(potentielle_isomorphe,signature_isomorph);

*/

comparer(signature_etalon,signature_isomorph);


printf("\n %d\n",1113*4 + 1154 + 1197*2 +1242*2+7213);// ca compile c'est deja ca

 //printf("10 shift 3  = %d\n", 10<<3); 




return 0;
}




