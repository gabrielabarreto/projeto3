#include <iostream>
#include <fstream>
#include <string>
#include "splay.h"
#include <unordered_map>

using namespace std;
using std::string;
using namespace splay;

struct cadastro {
    string coluna;
    string posicao;
};

bool operator==(const cadastro & a1, const cadastro & a2) {
    return (a1.coluna == a2.coluna);
}

bool operator<(const cadastro & a1, const cadastro   & a2) {
    return (a1.coluna < a2.coluna);
}


int main(int argc, char *argv[]) {
    string colunadigitada, valor, achou, posicaolinha, posicaocoluna, linha, verifica, verifica2;
    int k, posicaonum, pos = 0, pos1;

    ifstream arq(argv[1]);

    //Caso o arquivo não possa ser aberto, o programa retornará uma mensagem de erro
    if (!arq.is_open()) {
        cerr << "ao abrir o arquivo" << endl;
        return errno;
    }

    unordered_map<string, cadastro> dados;

    Splay<string> arv = cria_splay<string>();

    int tamanho = 0, tamanholinha1 = 0, j;
    string coluna, colunausuario, argumentos, conteudo, copia;

    //Cálculo da posição da coluna desejada:
    int posicao=0, contagem = 0;
    string substring;

    cadastro inf;

    //Mensagem de erro caso não seja possível realizar a abertura:
    if (!arq.is_open()) {
        perror("ao abrir o arquivo informado");
        return 0;
    } else {

        //Leitura das colunas passadas como argumento:
        for (j = 2; j < argc; j++) {

            argumentos += argv[j];

            coluna = argv[j];

            ifstream arq(argv[1]);

            //Leitura da primeira linha do arquivo (em que o nome das colunas estão presentes):
            getline(arq, linha);

            //Cálculo da posição de início da próxima linha:
            tamanho = linha.size() + 1;

            tamanholinha1 = tamanho;

            //Localização da coluna desejada no arquivo:
            posicao = linha.find(coluna);

            //Caso a coluna não seja encontrada no arquivo, retornará uma mensagem de erro:
            if (posicao == string::npos) {
                cout << "Não foi possível localizar a coluna informada" << endl;
            }

            //É criada uma substring do ínicio da linha até a posição da coluna informada:
            substring = linha.substr(0, posicao);

            //As vírgulas dessa substring são retiradas e contabilizadas para o cálculo da posição da coluna:
            while (posicao != string::npos) {
                posicao = substring.find(',');
                substring[posicao] = ' ';
                contagem++;
            }

            if (contagem != 0) {
                contagem--;
            }

            //Leitura de todas as linhas do arquivo:
            while (true) {

                getline(arq, linha);

                //Caso o arquivo chegue ao fim, o programa encerra o loop:
                if (arq.eof() || linha=="\r") {
                    break;
                } else {

                    int i, posicao=0, posicao2;
                    string conteudo, copia;

                    copia = linha;

                    if (contagem == 0) {
                        posicao = 0;
                    } else {
                        //Busca da coluna com base no número de vírgulas:
                        for (i = 0; i < contagem; i++) {
                            posicao = copia.find(',');
                            copia[posicao] = ' ';
                        }
                    }

                    //Essa posição é armazenada e é realizada a busca da próxima vírgula:
                    posicao2 = posicao;
                    posicao = copia.find(',');

                    //Caso não seja a última coluna da linha:
                    if (posicao != string::npos) {

                        //Caso seja a primeira coluna:
                        if (posicao2 == 0) {
                            conteudo = linha.substr(0, posicao - posicao2);

                            //Caso seja uma coluna do meio:
                        } else {
                            conteudo = linha.substr(posicao2 + 1, posicao - posicao2 - 1);
                        }

                        //Caso seja a última coluna:
                    } else {

                        //Caso seja a única coluna do arquivo:
                        if (posicao2 == 0) {
                            conteudo = linha.substr(0, linha.size());
                        } else {
                            int pos;

                            pos = linha.find("\r");
                            conteudo = linha.substr(posicao2 + 1, pos - posicao2 -1);
                        }
                    }

                    if (dados.count(conteudo) > 0) {
                        dados[conteudo].coluna += "_" + to_string(contagem);
                        dados[conteudo].posicao += "_" + to_string(tamanho);
                    } else {
                        dados[conteudo].coluna = to_string(contagem);
                        dados[conteudo].posicao = to_string(tamanho);
                        splay_adiciona(arv,conteudo);
                    }

                    //Cálculo da posição da próxima linha:
                    tamanho = linha.size() + tamanho + 1;

                }
            }
        }
    }

    //Loop responsável por implementar o menu
    while (true) {

        cout << "coluna> ";
        getline(cin, colunadigitada);

        //Caso seja digitado ENTER o loop será encerrado e o programa será finalizado
        if (colunadigitada.empty()) {
            break;
        }

        //Armazena o conteúdo presente na linha de comando (exceto nome do arquivo)
        for (k = 2; k < argc; k++) {
            verifica = argv[k];
            verifica2 += verifica + "_";
        }

        //Caso a coluna digitada não tenha sido indexada, o programa informará isso e será encerrado
        if (verifica2.find(colunadigitada) == string::npos) {
            cout << "Coluna digitada não está foi indexada pela linha de comando." << endl;
            break;
        }

        while (true) {
            cout << "valor> ";
            getline(cin, valor);

            //Caso seja digitado ENTER o programa voltará a apresentar o prompt da coluna.
            if (valor.empty())
                break;

            //Loop para buscar as informações na árvore
            try {
                achou = splay_acessa(arv, valor);
            } catch (...) {
                cout << "Conteúdo digitado não foi encontrado na coluna." << endl;
            }

            cadastro mapa;

            //Mapa será uma struct que atribuirá os valores da coluna e posição do valor encontrado
            mapa = dados[achou];

            posicaolinha = mapa.posicao;


            //Caso seja encontrado apenas uma coluna com esta informação
            if (posicaolinha.find("_") == string::npos) {
                posicaonum = stoi(posicaolinha);

                //Será lida a linha do arquivo em que a palavra foi encontrada
                arq.seekg(posicaonum);
                getline(arq, linha);
                cout << linha << endl;

                //Se um dado existir em mais de uma linha ou coluna
            } else {
                while (true) {

                    pos1 = posicaolinha.find("_", pos);

                    if (pos1 == string::npos) {
                        break;
                    }

                    posicaonum = stoi(posicaolinha.substr(pos, pos1));

                    //Apresentará todas as linhas com o valor encontrado, exceto a última
                    arq.seekg(posicaonum);
                    getline(arq, linha);
                    cout << linha << endl;

                    pos = pos1 + 1;
                }

                //Apresentará a última linha que possui o valor encontrado
                pos1 = posicaolinha.size();

                posicaonum = stoi(posicaolinha.substr(pos, pos1));
                arq.seekg(posicaonum);
                getline(arq, linha);
                cout << linha << endl;
            }
        }
    }

    return 0;
}
