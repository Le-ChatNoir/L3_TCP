#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main (int argc, char **argv) {

	if(argc < 3)
		{
		printf("Utilisation du programme: %s [Nom du serveur] [Port a utiliser]\n", argv[0]);	
		exit(0);
		}

	int sock;
	char buf[256];
	int entier_recu;
	struct sockaddr_in adresse_serveur;
	struct hostent *hote;
	unsigned short port;
	char ipserv[20];

	/* Création de la socket locale */
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
		perror("socket");
		exit(1);
		}

	/*Récupération de l'adresse du serveur a partir de son nom*/
	if ((hote = gethostbyname(argv[1])) == NULL)
		{
		printf("La machine demandée n'existe pas.\n");
		perror("gethostbyname");
		exit(2);
		}
	
	

	/* Preparation à l'adresse locale */
	port = (unsigned short) atoi(argv[2]);

	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_port = htons(port);

	bcopy(hote->h_addr, &adresse_serveur.sin_addr, hote->h_length);
	strcpy(ipserv, inet_ntoa(adresse_serveur.sin_addr));
	printf("Adresse IP du serveur: %s\n\n", ipserv);

	/* Demande de connexion au serveur */
	printf("En attente de connexion...\n");
	if((connect(sock, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))) == -1)
		{
		perror("connect"); exit(3);
		}

	/* Le serveur a accepté la connexion */
	printf("Vous êtes bien connecté à la machine %s\n\n", ipserv);

	/* Ecriture dans la socket d'une chaine */
	strcpy(buf, "Message pour le serveur...");
	if(write(sock, buf, strlen(buf)+1) != strlen(buf)+1)
		{
		perror("write");
		exit(4);
		}

	printf("Chaine envoyée : %s\n", buf);
	fflush(stdout);

	/* Lecture dans la socket d'un entier */
	if (read(sock, &entier_recu, sizeof(int)) < 0)
		{
		perror("read");
		exit(5);
		}

	printf("Entier reçu de la part du serveur : %d\n", entier_recu);
	fflush(stdout);

	return 0;
}
