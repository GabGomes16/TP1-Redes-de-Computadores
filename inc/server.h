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

#include "messages.h"

//especificações do server que muda de acordo com a escolha do método (se é IPv4 ou IPv6)
void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta);

//Sistema de quebra senhas da visão do server
// - faz a sequência de: envio, recebimento, envio; até que a senha esteja correta
// - faz a verificação da senha e configura o feedback da forma necessária
// - faz a contagem de tentativas e a exibição das mensagens enviadas pelo server
void game (int socket, int senha[5], int frequencia[10], HackerMessage *server_msg, HackerMessage *client_msg);