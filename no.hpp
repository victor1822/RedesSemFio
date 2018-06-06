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
	//std::vector<bool> modificacoes;
	
public:
	//std::mutex mtx_tabela; 
	std::vector<bool> modificacoes;
	bool busy_tone;
	
	no();

	no(int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_,const std::size_t &sz); 

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
		Tabela[Id].numero_de_sequencia.value++;
		return Tabela;
	}

	void set_tabela_paralela(std::vector<tabela> tab){
	//	mtx_tabela.lock();
		Tabela = tab;		
	}

	//void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id);
	void ouvir_canal();
	static void print_tab(int Id, int from_Id, std::vector<tabela> tabela_p_imprimir);

	static void envia_broadcast(int Id,int type, std::vector<no> &t, bool *m);
	static void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t);

	static void print_tab_this(int Id, std::vector<tabela> tabela_p_imprimir);
	//static void vida_de_no(int IdNo, std::vector<no> &t, bool *m);
	
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

no::no(int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_,const std::size_t &sz) : //lista de inicialização do nó
	Id{ Id_ },
        pos{ pos_ },
        ratios{ ratios_ },
        busy_tone{ busy_tone_ },
	Tabela{ tabela_ }
{

	
	for(int f = 0; f < sz; f++){
		modificacoes.push_back(true);
	}


}

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



void no::ouvir_canal(){

	// nessa parte a thread precisa calcular se o nó está ao alcance de outros nós. Só com essa iformação ela ouve o busy tone
	// OPA, TEM A TOPOLOGIA JÁ; CARA COMO TU É GENIAL, Vitor.

	// Pega os nós que conseguem me alançar e ouve o busy tone deles.

	// CASO GENIAL PARA ACONTECER UMA COLISÃO: Não ouço busytone, mas minha transmissão vai atrapalhar alguém por eu ter 
	// um alcance maior. Chupa mundo. Projeto está ganhando detalhes.


}

void no::print_tab(int Id, int from_Id, std::vector<tabela> tabela_p_imprimir){
	static std::mutex mtx_print_tabela; 
	mtx_print_tabela.lock();
	std::cout << std::endl << "Tabela de roteamento do nó "<< Id << "após receber do nó "<< from_Id << std::endl;


	std::cout<<"| destino|proximo salto|metrica|numero de sequencia (num ID) |"<<std::endl<<std::endl;
	for(int i = 0; i<tabela_p_imprimir.size(); i++){
	
	tabela_p_imprimir[i].print();
		
	}
	mtx_print_tabela.unlock();
}

