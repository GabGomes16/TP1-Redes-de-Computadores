#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//estrutura para guardar endereços
#include <netinet/in.h>

//validação de IP
#include <arpa/inet.h>

#include "messages.h"

int ip_validation (const char *endereco);

void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta);