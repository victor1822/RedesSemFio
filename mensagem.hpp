#ifndef MENSAGEM_HPP_INCLUDED
#define MENSAGEM_HPP_INCLUDED

#include <string>


// vamos criar buffer de pacotes sim ou n? Acho mais fácil não kkk (EDIT: Professor pediu, neh?)


class Mensagem{

public:
    Mensagem(string IdDest_, string msg_);
    string IdDest;
    string msg;
};

Mensagem::Mensagem(string IdDest_, string msg_){
    this->IdDest = IdDest_;
    this->msg = msg_;
}


#endif // MENSAGEM_HPP_INCLUDED