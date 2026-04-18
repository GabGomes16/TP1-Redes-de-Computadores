#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSG_SIZE 128

typedef enum {
    MSG_START, //Servidor solicita a senha de acesso
    MSG_GUESS, //Cliente envia o palpite de 5 digitos
    MSG_FEEDBACK, // Servidor retorna as dicas (0, 1 ou 2)
    MSG_WIN, // Servidorin forma que o codigo foi quebrado
    MSG_ERROR, // Servidor reporta erro de formato/tamanho
    MSG_EXIT // Encerramento da conexao
} MessageType ;

typedef struct {
int type ; // Tipo da mensagem (MessageType)
int guess[5] ; // Vetor com os 5 digitos enviados pelo cliente
int feedback[5] ; // Dicas retornadas pelo servidor (2, 1 ou 0)
int attempts; // Contador de tentativas realizadas
int win_status; // 1 para vitoria, 0 para em jogo, −1 para erro
char message[MSG_SIZE] ; // Mensagem de texto para logs do terminal
} HackerMessage ;

