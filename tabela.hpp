#ifndef TABELA_HPP_INCLUDED
#define TABELA_HPP_INCLUDED

#define INFINITO 1000
class num_seq{
public:
unsigned int id;//id do nó que deu origem ao numero de sequencia
unsigned int value;
};

//num_seq::num_seq( void ) : id{ 0 },value{ 0 }{}

//num_seq::num_seq( unsigned int &id_, unsigned int &value_ ) :	id{ id_ },value{ value_ }{}

class tabela{
public:
unsigned int destino;
int proximo_salto;
num_seq numero_de_sequencia;//quem atualizou a informação da tabela
int metrica;//numero de saltos
int tempo_de_registro;
};

//tabela::tabela( void ) : destino{ 0 },proximo_salto{ -1 },numero_de_sequencia{ num_seq(0,0) },metrica{ -1 },tempo_de_registro{ 0 }{}

//tabela::tabela( unsigned int &dst,int &ps,num_seq &num_seq_, int &met, int &tdr ) :
//destino{ dst },	proximo_salto{ ps },numero_de_sequencia{ num_seq_ },metrica{ met },tempo_de_registro{ tdr }{}

#endif // TABELA_HPP_INCLUDED
