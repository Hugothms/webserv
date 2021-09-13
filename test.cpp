#define PORT "4950"
#define STDIN 0

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <unistd.h>
# include <fcntl.h>
# include <ctime>
//NETWORK INCLUDES
# include <netinet/in.h>

//NEW

#include <arpa/inet.h>
#include <sys/select.h>



struct sockaddr name;

void set_nonblock(int socket) {
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int agrc, char** argv) {
    int status, sock, adrlen, new_sd;

    struct addrinfo hints;
    struct addrinfo *servinfo;  //will point to the results

    //store the connecting address and size
    struct sockaddr_storage their_addr;
    socklen_t their_addr_size;

    fd_set read_flags,write_flags; // the flag sets to be used
    struct timeval waitd = {10, 0};          // the max wait time for an event
    int sel;                      // holds return value for select();


    //socket infoS
    memset(&hints, 0, sizeof hints); //make sure the struct is empty
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //tcp
    hints.ai_flags = AI_PASSIVE;     //use local-host address

    //get server info, put into servinfo
    if ((status = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    //make socket
    sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sock < 0) {
        printf("\nserver socket failure %m", errno);
        exit(1);
    }

    //allow reuse of port
    int yes=1;
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    //unlink and bind
    unlink("127.0.0.1");
    if(bind (sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        printf("\nBind error %m", errno);
        exit(1);
    }

    freeaddrinfo(servinfo);

    //listen
    if(listen(sock, 5) < 0) {
        printf("\nListen error %m", errno);
        exit(1);
    }
    their_addr_size = sizeof(their_addr);

    //accept
    new_sd = accept(sock, (struct sockaddr*)&their_addr, &their_addr_size);
    if( new_sd < 0) {
        printf("\nAccept error %m", errno);
        exit(1);
    }

    //set non blocking
    set_nonblock(new_sd);
    std::cout<<"\nSuccessful Connection!\n";

    char in[255];
    char out[255];
    memset(&in, 0, 255);
    memset(&out, 0, 255);
    int numSent;
    int numRead;

    while(1) {

        FD_ZERO(&read_flags);
        FD_ZERO(&write_flags);
        FD_SET(new_sd, &read_flags);
        FD_SET(new_sd, &write_flags);
        FD_SET(STDIN_FILENO, &read_flags);
        FD_SET(STDIN_FILENO, &write_flags);

        sel = select(new_sd+1, &read_flags, &write_flags, (fd_set*)0, &waitd);

        //if an error with select
        if(sel < 0)
            continue;

        //socket ready for reading
        if(FD_ISSET(new_sd, &read_flags)) {

            //clear set
            FD_CLR(new_sd, &read_flags);

            memset(&in, 0, 255);

            numRead = recv(new_sd, in, 255, 0);
            if(numRead <= 0) {
                printf("\nClosing socket");
                close(new_sd);
                break;
            }
            else if(in[0] != '\0')
                std::cout<<"\nClient: "<<in;

        }   //end if ready for read

        //if stdin is ready to be read
        if(FD_ISSET(STDIN_FILENO, &read_flags))
            fgets(out, 255, stdin);


        //socket ready for writing
        if(FD_ISSET(new_sd, &write_flags)) {
            //printf("\nSocket ready for write");
            FD_CLR(new_sd, &write_flags);
            send(new_sd, out, 255, 0);
            memset(&out, 0, 255);
        }   //end if
    }   //end while

   std::cout<<"\n\nExiting normally\n";
    return 0;
}