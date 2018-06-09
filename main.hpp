#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <glm/glm.hpp> // coordenadas geométricas
#include <stdlib.h>
#include <iostream>
#include <vector> 
#include <stack> // pilha
#include <thread>
#include <string>
#include <climits>
#include <time.h>
#include "mob_programada.hpp"
#include <unistd.h> //int usleep(useconds_t useconds);
#include <mutex> // semáforo



#include "no.hpp"


std::stack<mobilidade_programada> mob_programada; // pilha usada para armazenar a mobilidade programada


void inicializa_tabelas(std::vector<no> &t); // inicia as tabelas de roteamento com métrica 0 para o proprio nó e infinito para os demais
std::mutex mtx_print_tabela; 


void print_tab(int Id, std::vector<tabela> tabela_p_imprimir); // imprime uma tabela de roteamento

void print_vet(std::vector<no> &v){ // imprime a topologia da rede
	std::cout<< "Minha topologia tem " << v.size() << " nós, que estão distribuídos da seguinte forma:"<<std::endl;
	for(int i = 0; i < v.size(); i++){
		no temp;
		glm::vec2 pos_;
		temp = v[i];
		pos_ = temp.get_pos();
		std::cout << "Nó " << i << ", localizado em ("<< pos_.x << "," << pos_.y << "), tem raio de cobertura igual a " << temp.get_ratios()<<std::endl;
}
}

std::vector<no> LoadFile(char **argv){ // lê o arquivo de entrada que contem a disposição dos nós e as futuras mudanças 

unsigned int tam;

std::cin >> tam;

const std::size_t sz = tam;


std::vector<no> topol; // vetor de nós. Chamamos este de topologia da rede.

for(int i=0; i<sz; i++){

float v1=0,v2=0,v3=0;

std::cin>>v1,std::cin>>v2,std::cin>>v3;	// lendo Id e posições

bool busy_tone = false; // faz parte do nosso protocolo busy_tone 

std::vector<tabela> tab;	// vector de tabelas. Na realidade, cada instancia da classe tabela é apenas um registro
std::stack<Mensagem> mens;	// pulha de mensagens.

topol.push_back(no(mens,i,glm::vec2(v1,v2),v3,busy_tone,tab,sz)); // cria nós e adiciona na topoligia da rede
}

std::cin >> tam;
const std::size_t sz2=tam;

for(int i=0; i<sz2; i++){

float v1=0,v2=0,v3=0;

std::cin>>v1,std::cin>>v2,std::cin>>v3;
mobilidade_programada mob_temp=mobilidade_programada((int)v1,glm::vec2(v2,v3));
mob_programada.push(mob_temp);//empilha as mobilidades programadas\\\\

}



inicializa_tabelas(topol);

for(int g = 0; g < topol.size(); g++){
	print_tab(g, topol[g].get_tabela());
}

return topol;
}

void atualiza_conexoes(std::vector<no> &t,bool *m){ // função utilizada para simular ambiente real de transmissão. 
	// a matriz m nos ajuda a sabem quem pode ouvir conexões para evitar inconsistencias na simulação
	
//	while(true){

		for(int i=0;i<t.size();i++){
			for(int j=0;j<t.size();j++){
				int offset = i*t.size()+j;
				if(i==j){
					m[offset] = 1;
				}
				else{
					glm::vec2 p1 = t[i].get_pos();
					float r1 = t[i].get_ratios();
					//r1 = r1*r1;
					glm::vec2 p2 = t[j].get_pos();
					// no seguinte trecho a gente poderia verificar se está havendo mudança na topologia pra daí 
					// exibir no log. Quando a mudança fosse percebida pelo algorítmo de roteamento também(tabelas), 
					// a gente exibiria também; Essas conexões vão nos servir para simular o contexto alcance de sinal
					if(glm::distance(p1,p2)>r1) m[offset] = 0; //fora
					else m[offset] = 1;// dentro			
				}		
			}
		}
	//	usleep(2000); //(useconds_t useconds)
	//}
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
			if(i==j){
				tab_tmp.proximo_salto = i; // destino é ele mesmo
				nst.id = i; 
				nst.value = 1; // numero de sequencia inicia em 1 ṕara o prorio nó
			}else{
				nst.value = 0; // não se tem um número de sequencia
				nst.id = j; 
				tab_tmp.proximo_salto = -1;	// não se sabe a rota ainda, valor negativo
			}
			tab_tmp.numero_de_sequencia = nst;
			if(i!=j)tab_tmp.metrica = INT_MAX;
			else tab_tmp.metrica = 0;
			tab_tmp.tempo_de_registro = 0;
			tabela_temp.push_back(tab_tmp);

		}
			t[i].set_tabela(tabela_temp); // seta uma copia
			tabela_temp.clear();
	}
}


void print_conexoes(bool *m,std::size_t size){ // printa quem alcança quem. 
	// linha alcança coluna se 1
	// se 0, não alcança

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


bool verifica_dump(std::vector<bool> dump){ // verifica se há atualizações parciais a serem enviadas 
	// baseado na ultima transmissão de tabela de roteamento.
	if(dump.empty())return false;
	for(int i = 0; i < dump.size(); i++){
		if(dump[i]==true)return true;
	}
	return false;
}


void vida_de_no(int IdNo, std::vector<no> &t, bool *m, std::vector<unsigned char> &count){
std::cout<<"processando nó "<<IdNo<<std::endl;
	
		// função que cada nó executa a cada interação no laço da main
	
		if(count[IdNo]<5 ){ // envia 5 atualizações parciais caso existam antes de enviar a tabela de atualização inteiroa.
			//isso evita a sobrecarga de menssagens.
			if(verifica_dump(t[IdNo].modificacoes)){
				t[IdNo].envia_broadcast(IdNo, 1, t, m);	// envia atualizações parciais
			}		
			count[IdNo]++;		
		}else{
			count[IdNo]=0;
			t[IdNo].envia_broadcast(IdNo, 0, t, m);  // envia tabela de completa
		}
		//usleep(1000+5*IdNo);



}



// imprime a tabela de roteamento
void print_tab(int Id, std::vector<tabela> tabela_p_imprimir){
	//static std::mutex mtx_print_tabela; 
	mtx_print_tabela.lock();
	std::cout << std::endl << "Tabela de roteamento do nó" << Id << std::endl;


	std::cout<<"|destino|proximo salto|metrica|numero de sequencia ( num ID )|"<<std::endl<<std::endl;
	for(int i = 0; i<tabela_p_imprimir.size(); i++){
	
	tabela_p_imprimir[i].print();
		
	}
	mtx_print_tabela.unlock();
}


#endif // MAIN_HPP_INCLUDED
