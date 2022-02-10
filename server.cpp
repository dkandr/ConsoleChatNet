#include "net.h"

std::vector<User> users;
std::vector<Message> messages;
User currentUser;

void login(int connection);
void signIn(int connection);

void sendMessages(int connection);
void addMessage(int connection);

bool existsUserWithLogin(const std::string &login);
bool setCurrentUser(User& u);

int main()
{
    // users.push_back(User{"1", "1", "1"});
    // messages.push_back(Message{"1", "2", "text 1 2"});
    // messages.push_back(Message{"2", "3", "text 2 3"});
    // messages.push_back(Message{"3", "1", "text 3 1"});

    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
        show_error_and_exit();

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    int bind_status = bind(socket_file_descriptor, (struct sockaddr*)&addr, sizeof(addr));
    if(bind_status == -1)
        show_error_and_exit();

    int listen_status = listen(socket_file_descriptor, SOMAXCONN);
    if (listen_status == -1)
        show_error_and_exit();

    bool isServer = true;
    
    while(isServer)
    {
        sockaddr_in client;
        socklen_t client_length = sizeof(client);

        int connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &client_length);
        if (connection == -1)
            show_error_and_exit();

        bool isSession = true;

        while(isSession)
        {
            PacketType packetType;        
            ssize_t bytes = read(connection, (char *)&packetType, sizeof(packetType));
            
            //ошибка получения данных
            if(bytes == -1)
                show_error_and_exit();
            
            //передача закончена (клиент разорвал сессию)
            if(bytes == 0)
            {
                isSession = false;
                close(connection);
                continue;
            }
            
            switch (packetType)
            {
                case ptLogin:
                    login(connection);
                    break;
                case ptSignIn:
                    signIn(connection);
                    break;
                case ptGetMessages:
                    sendMessages(connection);
                    break;
                case ptAddMessage:
                    addMessage(connection);
                    break;
                case ptExit:
                    isServer = false;
                    break;
                default:
                {
                    //закрываем сервер, если неизвестное сообщение
                    std::cerr << "error: unknow message type." << std::endl;
                    isServer = false;
                    break;
                }
            }        
        }
    }

    close(socket_file_descriptor);
    return 0;
}

void login(int connection)
{
    read_user(connection, currentUser);

    bool res = setCurrentUser(currentUser);
    write_result(connection, res);
 
    if (res)
    {
        write_user(connection, currentUser);
    }
}

void signIn(int connection)
{
    read_user(connection, currentUser);

    bool existsLogin = existsUserWithLogin(currentUser.getLogin());
    write_result(connection, !existsLogin);
 
    if (!existsLogin)
    {
        users.push_back(currentUser);
    }
}

void sendMessages(int connection)
{
    std::vector<Message> send_messages;

    for (auto &message : messages)
    {
        if (message.getFrom() == currentUser.getName() || message.getTo() == currentUser.getName() || message.getTo().empty())
            send_messages.push_back(message);
    }

    int message_count = send_messages.size();
    ssize_t bytes = write(connection, (char*)&message_count, sizeof(int));
    if(bytes == -1)
        show_error_and_exit();

    for (auto &message : send_messages)
    {
        write_message(connection, message);
    }    
}

void addMessage(int connection)
{
    Message message;
    read_message(connection, message);
    messages.push_back(message);
}

bool existsUserWithLogin(const std::string& login)
{
    for (auto &user : users)
    {
        if (login == user.getLogin())
            return true;
    }

    return false;
}

bool setCurrentUser(User& u)
{
    for (auto &user : users)
    {
        if (u.getLogin() == user.getLogin() && u.getPassword() == user.getPassword())
        {
            currentUser = user;
            return true;
        }
    }
    return false;
}