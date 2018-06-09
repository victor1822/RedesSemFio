#ifndef MOB_PROGR_HPP_INCLUDED
#define MOB_PROGR_HPP_INCLUDED


class mobilidade_programada{


private:

	glm::vec2 pos;
	int Id;

public:

	

	mobilidade_programada(int Id_, const glm::vec2 &pos_); 

	glm::vec2 get_pos(){

		return pos;
	}


	int get_Id(){

		return Id;	

	}


};




mobilidade_programada::mobilidade_programada(int Id_, const glm::vec2 &pos_) :
	Id{ Id_ },
    pos{ pos_ }    
{}



#endif