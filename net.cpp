#include <iostream>
#include <cstring>

#include "net.h"

void show_error_and_exit()
{
    std::cerr << "error : " << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

void write_string(int socket_file_descriptor, const std::string& str)
{
    ssize_t bytes;
    int str_size = str.size();
    bytes = write(socket_file_descriptor, (char*)&str_size, sizeof(int));
    if(bytes == -1)
        show_error_and_exit();

    bytes = write(socket_file_descriptor, str.c_str(), str_size);
    if(bytes == -1)
        show_error_and_exit();
}

void read_string(int socket_file_descriptor, std::string& str)
{
    ssize_t bytes;
    int str_size;
    bytes = read(socket_file_descriptor, (char*)&str_size, sizeof(int));
    if(bytes == -1)
        show_error_and_exit();

    char *msg = new char[str_size + 1];
    msg[str_size] = '\0';
    bytes = read(socket_file_descriptor, msg, str_size);
    if(bytes == -1)
        show_error_and_exit();

    str = msg;
    delete[] msg;
}

void write_result(int socket_file_descriptor, bool result)
{
    ssize_t bytes = write(socket_file_descriptor, (char*)&result, sizeof(bool));
    if(bytes == -1)
        show_error_and_exit();
}

bool read_result(int socket_file_descriptor)
{
    bool result;
    ssize_t bytes = read(socket_file_descriptor, (char*)&result, sizeof(bool));
    if(bytes == -1)
        show_error_and_exit();

    return result;
}

void write_user(int socket_file_descriptor, User& user)
{
    write_string(socket_file_descriptor, user.getLogin());
    write_string(socket_file_descriptor, user.getName());
    write_string(socket_file_descriptor, user.getPassword());
}

void read_user(int socket_file_descriptor, User& user)
{
    std::string login, name, password;
    read_string(socket_file_descriptor, login);
    read_string(socket_file_descriptor, name);
    read_string(socket_file_descriptor, password);
    user = {login, name, password};
}

void write_message(int socket_file_descriptor, Message& message)
{
    write_string(socket_file_descriptor, message.getFrom());
    write_string(socket_file_descriptor, message.getTo());
    write_string(socket_file_descriptor, message.getText());
}

void read_message(int socket_file_descriptor, Message& message)
{
    std::string from, to, text;
    read_string(socket_file_descriptor, from);
    read_string(socket_file_descriptor, to);
    read_string(socket_file_descriptor, text);
    message = {from, to, text};
}