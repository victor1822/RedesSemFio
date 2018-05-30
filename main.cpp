#include "main.hpp"

int main( int argc, char** argv )
{

std::vector<no> topologia; 
topologia = LoadFile(argv);
print_vet(topologia);

const std::size_t sz = topologia.size();

int *m = new int[sz*sz]; // matriz que define conexões físicas

return 0;

}

