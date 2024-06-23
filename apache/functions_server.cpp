#include <iostream>
#include <memory>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "gen-cpp/ApacheService.h"
#include "gen-cpp/functions_types.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

void PrintMenu()
{
    std::cout
        << "RPC - Escolha o método desejado:" << std::endl
        << "1 - Adição de 8 números (long x 8 /long)" << std::endl
        << "2 - Impressão de 10000 números aleatórios entre 0 e 99 (void/void)"
        << std::endl
        << "3 - Fibonacci (long/long)" << std::endl
        << "4 - Remoção de caracteres diferentes de dígitos (string/string)"
        << std::endl
        << "5 - Criação de usuário (int/Objeto Usuário[id, nome, idade, email])"
        << std::endl
        << "Para invocar o método: ./functions_client [número_metodo] [entradas]"
        << std::endl;
}

std::string RandomName()
{
    std::vector<std::string> names = {"João", "Sávio", "Daniel", "Pedro",
                                      "Yax", "Matheus", "Maria", "Pedrão"};
    return names[rand() % names.size()];
}

std::string RandomAge() { return std::to_string(20 + rand() % 40); }

std::string RandomEmail()
{
    std::vector<std::string> providers = {"gmail.com", "yahoo.com", "usp.br"};
    std::vector<std::string> domains = {"sintesejr", "how2play", "amazon",
                                        "faceitow", "overwatch"};

    std::string username = domains[rand() % domains.size()];
    std::string provider = providers[rand() % providers.size()];

    return username + "@" + provider;
}

class ServicesImpl : public functions::ApacheServiceIf
{
public:
    ServicesImpl() = default;

    int64_t add(const functions::AddRequest &request) override
    {
        int64_t sum = request.num1 + request.num2 + request.num3 +
                      request.num4 + request.num5 + request.num6 +
                      request.num7 + request.num8;
        return sum;
    }

    void printrandom() override
    {
        std::cout << "Random Printer: ";
        for (int i = 0; i < 9999; i++)
            std::cout << rand() % 100 << ", ";
        std::cout << rand() % 100 << std::endl
                  << std::endl;
        PrintMenu();
    }

    int64_t fibonacci(const int64_t input) override
    {
        int64_t number;
        number = input;
        if (number <= 0)
            return 0; // Return 0 for negative input
        else if (number == 1)
            return 1;
        else
        {
            int64_t a = 0, b = 1, c;

            for (int i = 2; i <= number; i++)
            {
                c = a + b;
                a = b;
                b = c;
            }
            return c;
        }
    }

    void onlydigits(std::string &reply, const std::string &input) override
    {
        std::string output;
        for (char c : input)
        {
            if (std::isdigit(c))
            {
                reply += c;
            }
        }
        return;
    }

    void getuser(functions::User &user, const int32_t id) override
    {
        // Simulando uma base de dados com um único usuário
        user.id = id;
        user.name = RandomName();
        user.age = RandomAge();
        user.email = RandomEmail();
    }
};

int main()
{
    const int port = 9090;

    std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    std::shared_ptr<ServicesImpl> services(new ServicesImpl());
    std::shared_ptr<TProcessor> processor(new functions::ApacheServiceProcessor(services));
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

    std::cout << "Starting the server..." << std::endl;
    PrintMenu();
    server.serve();
    std::cout << "Server stopped." << std::endl;

    return 0;
}
