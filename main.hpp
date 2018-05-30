#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "no.hpp"

void print_vet(std::vector<no> &v){
	std::cout<< "Minha topologia tem " << v.size() << " nós, que estão distribuídos da seguinte forma:"<<std::endl;
	for(int i = 0; i < v.size(); i++){
		no temp;
		glm::vec2 pos_;
		temp = v[i];
		pos_ = temp.get_pos();
		std::cout << "Nó " << i << ", localizado em ("<< pos_.x << "," << pos_.y << "), tem raio de cobertura igual a " << temp.get_ratios()<<std::endl;
}
}

std::vector<no> LoadFile(char **argv){

unsigned int tam;

std::cin >> tam;

const std::size_t sz = tam;

//int *m = new int[sz*sz];
std::vector<no> topol;

for(int i=0; i<sz; i++){

float v1=0,v2=0,v3=0;

std::cin>>v1,std::cin>>v2,std::cin>>v3;

topol.push_back(no(glm::vec2(v1,v2),v3));
}

//print_top(topologia,sz);
return topol;
} 

#endif // MAIN_HPP_INCLUDED
