#include "main.hpp"

int main( int argc, char** argv )
{

std::vector<no> topologia; 
topologia = LoadFile(argv);
print_vet(topologia);

const std::size_t sz = topologia.size();

bool *m = new bool[sz*sz]; // matriz que define conexões físicas

atualiza_conexoes(topologia,m);

print_conexoes(m,topologia.size());

return 0;

}

