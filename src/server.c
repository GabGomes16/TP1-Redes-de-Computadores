#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Uso: %s <protocolo> <porta> <senha>\n", argv[0]);
        return 1;
    }

    char *protocolo = argv[1];
    int porta = atoi(argv[2]);
    char *senha = argv[3];

    int domain;

    if (strcmp(protocolo, "v4") == 0) {
        domain = AF_INET;
        printf("Servidor iniciado em modo IPv4 na porta %d\n", porta);
    } else if (strcmp(protocolo, "v6") == 0) {
        domain = AF_INET6;
        printf("Servidor iniciado em modo IPv6 na porta %d\n", porta);
    } else {
        printf("Erro: protocolo deve ser v4 ou v6\n");
        return 1;
    }

    int server_fd = socket(domain, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    if (domain == AF_INET) {
        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(porta);
        
        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Erro no bind");
            return 1;
        }

    } else {
        struct sockaddr_in6 addr6;

        addr6.sin6_family = AF_INET6;
        addr6.sin6_addr = in6addr_any;
        addr6.sin6_port = htons(porta);

        if (bind(server_fd, (struct sockaddr*)&addr6, sizeof(addr6)) < 0) {
            perror("Erro no bind");
            return 1;
        }
    }

    if (listen(server_fd, 5) < 0) {
        perror("Erro no listen");
        return 1;
    }

    int client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("Erro no accept");
        return 1;
    }

    printf("Cliente conectado\n");

    close(client_fd);
    close(server_fd);

    printf("Cliente desconectado\n");

    return 0;
}
