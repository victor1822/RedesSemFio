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
	
	
public:
	//std::mutex mtx_tabela; 

	bool busy_tone;
	
	no();

	no(int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_); 

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

	std::vector<tabela> get_tabela_paralela(){
	//	mtx_tabela.lock();
		return Tabela;
	}

	void set_tabela_paralela(std::vector<tabela> tab){
	//	mtx_tabela.lock();
		Tabela = tab;		
	}

	void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id);
	void ouvir_canal();
	void print_tab();


	void vida_de_no(std::string IdNo, std::vector<no> &t, bool *m);
	
};

/*no::no( void ) : //se o nó for inicializado sem atribuir valores, estes serao seus valores padrao
        pos{ glm::vec2(0,0) },
        ratios{ 2.0f }
        Não fazer isso, pois o Id tem que ser inicializado
{}*/

no::no( void ) ://lista de inicialização do nó
	Id{ 0 },
        pos{ glm::vec2(0,0) },
        ratios{ 2.0f },
        busy_tone{ false }
{}

no::no(int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_) : //lista de inicialização do nó
	Id{ Id_ },
        pos{ pos_ },
        ratios{ ratios_ },
        busy_tone{ busy_tone_ },
	Tabela{ tabela_ }
{}

//=============
//no::no( void ) : //se o nó for inicializado sem atribuir valores, estes serao seus valores padrao
  //      pos{ glm::vec2(0,0) },
   //     ratios{ 2.0f }
//{}

//no::no( const glm::vec2 &pos_, const float &ratios_ ) : //lista de inicialização do nó
  //      pos{ pos_ },
   //     ratios{ ratios_ }
//{}
//=============

void no::reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id){ // Richelieu say: Acho que ainda n cobri tudo.
	// SEMAFORO LOCK ?

	for(int i=0;i<tabUpdate.size();i++){
		for(int j=0;j<Tabela.size();j++){

		if(tabUpdate[i].destino==Tabela[j].destino){

			if(tabUpdate[i].numero_de_sequencia.value>Tabela[j].numero_de_sequencia.value){
			
				if(tabUpdate[i].metrica<Tabela[j].metrica){
					Tabela[j].proximo_salto=from_Id;
					//Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					Tabela[j].numero_de_sequencia.value=tabUpdate[i].numero_de_sequencia.value;
					//Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=tabUpdate[i].metrica+1;
					break;
				}else if(tabUpdate[i].metrica == INT_MAX && Tabela[j].proximo_salto== from_Id){
					Tabela[j].proximo_salto=from_Id;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
				//	//Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=INT_MAX;
					break;
				}
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

void no::ouvir_canal(){

	// nessa parte a thread precisa calcular se o nó está ao alcance de outros nós. Só com essa iformação ela ouve o busy tone
	// OPA, TEM A TOPOLOGIA JÁ; CARA COMO TU É GENIAL, Vitor.

	// Pega os nós que conseguem me alançar e ouve o busy tone deles.

	// CASO GENIAL PARA ACONTECER UMA COLISÃO: Não ouço busytone, mas minha transmissão vai atrapalhar alguém por eu ter 
	// um alcance maior. Chupa mundo. Projeto está ganhando detalhes.


}

void no::print_tab(){

	std::cout << std::endl << "Tabela de roteamento do nó "<< Id << std::endl;


	std::cout<<"|destino|proximo salto|metrica|numero de sequencia|"<<std::endl<<std::endl;
	for(int i = 0; i<Tabela.size(); i++){
	
	Tabela[i].print();
		
	}
}

void no::vida_de_no(std::string IdNo, std::vector<no> &t, bool *m){






}



#endif // NO_HPP_INCLUDED
