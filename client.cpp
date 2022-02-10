#include "net.h"

void showLoginMenu(int socket_file_descriptor);
void showUserMenu(int socket_file_descriptor);
void login(int socket_file_descriptor);
void signIn(int socket_file_descriptor);
void ShowMessages(int socket_file_descriptor);
void addMessage(int socket_file_descriptor);

User currentUser;

int main()
{
    // Создадим сокет
    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
        show_error_and_exit();
 
    // Установим адрес сервера
    sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    addr.sin_port = htons(PORT);
    // Используем IPv4
    addr.sin_family = AF_INET;
    // Установим соединение с сервером
    int connection = connect(socket_file_descriptor, (struct sockaddr*)&addr, sizeof(addr));
    if(connection == -1)
        show_error_and_exit();
    
    showLoginMenu(socket_file_descriptor);
 
    close(socket_file_descriptor);

    return 0;
}

void showLoginMenu(int socket_file_descriptor)
{
    bool isClient = true;
    while (isClient)
    {
        std::cout << "1. Login" << std::endl;
        std::cout << "2. SignIn" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "4. Exit and close server" << std::endl;
        std::cout << ">> ";
        char operation;
        std::cin >> operation;

        PacketType packetType;
        ssize_t bytes;

        switch (operation)
        {
            case '1':
                login(socket_file_descriptor);
                break;
            case '2':
                signIn(socket_file_descriptor);
                break;
            case '3':
                isClient = false;
                break;
            case '4':
                isClient = false;
                packetType = ptExit;
                bytes = write(socket_file_descriptor, (char *)&packetType, sizeof(packetType));
                if(bytes == -1)
                    show_error_and_exit();
                break;
            default:
                std::cout << "Unknow choice." << std::endl;
                break;
        }
   }
}

void showUserMenu(int socket_file_descriptor)
{
    char operation;

    do
    {
        std::cout << "1. Show messages" << std::endl;
        std::cout << "2. Add message" << std::endl;
        std::cout << "3. Logout" << std::endl;
        std::cout << ">> ";
        std::cin >> operation;

        switch (operation)
        {
            case '1':
                ShowMessages(socket_file_descriptor);
                break;
            case '2':
                addMessage(socket_file_descriptor);
                break;
            case '3':
                break;
            default:
                std::cout << "Unknow choice." << std::endl;
                break;
        }
    } while (operation != '3');
}

void login(int socket_file_descriptor)
{
    std::string login, password;

    bool isLogin = false;

    do
    {
        std::cout << "login: ";
        std::cin >> login;
        std::cout << "password: ";
        std::cin >> password;

        currentUser = {login, "", password};

        PacketType packetType = ptLogin;
        ssize_t bytes = write(socket_file_descriptor, (char *)&packetType, sizeof(packetType));
        if(bytes == -1)
            show_error_and_exit();
 
        write_user(socket_file_descriptor, currentUser);

        bool result = read_result(socket_file_descriptor);

        if (result)
        {
            read_user(socket_file_descriptor, currentUser);
            isLogin = true;
        }
        else
        {
            std::cout << "login failed..." << std::endl;
        }

    } while(!isLogin);

    showUserMenu(socket_file_descriptor);
}

void signIn(int socket_file_descriptor)
{
    std::string login, name, password;

    bool isLogin = false;

    do
    {
        std::cout << "login: ";
        std::cin >> login;
        std::cout << "name: ";
        std::cin >> name;
        std::cout << "password: ";
        std::cin >> password;

        currentUser = {login, name, password};

        PacketType packetType = ptSignIn;
        ssize_t bytes = write(socket_file_descriptor, (char *)&packetType, sizeof(packetType));
        if(bytes == -1)
            show_error_and_exit();
 
        write_user(socket_file_descriptor, currentUser);

        bool result = read_result(socket_file_descriptor);

        if (result)
        {
            isLogin = true;
        }
        else
        {
            std::cout << "SignIn failed..." << std::endl;
        }

    } while(!isLogin);

    showUserMenu(socket_file_descriptor);
}

void ShowMessages(int socket_file_descriptor)
{
    PacketType packetType = ptGetMessages;
    ssize_t bytes = write(socket_file_descriptor, (char *)&packetType, sizeof(packetType));
    if(bytes == -1)
        show_error_and_exit();

    std::vector<Message> messages;

    int message_count;
    bytes = read(socket_file_descriptor, (char*)&message_count, sizeof(int));
    if(bytes == -1)
        show_error_and_exit();

    for(int i = 0; i < message_count; ++i)
    {
        Message message;
        read_message(socket_file_descriptor, message);
        messages.push_back(message);
    }

    for (auto &message : messages)
    {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "From : " << message.getFrom() << std::endl;
        std::cout << "To   : " << message.getTo() << std::endl;
        std::cout << "Text : " << message.getText() << std::endl;
    }
}

void addMessage(int socket_file_descriptor)
{
    PacketType packetType = ptAddMessage;
    ssize_t bytes = write(socket_file_descriptor, (char *)&packetType, sizeof(packetType));
    if(bytes == -1)
        show_error_and_exit();

    Message message;
    std::string to, text;

    std::cout << "To (name or all): ";
    std::cin.ignore();
    getline(std::cin, to);
    std::cout << "Text: ";
    std::cin.ignore();
    getline(std::cin, text);

    if (to == "all")
    {
        to.clear();
    }

    message = {currentUser.getName(), to, text};
    
    write_message(socket_file_descriptor, message);
}