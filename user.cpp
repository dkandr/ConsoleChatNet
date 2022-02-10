#include "user.h"

User::User(const std::string &login, const std::string &name, const std::string &password)
        : login_(login), name_(name), password_(password) {} 

const std::string& User::getLogin()
{ 
    return login_;
}

const std::string& User::getName()
{
    return name_; 
}

const std::string& User::getPassword() 
{
    return password_;
}

std::ifstream& operator>> (std::ifstream& is, User& obj) 
{
    std::getline(is, obj.login_);
    std::getline(is, obj.name_);
    std::getline(is, obj.password_);
    return is;
}

std::ostream& operator<< (std::ostream& os, const User& obj) 
{
    os << obj.login_ << std::endl;
    os << obj.name_ << std::endl;
    os << obj.password_ << std::endl;
    return os;
}