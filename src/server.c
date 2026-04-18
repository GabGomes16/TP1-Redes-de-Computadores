#include "../inc/server.h"

void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta){
    memset(server_address, 0, sizeof(struct sockaddr_storage));

    if (dominio == AF_INET){
        struct sockaddr_in *v4 = (struct sockaddr_in*) server_address;

        v4->sin_family = dominio;
        v4->sin_port = htons(porta);
        v4->sin_addr.s_addr = INADDR_ANY;

        //printf("Dominio: %d\n", dominio);
        //printf("Porta: %d\n", ntohs(v4->sin_port));
        //printf("IP: %u\n", v4->sin_addr.s_addr);  
    }
    if (dominio == AF_INET6){
        struct sockaddr_in6 *v6 = (struct sockaddr_in6*) server_address;

        v6->sin6_family = dominio;
        v6->sin6_port = htons(porta);
        v6->sin6_addr = in6addr_any;

        //printf("Dominio: %d\n", dominio);
        //printf("Porta: %d\n", ntohs(v6->sin6_port));
    }
}

int main(int argc, char *argv[]){
    char *protocolo = argv[1];
    int porta = atoi(argv[2]);
    char *senha = argv[3];

    //tratamento de aceitação de ambas as familias de protocolos: IPv4 e IPv6
    int dominio;
    socklen_t tamanho_endereco = 0;
    if (strcmp(protocolo, "v4") == 0){
        dominio = AF_INET;
        tamanho_endereco = sizeof(struct sockaddr_in);
        printf("Servidor iniciado em modo IPv4 na porta %d\n", porta);
    } else if (strcmp(protocolo, "v6") == 0){
        dominio = AF_INET6;
        tamanho_endereco = sizeof(struct sockaddr_in6);
        printf("Servidor iniciado em modo IPv6 na porta %d\n", porta);
    } else {
        fprintf(stderr, "Erro: protocolo inválido\n");
        return 1;
    }

    //cria um socket:
    // -com dominio escolhido pelo usuário (v4 ou v6);
    // -com o tipo STREAM que é orientado a conexão ;
    // -e protocolo TCP/IP como padrão: 0.
    int socket_server = socket(dominio, SOCK_STREAM, 0);

    if (socket_server < 0) {
        perror("Erro no socket");
        exit(1);
    }
    
    //especificação do servidor:
    // -com a familia de endereços de acordo com o protocolo escolhido: v4 ou v6;
    // -com a formatação da entrada do argv da porta
    // -com o servidor aceitando mensagens de qualquer endereço
    struct sockaddr_storage server_address;
    server_especifications(&server_address, dominio, porta);

    //bind: junta o socket com o IP e porta
    if (bind(socket_server, (struct sockaddr*)&server_address, tamanho_endereco) < 0) {
        perror("Erro no bind");
        exit(1);
    }

    //listen com backlog da lista de espera dos 10 sockets solicitando conexão
    if (listen(socket_server, 10) < 0) {
        perror("Erro no listen");
        exit(1);
    }

    //endpoint do cliente, sem especificação de endereço nem tamanho de endereço
    printf("Antes do accept.\n");
    int socket_client = accept(socket_server, NULL, NULL);
    printf("Cliente Conectado\n");

    char client_message[256];
    recv(socket_client, client_message, sizeof(client_message), 0);
    printf("%s\n", client_message);

    printf("Cliente Desconectado\n");
    close(socket_server);

    return 0;
}
