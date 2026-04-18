#include "../inc/client.h"

int ip_validation (const char *endereco){
    struct sockaddr_in v4;
    struct sockaddr_in6 v6;

    //testa se é IPv4
    if(inet_pton(AF_INET, endereco, &(v4.sin_addr)) == 1){
        return 4;
    }
    //testa se é IPv6
    if(inet_pton(AF_INET6, endereco, &(v6.sin6_addr)) == 1){
        return 6;
    }
    //caso seja inválido
    return 0;
}

void server_especifications (struct sockaddr_storage *server_address, int dominio, int porta){
    memset(server_address, 0, sizeof(struct sockaddr_storage));

    if (dominio == AF_INET){
        struct sockaddr_in *v4 = (struct sockaddr_in*) server_address;

        v4->sin_family = dominio;
        v4->sin_port = htons(porta);
        v4->sin_addr.s_addr = INADDR_ANY;
    }
    if (dominio == AF_INET6){
        struct sockaddr_in6 *v6 = (struct sockaddr_in6*) server_address;

        v6->sin6_family = dominio;
        v6->sin6_port = htons(porta);
        v6->sin6_addr = in6addr_any;
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

    //cria um socket:
    // -com dominio escolhido pelo usuário (v4 ou v6);
    // -com o tipo STREAM que é orientado a conexão ;
    // -e protocolo TCP/IP como padrão: 0.
    int socket_client = socket(dominio, SOCK_STREAM, 0);

    struct sockaddr_storage server_address;
    server_especifications(&server_address, dominio, porta);

    if (connect(socket_client, (struct sockaddr*)&server_address, tamanho_endereco)){
        perror("Erro no connect");
        exit(1);
    }
    
    char client_message[256] = "OiÊ!";
    send(socket_client, client_message, sizeof(client_message), 0);

    close(socket_client);
}