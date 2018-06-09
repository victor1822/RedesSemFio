#ifndef TABELA_HPP_INCLUDED
#define TABELA_HPP_INCLUDED

class num_seq{
public:
int id;//id do nó que deu origem ao numero de sequencia
unsigned int value;
};


class tabela{ // dicidimos não usar o tempo de instalação devido a complexidade de adicionar esse recurso.
public:
unsigned int destino;
int proximo_salto;
num_seq numero_de_sequencia;//quem atualizou a informação da tabela
int metrica;//numero de saltos
int tempo_de_registro; // não é utilizado!!
void print();
};

void tabela::print(){

std::cout<<"|   "<<destino<<"   |   "<<proximo_salto<<"    |   "<<metrica<<"     |     "<<numero_de_sequencia.value<<" "<<numero_de_sequencia.id<<"      |      "<<std::endl;

}

#endif // TABELA_HPP_INCLUDED
