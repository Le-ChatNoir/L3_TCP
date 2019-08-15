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

	if(argc < 3)
		{
		printf("Utilisation du programme: %s [Nom du serveur] [Port a utiliser] [Fichier a lire]\n", argv[0]);	
		exit(0);
		}
	
	/* Partie réseau */
	int sock;
	int entier_recu;
	struct sockaddr_in adresse_serveur;
	struct hostent *hote;
	unsigned short port;
	char ipserv[20];

	/* Variables de fichiers */
	int BUFSIZE = 150;
	int nb_lus = 0;
	int nb_ecrits = 0;
	char chaine[BUFSIZE+1];
	int fdsource;
	chaine[BUFSIZE+1] = '\0';

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
	
	/* open() - Récupération du descripteur de fichier sur le fichier en lecture */

	fdsource = open(argv[3], O_RDONLY);
	if (fdsource < 0)
		{
		perror("open fdsource");
		exit(-1);
		}
	printf("Fichier source ouvert avec succès !\nFichier : %d\n\n", fdsource);

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


	/* Ecriture dans la socket d'un fichier */
	strcpy(chaine, "Envoi du fichier sur le serveur...");
	while ((nb_lus = read(fdsource, chaine, BUFSIZE)) > 0)
		{
		printf("Nombre d'octets lus : %d\n", nb_lus);
		/*Affichage des BUFSIZE lus*/
		printf("%s\n", chaine);

		/*Ecriture sur le fichier destination*/

		if((nb_ecrits = write(sock, chaine, nb_lus)) != nb_lus)
			{
				perror("write");
				exit(-1);
			}
		}

	fflush(stdout);

	close(sock);
	close(fdsource);
	
	return 0;
}
