#ifndef MENSAGEM_HPP_INCLUDED
#define MENSAGEM_HPP_INCLUDED

#include <string>


// vamos criar buffer de pacotes sim ou n? Acho mais fácil não kkk (EDIT: Professor pediu, neh?)


class Mensagem{

public:
	Mensagem(int &IdOrig_,int &IdDest_, std::string msg_);
	int IdDest;
	int IdOrig;
	std::string msg;
};

Mensagem::Mensagem(int &IdOrig_,int &IdDest_, std::string msg_):
	IdOrig{ IdOrig_ },
	IdDest{ IdDest_ },
	msg{ msg_ }	
{}


#endif // MENSAGEM_HPP_INCLUDED
