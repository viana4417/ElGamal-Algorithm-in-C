#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int inversoModular(int num, int modulo);
int nprimoAleatorio();
long long expMod(long long base, long long exp, long long mod);

void criptografia();
void decifrarMensagem();

// Struct para a chave publica
typedef struct {
    int nprimo; // Numero primo aleatorio, quanto maior, mais seguro
    int gerador; // Numero inteiro aleatorio
    int e; // Valor calculado como: gerador^chaveprivada mod nprimo
} chavePublica;

int main() {
    setlocale(LC_ALL, "PT_BR");

    srand(time(NULL));

    int escolha = 0;

    while (escolha != 3) {
      printf("1 - Criptografar mensagem\n2 - Decifrar uma mensagem\n3 - Sair\n\n");

      scanf("%d", &escolha);
      getchar();

      switch (escolha) {
        case 1:
        criptografia();
        break;

        case 2:
        decifrarMensagem();
        break;
      }
    }

    return 0;
}

int inversoModular(int num, int modulo) {
    int i;
    for(i = 1; i < modulo; i++) {
        if ((num * i) % modulo == 1) {
            return i;
        }
    }
    return -1;
}

int nprimoAleatorio() {
    int num = rand() % 10000 + 300;
    int i;
    for(i = 2; i < num; i++) {
        if(num % i == 0) {
            return nprimoAleatorio();
        }
    }
    return num;
}

long long expMod(long long base, long long expoente, long long mod) {
    long long resul = 1;

    base = base % mod;

    while (expoente >= 1) {
        if (expoente % 2 == 1) {
            resul = (resul * base) % mod;
        }
        expoente = expoente / 2;
        base = (base * base) % mod;
    }
    return resul;
}

void criptografia() {
    char m[500]; // String que vai ser convertida para um vetor de inteiros
    printf("Mensagem a ser criptografada: ");
    scanf("%500[^\n]", m);

    int mensagem[strlen(m)]; // Vetor de inteiros que vai armazenar os valores ASCII da string m

    int i, check;

    for(i = 0; i < strlen(m); i++) {
        mensagem[i] = (int)m[i];
    }

    chavePublica chavePub;

    int c1; // Primeira parte da cifra, calculada como c1 = gerador^chavesecreta mod nprimo
    int c2[strlen(m)]; // Segunda parte da cifra, representa a mensagem criptografada, calculada para cada caractere da mensagem como: c2[i] = mensagem[i] X e^chavesecreta mod nprimo
    int chaveprivada; // Chave privada do algoritmo, escolhida aleatoriamente
    int chavesecreta; // Chave aleatoria gerada para cada mensagem criptografada, usada para gerar a cifra sem revelar a chave privada

    // Randomização das chaves
    chavePub.nprimo = nprimoAleatorio();
    chavePub.gerador = rand() % 300 + 2;
    chaveprivada = rand() % chavePub.nprimo;
    chavePub.e = expMod(chavePub.gerador, chaveprivada, chavePub.nprimo);

    chavesecreta = rand() % 300 + 2;

    // Calculo da primeira parte da cifra
    c1 = expMod(chavePub.gerador, chavesecreta, chavePub.nprimo);

    // Calculo da segunda parte da cifra para cada caractere da mensagem
    for(i = 0; i < strlen(m); i++) {
        c2[i] = (mensagem[i] * expMod(chavePub.e, chavesecreta, chavePub.nprimo)) % chavePub.nprimo;
    }

  printf("\nChave privada: %d", chaveprivada);
    printf("\nChave publica (numero primo, gerador, e): %d, %d, %d", chavePub.nprimo, chavePub.gerador, chavePub.e);
    printf("\nChave secreta: %d", chavesecreta);
    printf("\nC1: %d\n", c1);

    printf("C2: ");
    for(i = 0; i < strlen(m); i++) {
        printf("%d ", c2[i]);
    }

  printf("\nQuantidade de caracteres: %d\n\n", strlen(m));
}

void decifrarMensagem() {

    int i, car, chaveprivada, c1, primo;

    printf("Digite a chave privada: ");
    scanf(" %d", &chaveprivada);

    printf("Digite o numero primo: ");
    scanf(" %d", &primo);

    printf("Digite a primeira parte da cifra (C1): ");
    scanf(" %d", &c1);

    printf("Digite a quantidade de caracteres da mensagem que voce quer decifrar: ");
    scanf(" %d", &car);

    int c2[car];
    printf("Digite a segunda parte da cifra (C2): ");

    for(i = 0; i < car; i++) {
      scanf(" %d", &c2[i]);
    }

    int mdesc[car]; // Variavel que vai armazenar a mensagem decifrada

    for(i = 0; i < car; i++) {
        mdesc[i] = (c2[i] * inversoModular(expMod(c1, chaveprivada, primo), primo)) % primo;
    }

    printf("\nMensagem decifrada: ");
    for(i = 0; i < car; i++) {
        printf("%c", (char)mdesc[i]);
    }
    printf("\n\n");
}
