#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



int main (int argc, char **argv) {

	if(argc < 3)
		{
		printf("Utilisation du programme: %s [Nom du fichier source] [Nom du fichier cible]\n", argv[0]);	
		exit(0);
		}

	int BUFSIZE = 5;
	int nb_lus = 0;
	int nb_ecrits = 0;
	char chaine[30];
	int fdsource, fdcopy;
	chaine[30] = '\0';


	/* open() - Récupération du descripteur de fichier sur le fichier en lecture */

	fdsource = open(argv[1], O_RDONLY);
	if (fdsource < 0)
		{
		perror("open fdsource");
		exit(-1);
		}
	printf("Fichier source ouvert avec succès !\nFichier : %d\n\n", fdsource);

	/* open() - Création, ouverture et récupération du descripteur du fichier en écriture */

	fdcopy = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if (fdcopy < 0)
		{
		perror("open fdcopy");
		exit(-1);
		}
	printf("Fichier destination ouvert avec succès !\nFichier : %d\n\n", fdcopy);

	/* Boucle de copie (r/w) dans le fichier de destination  de BUFSIZE octetes jusqu'à la fin */

	while ((nb_lus = read(fdsource, chaine, BUFSIZE)) > 0)
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
	
	/* Fermeture des fichiers */

	close(fdsource);
	close(fdcopy);

	return 0;
}
