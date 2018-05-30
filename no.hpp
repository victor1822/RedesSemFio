#ifndef NO_HPP_INCLUDED
#define NO_HPP_INCLUDED

#include "tabela.hpp"
#include "mensagem.hpp"

class no{

private:

	glm::vec2 pos;
	float ratios; // Quanto maior disparidades houverem, pior a eficiencia da rede. 
	// Terminal "exposto" e "oculto" são atenuados.
	int Id;
	std::vector<tabela> Tabela;
	bool busy_tone;
	
public:

	no();

	no(int Id_, const glm::vec2 &pos_, const float &ratios_ );

	void set_no(float a, float b){

		pos = glm::vec2(a,b);
	}
	
	glm::vec2 get_pos(){

		return pos;
	}

	float get_ratios(){

		return ratios;	

	}

	int get_Id(){

		return Id;	

	}

	std::vector<tabela> get_tabela(){
		return Tabela;
	}

	void set_tabela(std::vector<tabela> tab){
		Tabela = tab;		
	}
};

/*no::no( void ) : //se o nó for inicializado sem atribuir valores, estes serao seus valores padrao
        pos{ glm::vec2(0,0) },
        ratios{ 2.0f }
        Não fazer isso, pois o Id tem que ser inicializado
{}*/

no::no(int Id_, const glm::vec2 &pos_, const float &ratios_ ) : //lista de inicialização do nó
        pos{ pos_ },
        ratios{ ratios_ }
        Id{ Id_ }
        busy_tone{ false }
{}

no::reciveTableUpdate(std::vector<tabela> tabUpdate){ // Richelieu say: Acho que ainda n cobri tudo.
	// SEMAFORO LOCK ?

	for(int i=0;i<tabUpdate.size();i++){
		for(int j=0;j<Tabela.size();j++){

			if(tabUpdate[i].destino==Tabela[j].destino){
				if(tabUpdate[i].metrica<Tabela[j].metrica){
					Tabela[j].proximo_salto=tabUpdate[i].proximo_salto;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=tabUpdate[i].metrica+1;
					break;
				}else if(tabUpdate[i].metrica == INFINITO){
					Tabela[j].proximo_salto=tabUpdate[i].proximo_salto;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=tabUpdate[i].INFINITO;
					break;
				}
			}
/*
unsigned int destino;
int proximo_salto;
num_seq numero_de_sequencia;//quem atualizou a informação da tabela
int metrica;//numero de saltos
int tempo_de_registro;
*/
		}
	}

	// SEMAFORO UNLOCK ?
}

no::ovir_canal(){

	// nessa parte a thread precisa calcular se o nó está ao alcance de outros nós. Só com essa iformação ela ouve o busy tone
	// OPA, TEM A TOPOLOGIA JÁ; CARA COMO TU É GENIAL, Vitor.

	// Pega os nós que conseguem me alançar e ouve o busy tone deles.

	// CASO GENIAL PARA ACONTECER UMA COLISÃO: Não ouço busytone, mas minha transmissão vai atrapalhar alguém por eu ter 
	// um alcance maior. Chupa mundo. Projeto está ganhando detalhes.


}



#endif // NO_HPP_INCLUDED
