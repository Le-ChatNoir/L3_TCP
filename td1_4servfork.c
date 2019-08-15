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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

int BUFSIZE=256;

/*pour les tests*/
void wasteTime(unsigned long ms)
{
unsigned long t,t0;
struct timeval tv;
gettimeofday(&tv,(struct timezone *)0);
t0=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
do
  {
  gettimeofday(&tv,(struct timezone *)0);
  t=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
  } while(t-t0<ms);
}


int main (int argc, char **argv) {

	if(argc < 3)
		{
		printf("Utilisation du programme: %s [Port a utiliser] [Nom du fichier à creer]\n", argv[0]);	
		exit(0);
		}
	int n=0; /* compteur de processus fils*/
	int socketRV;
	char buf[256];
	struct sockaddr_in adresseRV;
	int socket_service;
	int lgadresseRV;
	int val;
	int lgadresseClient;
	struct sockaddr_in adresseClient;
	unsigned short port;
	char fileName[256];
	int nblu=0;
	unsigned long ms;/*temps de pause */


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
		n++;
		int pidCPY=fork();

		
		switch (pidCPY)
		{
			case -1 : perror("fork");
				    exit(EXIT_FAILURE);
				    break;
	  		case  0 : printf("Je suis le processus fils READ\n");
					sprintf(fileName,"%s%d",argv[2],n);
					int fdWRITE= open(fileName,O_CREAT|O_WRONLY|O_TRUNC,0777);
					while((nblu=read(socket_service, buf, BUFSIZE)) > 0){
						/* Lecture dans la socket d'une chaine de caractères */
						printf("Chaine recue : %s\n", buf);
						write(fdWRITE, buf, nblu);
						fflush(stdout);
						wasteTime(2);
					}
					
					
					close(socket_service);
					return 0;
	 	}


	}

	close(socketRV);

	return 0;
}
