#ifndef NO_HPP_INCLUDED
#define NO_HPP_INCLUDED

#include "tabela.hpp"
#include "mensagem.hpp"

char * string_to_bits(std::string mensagem){
	return(char*) mensagem.c_str();
}

std::string bits_to_string(char * arr){
	std::string str(arr);
	return str;
}


class no{

private:

	glm::vec2 pos;
	float ratios; // Quanto maior disparidades houverem, pior a eficiencia da rede. 
	// Terminal "exposto" e "oculto" são atenuados.
	int Id;// ID no nõ para efeito de manupulação do projeto
	std::vector<tabela> Tabela; // tabela de roteamento
	
	
public:
	//std::mutex mtx_tabela; 

	std::stack<Mensagem> buffer;  // buffer de mensagens (repasse e leitura tbm)
	std::vector<bool> modificacoes; // Flags para indicar quais os registros que sofreram modificações desde a ultima transmissão da tabela de roteamento completa
	bool busy_tone; // busy_tone do nó. 
	
	no();

	no(std::stack<Mensagem> buffer_, int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_,const std::size_t &sz); 

	void set_no(glm::vec2 pos_){ // posso setar nova posição para um nó (mobilidade)

		pos = pos_;
	}
	
	glm::vec2 get_pos(){ // pego a posição do nó

		return pos;
	}

	float get_ratios(){ // raio do nó

		return ratios;	

	}

	int get_Id(){ // Id do no

		return Id;	

	}

	std::vector<tabela> get_tabela(){ // retorna cópia da tabela de roteamento do nó
		
		return Tabela;
	}

	void set_tabela(std::vector<tabela> tab){ // seta uma tabela de roteamento para o nó
		
		Tabela = tab;		
	}

	std::vector<tabela> get_tabela_paralela(){ // intrementa o número de sequencia da propria rota antes de retornar uma cópia
	//	mtx_tabela.lock();
		Tabela[Id].numero_de_sequencia.value++;
		return Tabela;
	}

	void set_tabela_paralela(std::vector<tabela> tab){ // caso fossem usado esquema multithread, seria necessário adicionar esse semáforo
	//	mtx_tabela.lock();
		Tabela = tab;		
	}

	void ouvir_canal();
	static void print_tab(int Id, int from_Id, std::vector<tabela> tabela_p_imprimir);

	static void envia_broadcast(int Id,int type, std::vector<no> &t, bool *m); // função responsável por fazer o papel de transmĩssão broadcast
	static void reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t); // função responsável por fazer o papel de receptora de trasmissão

	static void print_tab_this(int Id, std::vector<tabela> tabela_p_imprimir); //mais uma função para imprimir tabela de roteamento.
	
	
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

