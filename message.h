#pragma once
#include <string>
#include <fstream>

class Message
{
    std::string from_;
    std::string to_;
    std::string text_;

public:
    Message() = default;
    Message(const std::string& from, const std::string& to, const std::string& text);

    const std::string& getFrom();
    const std::string& getTo();
    const std::string& getText();

    friend std::ifstream& operator>> (std::ifstream& is, Message& obj) ;
    friend std::ostream& operator<< (std::ostream& os, const Message& obj);
};