#ifndef SPLAY_SPLAY_H
#define SPLAY_SPLAY_H

// as declarações da árvores splay ficarão dentro do namespace splay
// elas serão assim acessadas com o prefixo splay::
// ou diretamente, se for usado "using namespace splay"

namespace splay {

    template <typename T> struct Nodo {
        Nodo<T> *esq, *dir, *pai;
        T dado;
    };

    template <typename T> struct Splay {
        Nodo<T> * raiz;
    };

    // cria uma árvore splay vazia
    template <typename T> Splay<T> cria_splay();

    // adiciona "algo" à árvore.
    // ao final, esse novo valor se tornará a nova raiz da árvore
    template <typename T> void splay_adiciona(Splay<T> & arv, const T & algo);

    // procura o dado que é igual a "algo", e retorna uma cópia
    // se não o encontrar, dispara uma exceção
    template <typename T> T& splay_acessa(Splay<T> & arv, const T & algo);

    template <typename T> T move_raiz();

}
#endif //SPLAY_SPLAY_H

#include "splay-impl.h"