no::no(std::stack<Mensagem> buffer_, int Id_, const glm::vec2 &pos_, const float &ratios_, bool& busy_tone_, std::vector<tabela> &tabela_,const std::size_t &sz) : //lista de inicialização do nó
	buffer{ buffer_ },
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

	std::vector<int> me_ouvem;
	std::vector<int> me_alcancam;


	int size = t[Id].get_tabela().size();

	if(type==0){ // atualização completa da tabela de roteamento. (full dump)

	
		


			for(int j=0;j<size;j++){
				int offset = Id*size+j;
				if(m[offset]==1&&j!=Id){
// em um senário real, eu envio sinal e recebe quem ouve p sinal.
// nesse caso de simulação, eu apreciso saber a disposição geometrica dos
// nós para saber quem conseguiria ouvir a transmissão;
					me_ouvem.push_back(j); 

				}
			}
		

			for(int i=0;i<me_ouvem.size();i++){
				
				int offset = me_ouvem[i]*size+Id;

				if(m[offset]==1 && i!=Id){
// Essa informação eu preciso para saber se eu consigo ouvir o busy_tone dos nós que me escutam.
// Pensamos em permitir raios de ancances diferentes para diferentes nós.
					me_alcancam.push_back(i);

				}
			}

			mtx_ouvir_busy_tone.lock();
			bool all_fine=true;

			for(int i=0;i<me_alcancam.size() && all_fine;i++){

				all_fine=!t[me_alcancam[i]].busy_tone;

			}
			if(all_fine){ // se nenhum nó que me alcança estiver com o busy tone ativo, 
				//eu começo a transmissão.

				for(int i=0;i<me_alcancam.size();i++){

					t[me_alcancam[i]].busy_tone=true;

				}
			}else{
				// se algum nó que me ouve e eu ouço ele estiver com o busy_tone ativo, eu não transmito nada.
				mtx_ouvir_busy_tone.unlock();
				return;
			}

			mtx_ouvir_busy_tone.unlock();

			std::vector<tabela> my_table=t[Id].get_tabela_paralela(); //incremento meu número de sequencia antes de fazer uma copia da tabela.
			
			//my_table[Id].numero_de_sequencia.value++;
			
			print_tab_this(Id,t[Id].get_tabela()); // imprimo a tabela que vai ser transmitida

			for(int i=0;i<me_ouvem.size();i++){

				reciveTableUpdate(my_table, Id, me_ouvem[i], t);  // Como nosso esquema é sequencial,
				// adotamos a lógica de "entregar a transmissão" ao nó receptor. Como o busy_tone já tinha sido reservado, 
				// essa parte não burla o esquema de transmissão.

			}


	}else if(type==1){

			for(int j=0;j<size;j++){
				int offset = Id*size+j;
				if(m[offset]==1&&j!=Id){

					me_ouvem.push_back(j);

				}
			}

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
					// se tal registro sofreu alguma alteração desde a ultima full dump, envio o mesmo.
					my_table_changed.push_back(tab_temp[i]);	
				}
			}

			print_tab_this(Id,my_table_changed); // imprime a tabela de atualização incremental
			
			for(int i=0;i<me_ouvem.size();i++){

				reciveTableUpdate(my_table_changed, Id, me_ouvem[i], t); //envia só a atualização incremental
			
			}

			for(int i=0;i<size;i++){
				temp_mod[i]=false; // controle de modificações dos registros.
				//nesse ponto eu informo que não há nenhuma modificação na tabela de roteamento do nó que não tenha sido transmitida por broadcast.
			}
			t[Id].modificacoes=temp_mod; // faço a copia da tabela de volta pro nó 
			
			

	}

	bool redirect = !t[Id].buffer.empty();

	//nesse ponto acontece as retransmissões de menssagens.

	if(redirect){
		Mensagem mm = t[Id].buffer.top();
		t[Id].buffer.pop();
		if(Id != mm.IdDest){ // se o destino não for o prório nó
			std::vector<tabela> table = t[Id].get_tabela();
			int dst = table[mm.IdDest].proximo_salto;
			if(dst>=0){ // se eu tiver rota para o nó
	
				//t[dst].buffer.push(mm);
				std::cout<<"O no "<<Id<<" esta repassando a mensagem '"<<mm.msg<<"' enviada pelo no "<<mm.IdOrig<<" destinada para o no "<<mm.IdDest<<", mas vai passar pelo o no "<<dst<<std::endl;
				int offset =Id*size+mm.IdDest;
				if(m[offset]){ // se o proximo salto para o destino estiver ao meu alcance, eu envop ap buffer dele
					t[dst].buffer.push(mm); // repassando menssagem para buffer do nó do proximo salto
				}else{
					//caso não esteja mais, a menssagem se perderá no ar! Ninguem vai repassar

				}
			}
			else{// caso eu não tenha rota para o nó
			std::cout<<"O no "<<Id<<" nao conseguiu rota para o destino"<<std::endl;
				}
		}else{// caso o destino seja o prório nó Id:
			std::cout<<"O no "<<Id<<" recebeu a mensagem '"<<mm.msg<<"' enviada pelo no "<<mm.IdOrig<<" e eh o destino final da mensagem"<<std::endl;
		}
	}


	mtx_ouvir_busy_tone.lock();
	for(int i=0;i<me_alcancam.size();i++){
		t[me_alcancam[i]].busy_tone=false;
	}
	mtx_ouvir_busy_tone.unlock();


}

void no::reciveTableUpdate(std::vector<tabela> tabUpdate, int from_Id, int to_Id, std::vector<no> &t){ // Richelieu say: Acho que ainda n cobri tudo.
	// SEMAFORO LOCK ?

	//print_tab_this(from_Id,tabUpdate);

	std::vector<tabela> Tabela = t[to_Id].get_tabela();
	std::vector<bool> mod_temp=t[to_Id].modificacoes; // NESSA matriz eu faço o controle dos registros que sofreram modificações na atualização

	for(int i=0;i<tabUpdate.size();i++){ // quantidade de registros recebida
		for(int j=0;j<Tabela.size();j++){ // numero de registros da minha tabela de roteamento
// lógica da atualização das rotas nas tabelas de roteamento. Numério de sequencia, métrica, proximo salto.. 
			// detalhes são analizados aqui.
			// tempo de instalação não foi um item analisádo, pois elevaria a complexidade de implementação; 
			// o entendimento do uso prático do tempo de instalação não foi localizado por nós.
			if(tabUpdate[i].destino==Tabela[j].destino){

				if(tabUpdate[i].numero_de_sequencia.value>Tabela[j].numero_de_sequencia.value ){
		
					mod_temp[j]=true;
					
					Tabela[j].proximo_salto=from_Id;
					Tabela[j].numero_de_sequencia=tabUpdate[i].numero_de_sequencia;
					//Tabela[j].tempo_de_registro=timeOS_.now();
					Tabela[j].metrica=tabUpdate[i].metrica+1;
			
					break;
				}else if(tabUpdate[i].numero_de_sequencia.value==Tabela[j].numero_de_sequencia.value ){
					if(tabUpdate[i].metrica<Tabela[j].metrica){
					
						mod_temp[j]=true;
					
						Tabela[j].proximo_salto=from_Id;
						Tabela[j].metrica=tabUpdate[i].metrica+1;
						break;
					}
		
				}
			}
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
	std::cout << std::endl << "*Tabela de roteamento do nó "<< Id << std::endl;


	std::cout<<"|destino|proximo salto|metrica|numero de sequencia ( num ID )|"<<std::endl<<std::endl;
	for(int i = 0; i<tabela_p_imprimir.size(); i++){
	
	tabela_p_imprimir[i].print();
		
	}
//	mtx_print_tabela.unlock();
}





#endif // NO_HPP_INCLUDED
