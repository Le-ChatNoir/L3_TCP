#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main (int argc, char **argv) {

/* Recherche du nom de la machine avec gethostname*/
char nom1[253];  
char nom2[253];

if(argc < 2){
	printf("Utilisation du programme: %s [IP de la machine cible]\n", argv[0]);	
exit(0);
}

strcpy(nom1, argv[1]); 

/* Recupération de l'adresse IP avec gethostbyname */
struct in_addr adresse;
struct hostent *h;        

h = gethostbyname(nom1);

if(h == NULL){
	printf("La machine demandée n'existe pas.\n");
	perror("gethostbyname"); exit(-1);
}
bcopy(h->h_addr, &adresse, h->h_length);
strcpy(nom2, inet_ntoa(adresse));

/* Affichage des informations de l'adresse IP */
//gethostname(nom1, 253);
printf("Information de la machine : %s\n\n", nom1);
printf("Adresse IP: %s\n", nom2);
printf("Taille de l'IP: %d\n", h->h_length);

printf("Adresse en u : %u\nAdresse en d: %d\n", adresse.s_addr, adresse.s_addr);

return 0;
}
