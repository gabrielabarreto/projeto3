#ifndef SPLAY_SPLAY_IMPL_H
#define SPLAY_SPLAY_IMPL_H

#include "splay.h"

namespace splay {
    /*
     * funções privativas
     */

    template <typename T> Nodo<T> * cria_nodo(const T & algo, Nodo<T> * pai) {
        Nodo<T> * p_nodo = new Nodo<T>;

        p_nodo->dado = algo;
        p_nodo->pai = pai;
        // não tem filhos ... ao menos não inicialmente
        p_nodo->esq = nullptr;
        p_nodo->dir = nullptr;

        return p_nodo;
    }

    // atualiza o nodo pai de "atual", que deve agora
    // estar conectado em "novo"
    template <typename T> void atualiza_pai(Nodo<T> * novo, Nodo<T> * atual) {
        novo->pai = atual->pai;
        if (atual->pai != nullptr) {
            if (na_esquerda(atual)) atual->pai->esq = novo;
            else atual->pai->dir = novo;
        }
    }

    template <typename T> void zig(Nodo<T> * nodoB) {
        // nodoA é o pai do nodo B
        Nodo<T> * nodoA = nodoB->pai;
        // sub2 está à direita do nodo B
        Nodo<T> * sub2 = nodoB->dir;

        // Deve-se atualizar o nodo pai do nodoB, o qual
        // deve ser o mesmo nodo pai do nodoA
        atualiza_pai(nodoB, nodoA);

        // sub2 deve ir pra esquerda de nodoA
        nodoA->esq = sub2;
        if(sub2 != nullptr) sub2->pai = nodoA;

        // nodoA deve ficar à direita de nodoB
        nodoB->dir = nodoA;
        nodoA->pai = nodoB;
    }

    template <typename T> void zag(Nodo<T> * nodoA) {
        //nodoB é o pai do a
        Nodo<T> *nodoB=nodoA->pai;
        //sub2 esta a esquerda do nodoA
        Nodo<T> *sub2=nodoA->esq;

        // Deve-se atualizar o nodo pai do nodoA, o qual
        // deve ser o mesmo nodo pai do nodoB (se nodoB não for a raiz)
        atualiza_pai(nodoA, nodoB);

        //sub2 deve ir para dir do nodoB
        nodoB->dir=sub2;
        if(sub2 != nullptr) sub2->pai=nodoB;
        //nodo B deve ir para esquerda do nodoA
        nodoA->esq=nodoB;
        nodoB->pai=nodoA;

    }

    // testa se o nodo apontado por "ptr" está à esquerda
    // de seu nodo pai
    template <typename T> bool na_esquerda(Nodo<T> * ptr) {
        Nodo<T> * pai = ptr->pai;

        return pai->esq == ptr;
    }

    // ptr: nodo que se deseja transformar na raiz (ou subi-lo
    // de nível)
    template <typename T> bool testa_zig_zig(Nodo<T> * ptr) {
        return na_esquerda(ptr) && na_esquerda(ptr->pai);
    }

    template <typename T> bool testa_zag_zag(Nodo<T> * ptr) {
        return !na_esquerda(ptr) && !na_esquerda(ptr->pai);
    }

    template <typename T> bool testa_zig_zag(Nodo<T> * ptr) {
        return na_esquerda(ptr) && !na_esquerda(ptr->pai);
    }

    template <typename T> bool testa_zag_zig(Nodo<T> * ptr) {
        return !na_esquerda(ptr) && na_esquerda(ptr->pai);
    }

    template <typename T> void move_raiz(Nodo<T> * ptr, Splay<T> & arv) {
        // move o nodo apontado por "ptr" para a raiz

        // enquanto não for a raiz
        while (ptr->pai != nullptr) {
            if(ptr->pai->pai == nullptr) {
                if(na_esquerda(ptr)) zig(ptr);
                else zag(ptr);
            } else {
                if (testa_zig_zig(ptr)) {
                    zig(ptr->pai);
                    zig(ptr);
                } else if (testa_zag_zag(ptr)) {
                    zag(ptr->pai);
                    zag(ptr);
                } else if (testa_zig_zag(ptr)) {
                    zig(ptr);
                    zag(ptr);
                } else if (testa_zag_zig(ptr)) {
                    zag(ptr);
                    zig(ptr);
                }
            }
            // testa qual transformação aplicar, e então
            // a efetua
            // A transformação pode ser simples (zig ou zag)
            // ou uma combinação delas (zig-zig, zag-zag,
            // zig-zag, zag-zig)
        }

        arv.raiz = ptr;
    }
    /*
     * fim das funções privativas
     */

    template <typename T> Splay<T> cria_splay() {
        Splay<T> arv;
        arv.raiz = nullptr;

        return arv;
    }

    template <typename T> void splay_adiciona(Splay<T> & arv, const T & algo) {
        //a função move_raiz não será utilizada ao adicionar os dados na árvore. Logo, os dados já
        //estarão embaralhados
        if (arv.raiz == nullptr) { // está vazia
            arv.raiz = cria_nodo<T>(algo, nullptr);
        } else {
            Nodo<T> * ptr = arv.raiz;

            while (true) {
                if (ptr->dado == algo) {
                    ptr->dado = algo;
                    break;
                } else if (algo < ptr->dado) {
                    // dado a adicionar é menor que o dado do
                    // nodo apontado por ptr
                    if (ptr->esq != nullptr) {
                        ptr = ptr->esq;
                    } else {
                        ptr->esq = cria_nodo<T>(algo, ptr);
                        break;
                    }
                } else {
                    // dado a adicionar é maior que o dado do
                    // nodo apontado por ptr
                    if (ptr->dir != nullptr) {
                        ptr = ptr->dir;
                    } else {
                        ptr->dir = cria_nodo<T>(algo, ptr);
                        break;
                    }
                }
            }

        }
    }


    template <typename T> T& splay_acessa(Splay<T> & arv, const T & algo) {
        Nodo<T> * ptr = arv.raiz;

        while (ptr != nullptr) {
            if (ptr->dado == algo) {
                move_raiz(ptr, arv);
                return ptr->dado;
            }
            if (algo < ptr->dado) ptr = ptr->esq;
            else ptr = ptr->dir;
        }
    }
}

#endif //SPLAY_SPLAY_IMPL_H
