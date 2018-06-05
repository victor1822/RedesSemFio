#include "main.hpp"

int main( int argc, char** argv )
{

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


while(true){


std::cout<<"inicio do laço"<<std::endl;

for(int i=0;i<topologia.size();i++){
	std::thread th_no(vida_de_no,i,std::ref(topologia),m);	
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

