#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "user.h"
#include "message.h"

#define PORT 34567

enum PacketType
{
    ptLogin,
    ptSignIn,
    ptAddMessage,
    ptGetMessages,
    ptExit
};

void show_error_and_exit();

void write_string(int socket_file_descriptor, const std::string& str);
void read_string(int socket_file_descriptor, std::string& str);

void write_result(int socket_file_descriptor, bool result);
bool read_result(int socket_file_descriptor);

void write_user(int socket_file_descriptor, User& user);
void read_user(int socket_file_descriptor, User& user);

void write_message(int socket_file_descriptor, Message& message);
void read_message(int socket_file_descriptor, Message& message);