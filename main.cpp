#include "main.hpp"

int main( int argc, char** argv )
{

std::vector<no> topologia; // para fins de execução do programa, nós adicionamos todos os nós a esse vector para acesso mais fácil.

std::vector<std::string> msgs; //  conjunto de menssagens que podem ser enviadas ao longo da execução do programa.

msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("O que?");
msgs.push_back("Isso mesmo!");
msgs.push_back("Aham, claudia, senta lá");
msgs.push_back("Opa, amigão");
msgs.push_back("QUEEEEEE?");
msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("Boa noite");
 

topologia = LoadFile(argv); // carrega a toponogia e as mobilidades programadas.
print_vet(topologia); // imrpime a topologia da rede.

const std::size_t sz = topologia.size();

bool *m = new bool[sz*sz]; // matriz que representa o alcance de sinal de cada nós

//std::thread th1(atualiza_conexoes,std::ref(topologia),m);
std::cout<<"antes de atualiza conexoes"<<std::endl;

atualiza_conexoes(topologia,m); // pega as posições do nó e calcula o alcance de cada.

//th1.join(); // Essa thread daria um realismo a mais, porém nossos conchecimentos em c++ não nos
//permitiu fazer o uso de multithreads rodando em paralelo no mesmo código.
std::cout<<"antes de print conexões"<<std::endl;

print_conexoes(m,topologia.size()); // imprime a topologia da rede

std::vector<unsigned char> count; // contador usado para saber quantas atualizações parciais foram enviadas por cada nó

for(int i=0;i<sz;i++){
	count.push_back(0);
}

while(true){

		bool TF = (rand() % 100) < 30;
		if(TF){ 
			int var;
			var = (int)rand()%msgs.size();// var = (int)rand()%8
			int origem = rand()%topologia.size();
			int destino = origem;
		
			while(destino==origem) // evita que a mensagem gerada seja para o próprio nó
			destino = rand()%topologia.size();

			std::string mensagem = msgs[var]; // escolhe uma menssagem

			static const Mensagem mm = Mensagem(origem,destino,mensagem);
			std::cout<<" No "<<origem<<" quer enviar a mensagem '"<<mensagem<<"' para o no"<<destino<<std::endl;
			topologia[origem].buffer.push(mm); // coloca mensagem no buffer do nó de origem.
			
		}

		bool TF2 = (rand() % 100) < 5; // baixa probablididade para mobilidade
		if(TF2 && !mob_programada.empty()){ // caso a pilha ainda não esteja vazia
			
			mobilidade_programada mob_pro_aux= mob_programada.top(); 

			glm::vec2 aux_pos = mob_pro_aux.get_pos();
			

			topologia[mob_pro_aux.get_Id()].set_no(aux_pos);
			mob_programada.pop();
			std::cout<<" No "<<mob_pro_aux.get_Id()<<" agora está na posição ("<< aux_pos.x << "," << aux_pos.y << "),"<<std::endl;
			
			atualiza_conexoes(topologia,m); // atualizo a matriz que indica os nós estao ao alcance físico de outros.
			print_vet(topologia); // iḿprimo a topologia
			print_conexoes(m,topologia.size()); // impromo a matriz de alcances.
		}



		for(int i=0;i<topologia.size();i++){ // processo todos os nós 
			std::thread th_no(vida_de_no,i,std::ref(topologia),m, std::ref(count));	
			th_no.join(); 	
		}
//std::thread th_no0(vida_de_no,0,std::ref(topologia),m);
//std::thread th_no1(vida_de_no,1,std::ref(topologia),m);
//std::thread th_no2(vida_de_no,2,std::ref(topologia),m);
//std::cout<<"Ei"<<std::endl;
//th_no0.join(); 
//th_no1.join(); 
//th_no2.join(); 
		std::cout<<"_____________"<<std::endl;
}
return 0;

}

