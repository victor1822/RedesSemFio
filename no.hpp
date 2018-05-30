#ifndef NO_HPP_INCLUDED
#define NO_HPP_INCLUDED

#include "tabela.hpp"

class no{

private:

	glm::vec2 pos;
	float ratios;
	std::vector<tabela> Tabela;

public:

	no();

	no(const glm::vec2 &pos_, const float &ratios_ );

	void set_no(float a, float b){

		pos = glm::vec2(a,b);
	}
	
	glm::vec2 get_pos(){

		return pos;
	}

	float get_ratios(){

		return ratios;	

	}

	std::vector<tabela> get_tabela(){
		return Tabela;
	}

	void set_tabela(std::vector<tabela> tab){
		Tabela = tab;		
	}
};

no::no( void ) : //se o nó for inicializado sem atribuir valores, estes serao seus valores padrao
        pos{ glm::vec2(0,0) },
        ratios{ 2.0f }
{}

no::no( const glm::vec2 &pos_, const float &ratios_ ) : //lista de inicialização do nó
        pos{ pos_ },
        ratios{ ratios_ }
{}

#endif // NO_HPP_INCLUDED
