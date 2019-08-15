#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main (int argc, char **argv) {
int retour;

if(argc < 2){
	printf("Utilisation du programme: %s [IP de la machine cible]\n", argv[0]);	
exit(0);
}

char nom1[253];  
strcpy(nom1, argv[1]);                

retour = gethostname(nom1, 253);
printf("Information de la machine : %s\n", nom1);

return 0;
}
