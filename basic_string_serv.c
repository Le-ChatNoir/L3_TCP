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

	if(argc < 2)
		{
		printf("Utilisation du programme: %s [Port a utiliser]\n", argv[0]);	
		exit(0);
		}

	int socketRV;
	char buf[256];
	struct sockaddr_in adresseRV;
	int socket_service;
	int lgadresseRV;
	int val;
	int lgadresseClient;
	struct sockaddr_in adresseClient;
	unsigned short port;

	/* Création de la socket locale */
	if((socketRV = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	/* Preparation à l'adresse locale */
	port = (unsigned short) atoi(argv[1]);

	adresseRV.sin_family = AF_INET;
	adresseRV.sin_port = htons(port);
	adresseRV.sin_addr.s_addr = htonl(INADDR_ANY); //Une des adresses de la machine
	lgadresseRV = sizeof(adresseRV);

	/* Attachement de la socket à l'adresse locale  */
	if((bind(socketRV, (struct sockaddr *) &adresseRV, lgadresseRV)) == -1)
	{
		perror("bind"); exit(3);
	}

	/* Declaration de l'ouverture du service */
	if( listen(socketRV, 10) == -1)
	{
		perror("listen");
		exit(4);
	}

	/* Boucle d'attente de connexion */
	while(1)
	{
		printf("Debut de boucle.\n");
		fflush(stdout);

		/* Attente d'un client */
		lgadresseClient = sizeof(adresseClient);
		socket_service = accept(socketRV, (struct sockaddr *) &adresseClient, &lgadresseClient);
		if(socket_service == -1 && errno == EINTR)
		{
			/* Reception d'un signal */
			continue;
		}
		if(socket_service == -1)
		{
			/* Erreur plus grave */
			perror("accept");
			exit(5);
		}

		/* Un client arrive */
		printf("Connexion acceptée\n");
		fflush(stdout);

		/* Lecture dans la socket d'une chaine de caractères */
		if(read(socket_service, buf, 256) < 0)
		{
			perror("read");
			exit(6);
		}
		printf("Chaine recue : %s\n", buf);
		fflush(stdout);

		/* Ecriture de la socket */
		val = 621;
		if(write(socket_service, &val, sizeof(int)) != sizeof(int))
		{
			perror("write");
			exit(7);
		}
		printf("Entier envoyé : %d\n", val);

		close(socket_service);
	}

	close(socketRV);

	return 0;
}
