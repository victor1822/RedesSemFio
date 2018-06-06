#include "main.hpp"

int main( int argc, char** argv )
{

//atexit(exitfunc);

std::vector<no> topologia;

std::vector<std::string> msgs;
msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("O que?");
msgs.push_back("Isso mesmo!");
msgs.push_back("Aham, claudia, senta lá");
msgs.push_back("Opa, amigão");
msgs.push_back("QUEEEEEE?");
msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("Fernando, esses meninos merecem 10");
msgs.push_back("Boa noite");
 

topologia = LoadFile(argv);
print_vet(topologia);

const std::size_t sz = topologia.size();

bool *m = new bool[sz*sz]; // matriz que define conexões físicas

std::thread th1(atualiza_conexoes,std::ref(topologia),m);
std::cout<<"antes de atualiza conexoes"<<std::endl;
atualiza_conexoes(topologia,m);
//th1.join(); // 	Eita, essa thread tem que ser morta por alguém. 
			//	Quem vai matar ela? Zoa, quando todos os nós da rede estiverem mortos, podemos sair do loop e mata-la.
std::cout<<"antes de print conexões"<<std::endl;
print_conexoes(m,topologia.size());

std::vector<unsigned char> count;

for(int i=0;i<sz;i++){
	count.push_back(0);
}

while(true){

		bool TF = (rand() % 100) < 30;
		if(TF){ 
			static int var = 0;
			if(var ==10) var = 0;
			int origem = rand()%topologia.size();
			int destino = origem;
		
			while(destino==origem)
			destino = rand()%topologia.size();

			std::string mensagem = msgs[var];

			static const Mensagem mm = Mensagem(origem,destino,mensagem);
			std::cout<<" No "<<origem<<" quer enviar a mensagem '"<<mensagem<<"' para o no"<<destino<<std::endl;
			topologia[origem].buffer.push(mm);
			var++;
		}

std::cout<<"inicio do laço"<<std::endl;

for(int i=0;i<topologia.size();i++){
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
std::cout<<"eU"<<std::endl;
}
return 0;

}

