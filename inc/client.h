#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//pra conferir os tipos
#include <ctype.h>

//criação de sockets
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//criação de sockets
#include <sys/socket.h>
#include <sys/types.h>

//fechar os sockets
#include <unistd.h>

//estrutura para guardar endereços
#include <netinet/in.h>

//validação de IP
#include <arpa/inet.h>

#include "messages.h"

//verifica se o IP digitado está em um formato aceitável, seja ele IPv4 ou IPv6
int ip_validation (const char *endereco);

//especificações do server que muda de acordo com a escolha do método (se é IPv4 ou IPv6)
void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta, char *endereco);

//Sistema de quebra senhas da visão do client
// - faz a sequência de: recebimento, envio, recebimento; até que a Thaísa recupere o sistema.
// - faz a validação e tratamento dos guesses enviados para o server
void guesses (int socket, int *tentativas, HackerMessage *server_msg, HackerMessage *client_msg);