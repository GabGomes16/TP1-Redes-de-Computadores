#include "../inc/client.h"

int ip_validation (const char *endereco){
    struct sockaddr_in v4;
    struct sockaddr_in6 v6;

    if(inet_pton(AF_INET, endereco, &(v4.sin_addr)) == 1){
        return 4;
    }
    if(inet_pton(AF_INET6, endereco, &(v6.sin6_addr)) == 1){
        return 6;
    }
    return 0;
}

void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta, char *endereco){
    memset(server_address, 0, sizeof(struct sockaddr_storage));

    if (dominio == AF_INET){
        struct sockaddr_in *v4 = (struct sockaddr_in*) server_address;

        v4->sin_family = dominio;
        v4->sin_port = htons(porta);
        inet_pton(AF_INET, endereco, &v4->sin_addr);
    }
    if (dominio == AF_INET6){
        struct sockaddr_in6 *v6 = (struct sockaddr_in6*) server_address;

        v6->sin6_family = dominio;
        v6->sin6_port = htons(porta);
        inet_pton(AF_INET6, endereco, &v6->sin6_addr);
    }
}

void guesses (int socket, int *tentativas, HackerMessage *server_msg, HackerMessage *client_msg){
    memset(server_msg, 0, sizeof(*server_msg));
    memset(client_msg, 0, sizeof(*client_msg));

    recv(socket, server_msg, sizeof(*server_msg), 0);

    //só deixa o programa seguir se houver uma entrada válida
    int valido = 0;
    while(!valido){
        printf("%s", server_msg->message);
        char guess[7];
        scanf("%6s", guess);
        if(strlen(guess) == 5){
            for (int i = 0; i < 5; i++){
                if (isdigit(guess[i])){
                    client_msg->guess[i] = guess[i] - '0';
                } else {
                    printf("Insira uma sequência válida!\n");
                    break;
                }
                if (i == 4){
                    valido = 1;
                }
            }
        } else {
            printf("Insira uma sequência válida!\n");
        }
    }
    client_msg->type = MSG_GUESS;
    send(socket, client_msg, sizeof(*client_msg), 0);
    recv(socket, server_msg, sizeof(*server_msg), 0);
    if (server_msg->win_status != 1){
        printf("Dica: %s\n", server_msg->message);
        (*tentativas)++;
        printf("Tentativas realizadas: %d\n", *tentativas);
    }
}

int main(int argc, char *argv[]){
    char *endereco = argv[1];
    int porta = atoi(argv[2]);

    int tipo = ip_validation(endereco);
    int dominio;
    socklen_t tamanho_endereco = 0;
    if (tipo == 0){
        printf("Insira um endereço de IP válido!\n");
        return(1);
    } else if (tipo == 4){
        dominio = AF_INET;
        tamanho_endereco = sizeof(struct sockaddr_in);
    } else{
        dominio = AF_INET6;
        tamanho_endereco = sizeof(struct sockaddr_in6);
    }

    int socket_client = socket(dominio, SOCK_STREAM, 0);

    struct sockaddr_storage server_address;
    server_especifications(&server_address, dominio, porta, endereco);

    if (connect(socket_client, (struct sockaddr*)&server_address, tamanho_endereco) < 0){
        perror("Erro no connect");
        exit(1);
    }

    HackerMessage server_menssage;
    HackerMessage client_message;
    client_message.type = MSG_GUESS;
    
    //loop que roda a função e encerra apenas quando a senha enviada estiver correta
    int tentativas = 0;
    memset(&server_menssage, 0, sizeof(server_menssage));
    server_menssage.win_status = 0;
    while (server_menssage.win_status != 1) {
        guesses(socket_client, &tentativas, &server_menssage, &client_message);
    }
    printf("Acesso concedido! Thaísa recuperou o sistema!\n");

    close(socket_client);
    
    return 0;
}