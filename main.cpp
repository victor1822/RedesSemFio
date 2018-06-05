#include "main.hpp"

void exit_program( void ){

	std::terminate();//kill whatever process
	std::clog << "Exiting...\n";

}

int main( int argc, char** argv )
{

atexit(exit_program); //call_back para terminar qualquer thread em execução

std::vector<no> topologia; 

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
std::cout<<"depois das conexões"<<std::endl;


for(int i=0;i<topologia.size();i++){

	topologia[i].dispara_thread(i,std::ref(topologia), m);

}

//std::thread th_no0(vida_de_no,0,std::ref(topologia),m);
//std::thread th_no1(vida_de_no,0,std::ref(topologia),m);

//std::cout<<"Ei"<<std::endl;
//th_no0.join(); 
//th_no1.join(); 
//std::cout<<"eU"<<std::endl;


return 0;

}

