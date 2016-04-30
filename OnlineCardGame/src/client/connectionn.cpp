/**
 * Autor: Kamil Zieliński
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connectionn.h"
/*
  Funkcja do tworzenia połączeń, przypisuje adres lokalny gniazdu.
*/

void create_connection(int argc, char** argv, int& sock,char nick[])
{  
  int i=0;
  int getC;
  char conn[2];
  char trybGracza[2];
  char haslo[200];
  //zmiena mowiaca czy adres jest z przestrzeni ipv6 czy ipv4
  bool ipv6 = false;
  //struktura dla ipv6
  struct sockaddr_in6 server_ipv6;
  //struktura dla ipv4
  struct sockaddr_in server_ipv4;
  //stuktura do danych hosta
  struct hostent* hp;
  hp = gethostbyname(argv[1]);
  //jezeli adres hosta nie jest z przestrzeni ipv4
  if(hp == (struct hostent *)0)
  {
    ipv6 = true;
    hp = gethostbyname2(argv[1],AF_INET6);
  }
  //adres nie jest z przestrzeni ipv4 i ipv6
  if(hp == (struct hostent *)0)
  {
    printf("Nie znaleziono hosta \n");
    exit(2);
  }

  //stworz gniazdo
  if(ipv6)
  {
    sock = socket(AF_INET6, SOCK_STREAM, 0);
  }
  else
  {
    sock = socket(AF_INET, SOCK_STREAM, 0);
  }
  if(sock == -1)
  {
    perror ("BLAD TWORZENIA SOCKETU");
    exit(1);
  }
  //jeżeli adres hosta z ipv6
  if(ipv6)
  {
    //ustawiamy rodzine adresow na ipv6
    server_ipv6.sin6_family = AF_INET6;
    //ustawiamy adres Ip do polaczenia
    memcpy((char *) &server_ipv6.sin6_addr, (char *) hp->h_addr, hp->h_length);
   //ustawiamy port do polaczenia
    server_ipv6.sin6_port = htons(atoi(argv[2]));
    //laczenie
    if(connect(sock, (struct sockaddr*) &server_ipv6, sizeof(server_ipv6) )==-1)
   {
      perror("Blad polaczenia");
      exit(1);
    }
  }
 
  else
  {  
    //ustawiamy rodzine adresow na ipv4 
    server_ipv4.sin_family = AF_INET;
    //ustawiamy adres Ip do polaczenia
    memcpy((char *) &server_ipv4.sin_addr, (char *) hp->h_addr, hp->h_length);
    //ustawiamy port do polaczenia
    server_ipv4.sin_port = htons(atoi(argv[2]));
    //laczenie
    if(connect(sock, (struct sockaddr*) &server_ipv4, sizeof(server_ipv4) )==-1)
    {
      perror("Blad polaczenia");
      exit(1);
    }
  }
  write(sock,nick,50);
  //wczytywanie hasla do serwera:
  printf("Podaj haslo zabezpieczajace serwer: ");
  scanf("%99[^\n]",haslo);
  getC=getchar();
  write(sock,haslo,201);
  char bufor[1024];
  if(( recv( sock, conn, 2, 0 ) ) <= 0 )
  {
        perror( "Blad recv\n" );
        exit(- 1 );
  }
  printf("Haslo: %s\n",conn);
  while( conn[0]=='0')
  {
     if(i==3)
     {
        printf("Za duzo zlych prob logowania.Zamykam polaczenie!\n\n");
        exit(1);
     }
     printf("Podaj haslo zabezpieczajace serwer: ");
     scanf("%99[^\n]",haslo);
     getC=getchar();
     write(sock,haslo,201);
     ++i;
     if(( recv( sock, conn, 2, 0 ) ) <= 0 )
     {
       perror( "Blad recv\n" );
        exit(- 1 );
     }
  }
  if(( recv( sock, bufor, sizeof( bufor ), 0 ) ) <= 0 )
  {
        perror( "Blad recv\n" );
        exit( - 1 );
  }
  printf( "Server: %s ", bufor );
  scanf("%s",trybGracza);
  if(trybGracza[0]!='1' && trybGracza[0]!='2')
        strcpy( trybGracza,"2" );
  write(sock,trybGracza,2);
  //sleep(5);
  char test[1024];
  while(1){
  	if(( recv( sock, test, sizeof( test ), 0 ) ) <= 0 )
  	{
        	perror( "Blad recv\n" );
        	exit( - 1 );
  	}
  	printf( "Server: %s ", test );
  }
}

