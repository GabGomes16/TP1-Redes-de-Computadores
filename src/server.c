#include "../inc/server.h"

int send_all(int socket, void *buffer, int length){
    int total = 0;
    int bytes;

    while (total < length) {
        bytes = send(socket, (char*)buffer + total, length - total, 0);

        if (bytes <= 0) {
            return -1; // erro
        }

        total += bytes;
    }

    return total;
}

int recv_all(int socket, void *buffer, int length){
    int total = 0;
    int bytes;

    while (total < length) {
        bytes = recv(socket, (char*)buffer + total, length - total, 0);

        if (bytes <= 0) {
            return -1; // erro
        }

        total += bytes;
    }

    return total;
}

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

void game (int socket, int senha[5], int frequencia[10], HackerMessage *server_msg, HackerMessage *client_msg){
    memset(server_msg, 0, sizeof(*server_msg));
    memset(client_msg, 0, sizeof(*client_msg));

    server_msg->type = MSG_START;
    const char msg_start[MSG_SIZE] = "Insira seu palpite:\n> ";
    strcpy(server_msg->message, msg_start);

    send_all(socket, server_msg, sizeof(*server_msg));
    recv_all(socket, client_msg, sizeof(*client_msg));

    int freq_local[10];
    for (int i = 0; i < 10; i++) {
        freq_local[i] = frequencia[i];
    }

    char feedback_msg[MSG_SIZE]; 
    for (int i = 0; i < 5; i++) {
        feedback_msg[i] = '?';
    }

    for (int i = 0; i < 5; i++){
        if (client_msg->guess[i] == senha[i]){
            feedback_msg[i] = senha[i] + '0';
            freq_local[client_msg->guess[i]]--;
        }
    }
    for (int i = 0; i < 5; i++){
        if (feedback_msg[i] == '?'){
            if (freq_local[client_msg->guess[i]] > 0){
                feedback_msg[i] = '*';
                freq_local[client_msg->guess[i]]--;
            }else {
                feedback_msg[i] = '_';
            }
        }
    }
    feedback_msg[5] = '\0';

    server_msg->win_status = 1;
    for (int i = 0; i < 5; i++){
        if (client_msg->guess[i] != senha[i]) {
            server_msg->win_status = 0;
            break;
        }
    }
    strcpy(server_msg->message, feedback_msg);
    send_all(socket, server_msg, sizeof(*server_msg));
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

    // conferir se a senha é válida (toda numérica e com 5 dígitos)
    int senha_numerica[5];
    if(strlen(senha) == 5){
        for (int i = 0; i < 5; i++){
            if (isdigit(senha[i])){
                senha_numerica[i] = senha[i] - '0';
            } else {
                printf("Senha inválida! Use apenas dígitos.\n");
                return 1;
            }
        }
    } else {
        printf("Senha inválida! Use exatamente 5 dígitos.\n");
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
    // printf("Antes do accept.\n");
    int socket_client = accept(socket_server, NULL, NULL);
    if (socket_client < 0) {
        perror("Erro no accept");
        exit(1);
    }
    printf("Cliente conectado\n");

    HackerMessage server_menssage;
    HackerMessage client_menssage;

    int frequencia[10] = {0};
    for (int i = 0; i < 5; i++){
        int numero = senha_numerica[i];
        frequencia[numero]++;
    }

    memset(&server_menssage, 0, sizeof(server_menssage));
    server_menssage.win_status = 0;
    while (server_menssage.win_status != 1){
        game(socket_client, senha_numerica, frequencia, &server_menssage, &client_menssage);
    }

    printf("Cliente desconectado\n");
    close(socket_server);
    close(socket_client);

    return 0;
}
