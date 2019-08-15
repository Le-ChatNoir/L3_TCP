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
#include <fcntl.h>

int main (int argc, char **argv) {

	if(argc < 2)
		{
		printf("Utilisation du programme: %s [Port a utiliser] [Nom du fichier à écrire]\n", argv[0]);	
		exit(0);
		}

	int socketRV;
	struct sockaddr_in adresseRV;
	int socket_service;
	int lgadresseRV;
	int val;
	int lgadresseClient;
	struct sockaddr_in adresseClient;
	unsigned short port;

	/* Variables de fichiers */
	int BUFSIZE = 5;
	int nb_lus = 0;
	int nb_ecrits = 0;
	char chaine[BUFSIZE];
	int fdcopy;
	chaine[BUFSIZE] = '\0';

	/* Création de la socket locale */
	if((socketRV = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	/* open() - Création, ouverture et récupération du descripteur du fichier en écriture */

	fdcopy = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if (fdcopy < 0)
		{
		perror("open fdcopy");
		exit(-1);
		}
	printf("Fichier destination ouvert avec succès !\nFichier : %d\n\n", fdcopy);

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

		while ((nb_lus = read(socket_service, chaine, BUFSIZE)) > 0)
		{
		printf("Nombre d'octets lus : %d\n", nb_lus);
		/*Affichage des BUFSIZE lus*/
		printf("%s\n", chaine);

		/*Ecriture sur le fichier destination*/

		if((nb_ecrits = write(fdcopy, chaine, nb_lus)) != nb_lus)
			{
			perror("write");
			exit(-1);
			}
		}


		close(socket_service);
		exit(0);
	}

	close(socketRV);
	close(fdcopy);

	return 0;
}
