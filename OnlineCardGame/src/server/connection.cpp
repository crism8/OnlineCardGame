/**
 * Autor: Kamil Zieliński
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "connection.h"
#include "loger.h"
int MAX_CLIENTS=20;
struct client_t
{
    int socket;
    pthread_mutex_t* mutex;
    char nick[50];
};
/**
 * Tworzy połączenie korzystając z gniazd BSD
 *
 * @return numer gniazda
 */
int create_connection()
{
    int sock;
    socklen_t length;
    struct sockaddr_in6 server;
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Blad socketu");
        exit(1);
    }
    server.sin6_family = AF_INET6;
    server.sin6_addr = in6addr_any;
    server.sin6_port = htons(8888);
    if (bind(sock, (struct sockaddr *) &server,sizeof server) == -1)
    {
        perror("Blad bindingu socketu");
        exit(1);
    }

    length = sizeof(server);
    if (getsockname(sock,(struct sockaddr *) &server, &length) == -1)
    {
        perror("Blad getsockname");
        exit(1);
    }
    printf("\nPort nasluchujacy: %d\n", ntohs(server.sin6_port));
    return sock;

}

/**
 * Funkcja nasłuchująca i dla każdego klienta włączająca odzielny wątek do obsługi
 */
void listen_connections(int sock,char haslo[])
{
    int liczbaGraczy=0;
    pthread_t thread; 
    listen(sock, MAX_CLIENTS);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    do 
    {
        char trybGracza[2];
    	struct client_t *new_client;
        //alokacja kilenta
    	if ((new_client = (client_t*)calloc(1, sizeof(struct client_t))) == NULL)
    	{
    		perror("Blad alokacji pamieci");
    		exit(1);
    	}
        new_client->socket = accept(sock,(struct sockaddr *) 0,(socklen_t *) 0);
        if (new_client->socket == -1 )
        {
            perror("Blad accept");
        }
        else
        {   
 	    if(( recv( new_client->socket,new_client->nick, 50, 0 ) ) <= 0 )
            {
        		perror( "Blad recv\n" );
        		exit( - 1 );
            }
            pthread_mutex_unlock(&mutex);
            char czyDobreHaslo[2]="0";
            int i=0;
            char haslo2[200];
  	    int temp;
	    while(i<5 && czyDobreHaslo[0]=='0')
   	    {  
		if(i==4)
		  break;                
		if(( recv( new_client->socket, haslo2, 201, 0 ) ) <= 0 )
            	{
        		perror( "Blad recv\n" );
        		exit( - 1 );
            	}
                printf("Uzytkownik %s podal haslo: %s \n",new_client->nick,haslo2);
		temp=strcmp(haslo,haslo2);
		if(temp==0)
			czyDobreHaslo[0]='1';
		else	
			czyDobreHaslo[0]='0';
		write(new_client->socket,czyDobreHaslo,2);
		++i;
            }
            if(czyDobreHaslo[0]=='1')
 	    {
		
		write(new_client->socket, "Witaj, chcesz byc graczem (1) czy moze widzem(2). Wybierz  odpowiedni numer:",81 );
            //tu bedzie obsługa watek
            	if(( recv( new_client->socket, trybGracza, 2, 0 ) ) <= 0 )
        	{
        	     perror( "Blad recv\n" );
        	     exit( - 1 );
        	}
		liczbaGraczy+=1;
        	printf("Tryb gracza: %s\n",trybGracza);
            }
 	    pthread_mutex_unlock(&mutex);
       //login(new_client->socket,haslo, &mutex,new_client->nick);
       }
    } while(1);
}
