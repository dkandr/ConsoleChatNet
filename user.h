#pragma once
#include <string>
#include <fstream>

class User
{
    std::string login_;
    std::string name_;
    std::string password_;
 
public:
    User() = default;
    User(const std::string& login, const std::string& name, const std::string& password);
        
    const std::string& getLogin();
    // void setLogin(const std::string& login);
 
    const std::string& getName();
    // void setName(const std::string& name);

    const std::string& getPassword();
    // void setPassword(const std::string& password);

    friend std::ifstream& operator>> (std::ifstream& is, User& obj);
    friend std::ostream& operator<< (std::ostream& os, const User& obj);
};