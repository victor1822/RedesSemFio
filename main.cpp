#include "main.hpp"

int main( int argc, char** argv )
{

std::vector<no> topologia; 

topologia = LoadFile(argv);
print_vet(topologia);

const std::size_t sz = topologia.size();

bool *m = new bool[sz*sz]; // matriz que define conexões físicas

std::thread th1 (atualiza_conexoes, topologia,m);
//atualiza_conexoes(topologia,m);
th1.join(); // 	Eita, essa thread tem que ser morta por alguém. 
			//	Quem vai matar ela? Zoa, quando todos os nós da rede estiverem mortos, podemos sair do loop e mata-la.

print_conexoes(m,topologia.size());



return 0;

}

