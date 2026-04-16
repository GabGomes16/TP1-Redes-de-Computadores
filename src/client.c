#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s <ip> <porta>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    int porta = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(porta);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("Erro: IP inválido\nExemplo de IP válido: 127.0.1.1\n");
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao conectar");
        return 1;
    }

    printf("Cliente Conectado\n");

    close(sock);

    return 0;
}