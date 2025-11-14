// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.


    // 🧩 Nível Novato: Fila de Peças Futuras
    //
    // - Crie uma struct Peca com os campos: tipo (char) e id (int).
    // - Implemente uma fila circular com capacidade para 5 peças.
    // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
    // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
    // - Exiba a fila após cada ação com uma função mostrarFila().
    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair
    // - A cada remoção, insira uma nova peça ao final da fila.



    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------------------
// STRUCTS
// -------------------------------------------
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

typedef struct {
    Peca itens[5];
    int front, rear, size;
} Fila;

typedef struct {
    Peca itens[3];
    int top;
} Pilha;

// Para desfazer
typedef struct {
    int op;        // código da operação
    Peca p1, p2;   // peças envolvidas
} Acao;

Pilha historico; // pilha de desfazer

// -------------------------------------------
// GERA PEÇA AUTOMATICAMENTE
// -------------------------------------------
char tipos[] = {'I', 'O', 'T', 'L'};
int contadorID = 1;

Peca gerarPeca() {
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// -------------------------------------------
// FILA
// -------------------------------------------
void inicializarFila(Fila *f) {
    f->front = 0;
    f->rear = 4;
    f->size = 5;

    for (int i = 0; i < 5; i++)
        f->itens[i] = gerarPeca();
}

int filaVazia(Fila *f) {
    return f->size == 0;
}

int filaCheia(Fila *f) {
    return f->size == 5;
}

Peca removerFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        Peca nul = {'-', -1};
        return nul;
    }
    Peca p = f->itens[f->front];
    f->front = (f->front + 1) % 5;
    f->size--;
    return p;
}

void inserirFila(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }
    f->rear = (f->rear + 1) % 5;
    f->itens[f->rear] = p;
    f->size++;
}

// -------------------------------------------
// PILHA
// -------------------------------------------
void inicializarPilha(Pilha *p) {
    p->top = -1;
}

int pilhaVazia(Pilha *p) {
    return p->top == -1;
}

int pilhaCheia(Pilha *p) {
    return p->top == 2;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }
    p->itens[++p->top] = x;
}

Peca pop(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        Peca nul = {'-', -1};
        return nul;
    }
    return p->itens[p->top--];
}

// -------------------------------------------
// CONTROLE DE DESFAZER
// -------------------------------------------
void registrarAcao(int codigo, Peca p1, Peca p2) {
    if (pilhaCheia(&historico)) return;
    Acao *a = (Acao*)&historico.itens[++historico.top];
    a->op = codigo;
    a->p1 = p1;
    a->p2 = p2;
}

void desfazer(Fila *f, Pilha *r) {
    if (pilhaVazia(&historico)) {
        printf("Nada a desfazer.\n");
        return;
    }

    Acao *a = (Acao*)&historico.itens[historico.top--];

    switch (a->op) {
        case 1: // desfaz jogar peça
            // recoloca peça no início
            f->front = (f->front - 1 + 5) % 5;
            f->itens[f->front] = a->p1;
            f->size++;
            break;

        case 2: // desfaz reserva
            push(r, a->p1);
            break;

        case 3: // desfaz uso de peça reservada
            inserirFila(f, a->p1);
            break;

        case 4: // desfaz troca fila-pilha
            // desfaz novamente trocando de volta
            {
                Peca temp = a->p1;
                a->p1 = a->p2;
                a->p2 = temp;

                // fila recebe p1
                f->itens[f->front] = a->p1;
                // pilha recebe p2
                r->itens[r->top] = a->p2;
            }
            break;
    }

    printf("Última ação desfeita!\n");
}

// -------------------------------------------
// INVERTE FILA
// -------------------------------------------
void inverterFila(Fila *f) {
    int i = f->front;
    int j = (f->front + f->size - 1) % 5;

    for (int k = 0; k < f->size / 2; k++) {
        Peca temp = f->itens[i];
        f->itens[i] = f->itens[j];
        f->itens[j] = temp;
        i = (i + 1) % 5;
        j = (j - 1 + 5) % 5;
    }
}

// -------------------------------------------
// EXIBIÇÃO
// -------------------------------------------
void mostrarFila(Fila *f) {
    printf("\nFila: ");
    int idx = f->front;
    for (int i = 0; i < f->size; i++) {
        printf("[%c-%d] ", f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % 5;
    }
}

void mostrarPilha(Pilha *p) {
    printf("\nPilha: ");
    for (int i = 0; i <= p->top; i++)
        printf("[%c-%d] ", p->itens[i].tipo, p->itens[i].id);
}

// -------------------------------------------
// MAIN
// -------------------------------------------
int main() {
    Fila fila;
    Pilha reserva;
    inicializarFila(&fila);
    inicializarPilha(&reserva);
    inicializarPilha(&historico);

    int op;

    do {
        printf("\n\n========= TETRIS STACK - MESTRE =========\n");
        mostrarFila(&fila);
        mostrarPilha(&reserva);

        printf("\n\nMenu:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar topo da pilha com frente da fila\n");
        printf("5 - Desfazer última ação\n");
        printf("6 - Inverter fila\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch(op) {
            case 1: {
                Peca jogada = removerFila(&fila);
                Peca nova = gerarPeca();
                inserirFila(&fila, nova);
                registrarAcao(1, jogada, nova);
                printf("Jogou peça!\n");
                break;
            }

            case 2: {
                if (pilhaCheia(&reserva)) {
                    printf("Pilha cheia!\n");
                    break;
                }
                Peca p = removerFila(&fila);
                push(&reserva, p);
                Peca nova = gerarPeca();
                inserirFila(&fila, nova);
                registrarAcao(2, p, nova);
                printf("Peça reservada!\n");
                break;
            }

            case 3: {
                if (pilhaVazia(&reserva)) {
                    printf("Não há peças reservadas!\n");
                    break;
                }
                Peca usada = pop(&reserva);
                registrarAcao(3, usada, (Peca){'-', -1});
                printf("Peça reservada usada!\n");
                break;
            }

            case 4: {
                if (pilhaVazia(&reserva)) {
                    printf("Não há peça na pilha!\n");
                    break;
                }
                Peca topo = reserva.itens[reserva.top];
                Peca frente = fila.itens[fila.front];
                reserva.itens[reserva.top] = frente;
                fila.itens[fila.front] = topo;
                registrarAcao(4, topo, frente);
                printf("Peças trocadas!\n");
                break;
            }

            case 5:
                desfazer(&fila, &reserva);
                break;

            case 6:
                inverterFila(&fila);
                printf("Fila invertida!\n");
                break;
        }

    } while (op != 0);

    printf("Saindo...\n");
    return 0;
}


