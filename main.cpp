#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <pthread.h>

#include "no.hpp"

void print_top(float *topologia, int size){

std::cout<< "Minha topologia tem " << size << " nós, que estão distribuídos da seguinte forma:"<<std::endl;

for(int i = 0; i < size; i++){
int offset = 3*i;
std::cout << "Nó " << i << ", localizado em ("<< topologia[offset] << "," << topologia[offset+1] << "), tem raio de cobertura igual a "<<topologia[offset+2]<<std::endl;
} 

}

void print_vet(vector<no> &v){
	std::cout<< "Minha topologia tem " << v.size() << " nós, que estão distribuídos da seguinte forma:"<<std::endl;
	for(int i = 0; i < v.size(); i++){
		no temp;
		glm::vec2 pos_;
		temp = v[i];
		pos_ = temp.get_pos();
		std::cout << "Nó " << i << ", localizado em ("<< pos_.x << "," << pos_.y << "), tem raio de cobertura igual a " << temp.get_ratios()<<std::endl;
}
}

vector<no> LoadFile(char **argv){

unsigned int tam;

cin >> tam;

const std::size_t sz = tam;

int *m = new int[sz*sz];
float *topologia = new float[sz*3]; //raio, posicao 2D
vector<no> topol;

for(int i=0; i<sz; i++){

float ratios=0,pos=0;
int offset = i*3;

cin >> pos;
topologia[offset] = pos;
cin >> pos;
topologia[offset+1] = pos;
cin >> ratios;
topologia[offset+2] = ratios;
topol.push_back(no(glm::vec2(topologia[offset],topologia[offset+1]),topologia[offset+2]));
}

print_top(topologia,sz);
return topol;
} 

int main( int argc, char** argv )
{

vector<no> topologia; 
topologia = LoadFile(argv);
print_vet(topologia);

return 0;

}

