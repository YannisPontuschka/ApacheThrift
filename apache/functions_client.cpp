#include <iostream>
#include <memory>
#include <chrono>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/ApacheService.h"
#include "gen-cpp/functions_types.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace std::chrono;

bool ParseArgumentsAdd(char **argv, int64_t *a, int64_t *b, int64_t *c,
                       int64_t *d, int64_t *e, int64_t *f, int64_t *g,
                       int64_t *h)
{
    try
    {
        *a = std::stoll(argv[2]);
        *b = std::stoll(argv[3]);
        *c = std::stoll(argv[4]);
        *d = std::stoll(argv[5]);
        *e = std::stoll(argv[6]);
        *f = std::stoll(argv[7]);
        *g = std::stoll(argv[8]);
        *h = std::stoll(argv[9]);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Entrada inválida: " << e.what() << std::endl;
        return false;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Fora do escopo: " << e.what() << std::endl;
        return false;
    }
    return true;
}

functions::AddRequest configureAddInput(char **argv)
{
    int64_t a, b, c, d, e, f, g, h;
    functions::AddRequest request;

    if (ParseArgumentsAdd(argv, &a, &b, &c, &d, &e, &f, &g, &h) == false)
        return request;
    request.num1 = a;
    request.num2 = b;
    request.num3 = c;
    request.num4 = d;
    request.num5 = e;
    request.num6 = f;
    request.num7 = g;
    request.num8 = h;
    return request;
}

void ExecuteChosen(int argc, char **argv, std::shared_ptr<TTransport> transport,
                   functions::ApacheServiceClient client,
                   std::string *output)
{
    try
    {
        transport->open();

        if (std::strcmp(argv[1], "1") == 0)
        {
            if (argc != 10)
            {
                std::cerr << "Uso: " << argv[0]
                          << " 1 <int64> <int64> <int64> <int64> <int64> <int64> <int64> "
                             "<int64>"
                          << std::endl;
            }
            functions::AddRequest request = configureAddInput(argv);
            *output = std::to_string(client.add(request));
        }
        else if (std::strcmp(argv[1], "2") == 0)
        {
            client.printrandom();
            *output = "No servidor";
        }
        else if (std::strcmp(argv[1], "3") == 0)
        {
            if (argc != 3)
            {
                std::cerr << "Uso: " << argv[0] << " 3 <int64_t>" << std::endl;
                return;
            }
            int64_t input = std::stoi(argv[2]);
            *output = std::to_string(client.fibonacci(input));
        }
        else if (std::strcmp(argv[1], "4") == 0)
        {
            if (argc != 3)
            {
                std::cerr << "Uso: " << argv[0] << " 4 <string>" << std::endl;
            }
            std::string input = argv[2];
            client.onlydigits(*output, input);
        }
        else if (std::strcmp(argv[1], "5") == 0)
        {
            if (argc != 3)
            {
                std::cerr << "Uso: " << argv[0] << " 5 <int32>" << std::endl;
            }
            functions::User user;
            int32_t input = std::stoi(argv[2]);
            client.getuser(user, input);
            *output = "\nUSUÁRIO CRIADO:\nid: " + std::to_string(user.id) + "\nnome: " + user.name +
                      "\nidade: " + user.age + "\nemail: " + user.email;
        }
        else
        {
            std::cerr << "Método inválido." << std::endl;
        }
        transport->close();
        return;
    }
    catch (TException &tx)
    {
        std::cerr << "ERROR: " << tx.what() << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    functions::ApacheServiceClient client(protocol);
    std::string output;

    auto start = high_resolution_clock::now();

    ExecuteChosen(argc, argv, transport, client, &output);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Apresenta tempo gasto e a saída do RPC.
    std::cout << "Saída: " << output << std::endl;
    std::cout << "Tempo de duração: " << duration.count() << " ms" << std::endl;
    return 0;
}
