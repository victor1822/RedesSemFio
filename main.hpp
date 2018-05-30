#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include <unistd.h> //int usleep(useconds_t useconds);
#include <mutex>


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

void atualiza_conexoes(std::vector<no> &t,bool *m){

	for(int i=0;i<t.size();i++){
		for(int j=0;j<t.size();j++){
			int offset = i*t.size()+j;
			if(i==j){
				m[offset] = 1;
			}
			else{
				glm::vec2 p1 = t[i].get_pos();
				float r1 = t[i].get_ratios();
				r1 = r1*r1;
				glm::vec2 p2 = t[j].get_pos();
				if(glm::distance(p1,p2)>r1) m[offset] = 0; //dentro
				else m[offset] = 1;// fora			
			}		
		}
	}
	//return m;
} 

void print_conexoes(bool *m,std::size_t size){

	std::cout<<" |";
	for(int k=0;k<size;k++) std::cout<<k<<"|";
	//stdd::cout<<std::endl;

	for(int i=0;i<size;i++){
		std::cout<<std::endl;
		std::cout<<i<<"|";
		for(int j=0;j<size;j++){
			int offset = i*size+j;
			std::cout<<m[offset]<<"|";
					
		}
	}

}

void vida_de_no(String IdNo, no &t){





}

#endif // MAIN_HPP_INCLUDED
