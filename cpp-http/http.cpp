#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LISTEN 8080
#define BUFFER 2048

char rec[BUFFER];
static std::string reply = "Hello World";

int main()
{
    int sfd, cfd, client, err;
    struct sockaddr_in addr;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0){
        std::cerr << "failed to create a socket" << std::endl;
        goto err;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(LISTEN);

    cfd = bind(sfd, (struct sockaddr *) &addr, sizeof(addr));
    if(cfd < 0){
        std::cerr << "failed to bind" << std::endl;
        goto err;
    }

    cfd = listen(sfd, 1);
    if(cfd < 0){
        std::cerr << "failed to listen on socket"<< std::endl;
        goto err;
    }

    std::cout << "listening on port: " << LISTEN << std::endl;
    while(true){
        // client = accept(sfd, (struct sockaddr *) &addr, sizeof(addr));
        client = accept(sfd, NULL, 0);
        
        if(client < 0){
            std::cerr << "failed to accept incoming connections" << std::endl;
            goto err;
        }

        read(client, rec, BUFFER);

        err = write(client, reply.c_str(), reply.length());
        if(err < 0){
            std::cerr << "failed to write back" << std::endl;
            goto err;
        }
        std::cout << "successful reply" << std::endl;

        close(client);
    }

    exit(0);
    err:
        exit(-1);
}