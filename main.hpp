#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <climits>
#include <time.h>

#include <unistd.h> //int usleep(useconds_t useconds);
#include <mutex>

#include "no.hpp"

void inicializa_tabelas(std::vector<no> &t);
void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t);

//std::mutex mtx_ouvir_busy_tone; 

void print_vet(std::vector<no> &v){
	std::cout<< "Minha topologia tem " << v.size() << " nós, que estão distribuídos da seguinte forma:"<<std::endl;
	for(int i = 0; i < v.size(); i++){
		no temp;
		glm::vec2 pos_;
		temp = v[i];
		pos_ = temp.get_pos();
		std::cout << "Nó " << i << ", localizado em ("<< pos_.x << "," << pos_.y << "), tem raio de cobertura igual a " << temp.get_ratios()<<std::endl;
}
}

std::vector<no> LoadFile(char **argv){

unsigned int tam;

std::cin >> tam;

const std::size_t sz = tam;

//int *m = new int[sz*sz];
std::vector<no> topol;

for(int i=0; i<sz; i++){

float v1=0,v2=0,v3=0;

std::cin>>v1,std::cin>>v2,std::cin>>v3;

bool busy_tone = false;

std::vector<tabela> tab;
//tabela tt;
//num_seq nst;
//nst.value = 0;
//nsr.id = -1; 

topol.push_back(no(i,glm::vec2(v1,v2),v3,busy_tone,tab));
}

inicializa_tabelas(topol);

for(int g = 0; g < topol.size(); g++){
	topol[g].print_tab();
}

//print_top(topologia,sz);
return topol;
}

void atualiza_conexoes(std::vector<no> &t,bool *m){
	
	while(true){

		for(int i=0;i<t.size();i++){
			for(int j=0;j<t.size();j++){
				int offset = i*t.size()+j;
				if(i==j){
					m[offset] = 1;
				}
				else{
					glm::vec2 p1 = t[i].get_pos();
					float r1 = t[i].get_ratios();
					r1 = r1*r1;
					glm::vec2 p2 = t[j].get_pos();
					// no seguinte trecho a gente poderia verificar se está havendo mudança na topologia pra daí 
					// exibir no log. Quando a mudança fosse percebida pelo algorítmo de roteamento também(tabelas), 
					// a gente exibiria também; Essas conexões vão nos servir para simular o contexto alcance de sinal
					if(glm::distance(p1,p2)>r1) m[offset] = 0; //dentro
					else m[offset] = 1;// fora			
				}		
			}
		}
		usleep(2000); //(useconds_t useconds)
	}
} 


void inicializa_tabelas(std::vector<no> &t){

	tabela tab_tmp;
	num_seq nst;
	nst.value = 0;
	nst.id = -1; 

	for(int i = 0; i < t.size(); i++){

		std::vector<tabela> tabela_temp;
		tabela_temp = t[i].get_tabela();

		for(int j = 0; j < t.size(); j++){

			tab_tmp.destino = j;
			if(i==j)tab_tmp.proximo_salto = i;
			else tab_tmp.proximo_salto = -1;
			tab_tmp.numero_de_sequencia = nst;
			if(i!=j)tab_tmp.metrica = INT_MAX;
			else tab_tmp.metrica = 0;
			tab_tmp.tempo_de_registro = 0;
			tabela_temp.push_back(tab_tmp);

		}
			t[i].set_tabela(tabela_temp);
			tabela_temp.clear();
	}
}


void print_conexoes(bool *m,std::size_t size){

	std::cout<<" |";
	for(int k=0;k<size;k++) std::cout<<k<<"|";

	for(int i=0;i<size;i++){
		std::cout<<std::endl;
		std::cout<<i<<"|";
		for(int j=0;j<size;j++){
			int offset = i*size+j;
			std::cout<<m[offset]<<"|";
					
		}
	}

}

void envia_broadcast(int Id, int type, std::vector<no> &t, bool *m){
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
			}

			mtx_ouvir_busy_tone.unlock();

			std::vector<tabela> my_table=t[Id].get_tabela_paralela();

			for(int i=0;i<me_ouvem.size();i++){

				
				reciveTableUpdate(my_table, Id, me_ouvem[i], t);
				//chama reciveTableUpdate					
				// sucesso

			}

			mtx_ouvir_busy_tone.lock();
			for(int i=0;i<me_alcancam.size();i++){

				t[me_alcancam[i]].busy_tone=true;

			}
			mtx_ouvir_busy_tone.unlock();

	}else if(type==1){

	}


}

void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t){ // Richelieu say: Acho que ainda n cobri tudo.
	// SEMAFORO LOCK ?
	std::vector<tabela> Tabela = t[to_Id].get_tabela();

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



#endif // MAIN_HPP_INCLUDED