void no::envia_broadcast(int Id, int type, std::vector<no> &t, bool *m){
	

	static std::mutex mtx_ouvir_busy_tone; 

	int size = t[Id].get_tabela().size();

	if(type==0){ // atualização completa da tabela de roteamento. (full dump)

	
		std::vector<int> me_ouvem;


			for(int j=0;j<size;j++){
				int offset = Id*size+j;
				if(m[offset]==1&&j!=Id){

					me_ouvem.push_back(j);

				}
			}
		
		std::vector<int> me_alcancam;

			for(int i=0;i<me_ouvem.size();i++){
				
				int offset = me_ouvem[i]*size+Id;

				if(m[offset]==1 && i!=Id){

					me_alcancam.push_back(i);

				}
			}

			mtx_ouvir_busy_tone.lock();
			bool all_fine=true;

			for(int i=0;i<me_alcancam.size() && all_fine;i++){

				all_fine=!t[me_alcancam[i]].busy_tone;

			}
			if(all_fine){

				for(int i=0;i<me_alcancam.size();i++){

					t[me_alcancam[i]].busy_tone=true;

				}
			}else{
				mtx_ouvir_busy_tone.unlock();
				return;
			}

			mtx_ouvir_busy_tone.unlock();

			std::vector<tabela> my_table=t[Id].get_tabela_paralela();
			//my_table[Id].numero_de_sequencia.value++;
			print_tab_this(Id,t[Id].get_tabela());

			for(int i=0;i<me_ouvem.size();i++){

				

				reciveTableUpdate(my_table, Id, me_ouvem[i], t);
				//chama reciveTableUpdate					
				// sucesso

			}

			mtx_ouvir_busy_tone.lock();
			for(int i=0;i<me_alcancam.size();i++){

				t[me_alcancam[i]].busy_tone=false;

			}
			mtx_ouvir_busy_tone.unlock();

	}else if(type==1){

		std::vector<int> me_ouvem;


			for(int j=0;j<size;j++){
				int offset = Id*size+j;
				if(m[offset]==1&&j!=Id){

					me_ouvem.push_back(j);

				}
			}
		
		std::vector<int> me_alcancam;

			for(int i=0;i<me_ouvem.size();i++){
				
				int offset = me_ouvem[i]*size+Id;

				if(m[offset]==1 && i!=Id){

					me_alcancam.push_back(i);

				}
			}

			mtx_ouvir_busy_tone.lock();
			bool all_fine=true;

			for(int i=0;i<me_alcancam.size() && all_fine;i++){

				all_fine=!t[me_alcancam[i]].busy_tone;

			}
			if(all_fine){

				for(int i=0;i<me_alcancam.size();i++){

					t[me_alcancam[i]].busy_tone=true;

				}
			}else{
				mtx_ouvir_busy_tone.unlock();
				return;
			}

			mtx_ouvir_busy_tone.unlock();

			std::vector<tabela> my_table_changed;
			std::vector<tabela> tab_temp=t[Id].get_tabela();
			
			std::vector<bool> temp_mod;
			temp_mod=t[Id].modificacoes;
			
			for(int i=0;i<size;i++){
				if(temp_mod[i]){
					my_table_changed.push_back(tab_temp[i]);	
				}
			}

			print_tab_this(Id,my_table_changed);
			//my_table[Id].numero_de_sequencia.value++;
			for(int i=0;i<me_ouvem.size();i++){

				
				
				reciveTableUpdate(my_table_changed, Id, me_ouvem[i], t);
				//chama reciveTableUpdate					
				// sucesso

			}

			for(int i=0;i<size;i++){
				temp_mod[i]=false;
			}
			t[Id].modificacoes=temp_mod;
			mtx_ouvir_busy_tone.lock();
			

			for(int i=0;i<me_alcancam.size();i++){

				t[me_alcancam[i]].busy_tone=false;

			}
			mtx_ouvir_busy_tone.unlock();

	}


}

void no::reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t){ // Richelieu say: Acho que ainda n cobri tudo.
	// SEMAFORO LOCK ?

	//print_tab_this(from_Id,tabUpdate);

	std::vector<tabela> Tabela = t[to_Id].get_tabela();
	std::vector<bool> mod_temp=t[to_Id].modificacoes;

	for(int i=0;i<tabUpdate.size();i++){
		for(int j=0;j<Tabela.size();j++){

		if(tabUpdate[i].destino==Tabela[j].destino){

			if(tabUpdate[i].numero_de_sequencia.value>Tabela[j].numero_de_sequencia.value ){
			
				if(tabUpdate[i].metrica<Tabela[j].metrica){
					
					mod_temp[j]=true;
					
					Tabela[j].proximo_salto=from_Id;
					//Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
			//Tabela[j].numero_de_sequencia.value=tabUpdate[i].numero_de_sequencia.value;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					//Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=tabUpdate[i].metrica+1;
					break;
				}else if(Tabela[j].proximo_salto== from_Id){
					mod_temp[j]=true;
					Tabela[j].proximo_salto=from_Id;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
				//	//Tabela[j].tempo_de_registro=timeOS_.now();
					if(tabUpdate[i].metrica == INT_MAX){
						Tabela[j].metrica=INT_MAX;	
						Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					}else{
						Tabela[j].metrica=tabUpdate[i].metrica+1;
						Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					}
					
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
	t[to_Id].set_tabela(Tabela);
	t[to_Id].modificacoes=mod_temp;
	no::print_tab(to_Id,from_Id, Tabela);
	// SEMAFORO UNLOCK ?
}

void no::print_tab_this(int Id, std::vector<tabela> tabela_p_imprimir){
//	static std::mutex mtx_print_tabela; 
//	mtx_print_tabela.lock();
	std::cout << std::endl << "Tabela de roteamento do nó "<< Id << std::endl;


	std::cout<<"|destino|proximo salto|metrica|numero de sequencia ( num ID )|"<<std::endl<<std::endl;
	for(int i = 0; i<tabela_p_imprimir.size(); i++){
	
	tabela_p_imprimir[i].print();
		
	}
//	mtx_print_tabela.unlock();
}




#endif // NO_HPP_INCLUDED
