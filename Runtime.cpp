//
// Created by VSharma on 1/5/2018.
//
#include "Runtime.h"
#include <android/log.h>

#define APPNAME "MyNativeApp"

Runtime *Runtime::getInstance() {
    static Runtime runtime_;
    return &runtime_;
}

void Runtime::serverThreadFunction() {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::serverThreadFunction() start");
    mainLoopThread();
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::serverThreadFunction() end");
}

void Runtime::clientThreadFunction() {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::clientThreadFunction() start");
    initMultiCastTimeSocket("224.244.224.245", 30490);
    while (!stopThreads) {
        sendPeriodicMessages();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::clientThreadFunction() end");
}

bool Runtime::init(std::string destinationAddress) {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::init() start");
    destinationAddress_ = destinationAddress;
    m_serverThread = std::thread(&Runtime::serverThreadFunction, this);
    m_clientThread = std::thread(&Runtime::clientThreadFunction, this);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::init() end");
    return true;
}

bool Runtime::deInit() {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::deInit() start");
    stopThreads = true;
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::deInit() end");
    return true;
}

void Runtime::setDestinationAddress(std::string destinationAddress) {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::setDestinationAddress() start");
    destinationAddress_ = destinationAddress;
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::setDestinationAddress() end");
}

bool
Runtime::sendMultiCastMessage(std::string broadCastAddress, int portNumber, std::string message) {
    struct sockaddr_in multiAddress;
    int multiFd;

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendMultiCastMessage() start, broadcast address: %s, portNumber: %d",
                        broadCastAddress.c_str(), portNumber);

    /* set up socket */
    multiFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (multiFd < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendMultiCastMessage() function, after socket, error: %s",
                            strerror(errno));
        return false;
    }

    bzero((char *) &multiAddress, sizeof(multiAddress));
    multiAddress.sin_family = AF_INET;
    multiAddress.sin_port = htons(portNumber);
    multiAddress.sin_addr.s_addr = inet_addr(broadCastAddress.c_str());
    socklen_t addressLength = sizeof(multiAddress);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendMultiCastMessage(), sending multiCast message: %s",
                        message.c_str());
    ssize_t cnt = sendto(multiFd, message.c_str(), message.length(), 0,
                         (struct sockaddr *) &multiAddress, addressLength);
    if (cnt < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendMultiCastMessage() function, sendto() failed, error: %s",
                            strerror(errno));
        return false;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::sendMultiCastMessage() multicast data sent successfully");
    }
    close(multiFd);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendMultiCastMessage() end");
    return true;
}

bool Runtime::sendTcpMessage(std::string destinationAddress, int portNumber, std::string message) {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendTcpMessage() start, destination: %s, portNumber: %d",
                        destinationAddress.c_str(), portNumber);
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), socket() call failed, error: %s",
                            strerror(errno));
        return false;
    }

    // set sock file descriptor as NON BLOCKING
    int flags = fcntl(sock, F_GETFL);
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), fcntl() call failed, error: %s",
                            strerror(errno));
        close(sock);
        return false;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNumber);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendTcpMessage() Test1");

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, destinationAddress.c_str(), &serv_addr.sin_addr) <= 0) {
        // Invalid address or Address not supported
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), inet_pton() call failed, error: %s",
                            strerror(errno));
        close(sock);
        return false;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendTcpMessage() Test2");
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        if (errno == EINPROGRESS) {
            struct pollfd fds[5];
            memset(fds, 0, sizeof(fds));
            fds[0].fd = sock;
            fds[0].events = POLLOUT;
            nfds_t numberOfFds = 1;
            int result = poll(fds, numberOfFds, 5000);
            if (result < 0) {
                __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                    "error Inside Runtime::sendTcpMessage(), poll() system call failed, error: %s",
                                    strerror(errno));
                close(sock);
                return false;
            } else if (result == 0) {
                __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                    "error Inside Runtime::sendTcpMessage(), timeout happened on poll()");
                close(sock);
                return false;
            } else {
                __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                    "Runtime::sendTcpMessage(), TCP connection created successfully");
            }
        } else {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "error Inside Runtime::sendTcpMessage(), connect() call failed, error: %s",
                                strerror(errno));
            close(sock);
            return false;
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendTcpMessage() Test3");
    if (send(sock, message.c_str(), message.length(), 0) <= 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), send() call failed, error: %s",
                            strerror(errno));
        close(sock);
        return false;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendTcpMessage() Test4");
    if (read(sock, buffer, 1024) <= 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), read() call failed, error: %s",
                            strerror(errno));
        close(sock);
        return false;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::sendTcpMessage(), received message %s", buffer);
    }

    close(sock);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendTcpMessage() end");
    return true;
}

bool Runtime::sendUdpMessage(std::string destinationAddress, int portNumber, std::string message) {
    int sockfd;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    //char buffer[1024];
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendUdpMessage() start, destination: %s, portNumber: %d",
                        destinationAddress.c_str(), portNumber);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendUdpMessage(), socket() call failed, error: %s",
                            strerror(errno));
        return false;
    }

    // set sock file descriptor as NON BLOCKING
    int flags = fcntl(sockfd, F_GETFL);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendTcpMessage(), fcntl() call failed, error: %s",
                            strerror(errno));
        close(sockfd);
        return false;
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(destinationAddress.c_str());
    if (server == NULL) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendUdpMessage(), gethostbyname() call returned NULL, error: %s",
                            strerror(errno));
        return false;
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *) &serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portNumber);

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    ssize_t sentLength = sendto(sockfd, message.c_str(), message.length(), 0,
                                (const sockaddr *) &serveraddr,
                                serverlen);
    if (sentLength <= 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendUdpMessage(), sendto() call failed, error: %s",
                            strerror(errno));
        return false;
    }

    /* print the server's reply */
    /*if (recvfrom(sockfd, buffer, strlen(buffer), 0, (sockaddr *) &serveraddr, &serverlen) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendUdpMessage(), recvfrom() call failed, error: %s",
                            strerror(errno));
        return false;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::sendUdpMessage(), received message: %s", buffer);
    }*/
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::sendUdpMessage() end");
    return true;
}

bool Runtime::initMultiCastTimeSocket(std::string multiCastAddress, int portNumber) {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::initMulticastUdpConnection() start");
    /* set up socket */
    multiCastSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (multiCastSocketFd < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::initMulticastUdpConnection() function, after socket, error: %s",
                            strerror(errno));
        return false;
    }
    bzero((char *) &multiCastAddress_, sizeof(multiCastAddress_));
    multiCastAddress_.sin_family = AF_INET;
    multiCastAddress_.sin_port = htons(portNumber);
    multiCastAddress_.sin_addr.s_addr = inet_addr(multiCastAddress.c_str());
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::initMulticastUdpConnection() end");
    return true;
}

bool Runtime::sendMultiCastTimeMessage() {
    char message[100];
    time_t t = time(0);
    socklen_t addressLength = sizeof(multiCastAddress_);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendMultiCastMessage() start");
    sprintf(message, "time of android target is %-24.24s", ctime(&t));
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::sendMultiCastMessage(), sending multiCast message: %s",
                        message);
    ssize_t cnt = sendto(multiCastSocketFd, message, sizeof(message), 0,
                         (struct sockaddr *) &multiCastAddress_, addressLength);
    if (cnt < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::sendMultiCastMessage() function, sendto() failed, error: %s",
                            strerror(errno));
        return false;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::sendMultiCastMessage() multicast data sent successfully");
    }
    return true;
}


int Runtime::createMasterTcpSocket(int portNumber) {
    int opt = 1;
    int masterTcpSocket;
    struct sockaddr_in tcpServerAddress;

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::createMasterTcpSocket() start");

    //create a master socket
    if ((masterTcpSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterTcpSocket() function, socket() failed for TCP, error: %s",
                            strerror(errno));
        return 0;
    }

    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if (setsockopt(masterTcpSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterTcpSocket() function, setsockopt() failed for TCP, error: %s",
                            strerror(errno));
        return 0;
    }

    //type of socket created
    tcpServerAddress.sin_family = AF_INET;
    tcpServerAddress.sin_addr.s_addr = INADDR_ANY;
    tcpServerAddress.sin_port = htons(portNumber);

    //bind the socket to given localhost port
    if (bind(masterTcpSocket, (struct sockaddr *) &tcpServerAddress, sizeof(tcpServerAddress)) <
        0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterTcpSocket() function, bind failed for TCP socket, error: %s",
                            strerror(errno));
        return 0;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::createMasterTcpSocket() listening on TCP port %d", portNumber);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(masterTcpSocket, 3) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterTcpSocket() function, listen failed for TCP socket, error: %s",
                            strerror(errno));
        return 0;
    }
    return masterTcpSocket;
}

int Runtime::createMasterUdpSocket(int portNumber) {
    struct sockaddr_in udpServerAddress;
    int masterUdpSocket;
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::createMasterUdpSocket() start");

    /* create a UDP socket */
    if ((masterUdpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterUdpSocket() function, after socket, error: %s",
                            strerror(errno));
        return 0;
    }

    /* bind the socket to any valid IP address and a specific port */
    memset((char *) &udpServerAddress, 0, sizeof(udpServerAddress));
    udpServerAddress.sin_family = AF_INET;
    udpServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    udpServerAddress.sin_port = htons(portNumber);

    if (bind(masterUdpSocket, (struct sockaddr *) &udpServerAddress, sizeof(udpServerAddress)) <
        0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMasterUdpSocket() function, bind failed, error: %s",
                            strerror(errno));
        return 0;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::createMasterUdpSocket() end");
    return masterUdpSocket;
}

int Runtime::createMultiCastReceiverSocket(std::string multiCastAddress, int portNumber) {
    struct sockaddr_in addr;
    int fd;
    struct ip_mreq mreq;
    u_int yes = 1;

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::createMultiCastReceiverSocket() start");
    /* create what looks like an ordinary UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMultiCastReceiverSocket() function, socket() failed, error: %s",
                            strerror(errno));
        return 0;
    }

    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMultiCastReceiverSocket() function, setsockopt() failed, error: %s",
                            strerror(errno));
        return 0;
    }

    /* set up destination address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port = htons(portNumber);

    /* bind to receive address */
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMultiCastReceiverSocket() function, bind() failed, error: %s",
                            strerror(errno));
        return 0;
    }

    /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr = inet_addr(multiCastAddress.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::createMultiCastReceiverSocket() function, setsockopt() failed, error: %s",
                            strerror(errno));
        return 0;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::createMultiCastReceiverSocket() end");
    return fd;
}

bool Runtime::mainLoopThread() {
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::mainLoopThread() start");

    int masterTcpSocket = createMasterTcpSocket(30509);
    int masterUdpSocket = createMasterUdpSocket(30499);
    int multiCastReceiverSocket = createMultiCastReceiverSocket("224.244.224.246", 30491);

    struct pollfd pfd1;
    pfd1.fd = masterTcpSocket;
    pfd1.revents = 0;
    pfd1.events = POLL_IN;
    managedPollFds.push_back(pfd1);
    handlersMap[masterTcpSocket] = std::bind(&Runtime::handleTcpConnectionRequest, this,
                                             placeholders::_1);

    struct pollfd pfd2;
    pfd2.fd = masterUdpSocket;
    pfd2.revents = 0;
    pfd2.events = POLL_IN;
    managedPollFds.push_back(pfd2);
    handlersMap[masterUdpSocket] = std::bind(&Runtime::handleUdpData, this,
                                             placeholders::_1);

    struct pollfd pfd3;
    pfd3.fd = multiCastReceiverSocket;
    pfd3.revents = 0;
    pfd3.events = POLL_IN;
    managedPollFds.push_back(pfd3);
    handlersMap[multiCastReceiverSocket] = std::bind(&Runtime::handleMultiCastData, this,
                                                     placeholders::_1);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::mainLoopThread(), Waiting for TCP and UDP connections");
    while (!managedPollFds.empty()) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            " MainLoop::mainLoopThread blocked on polling");
        int numReadyFileDescriptors = ::poll(&(managedPollFds[0]),
                                             (nfds_t) managedPollFds.size(),
                                             10000); //10s timeout for polling
        if (numReadyFileDescriptors > 0) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                " MainLoop::mainLoopThread number of fd's ready: %d",
                                numReadyFileDescriptors);
            for (auto managedPollFd = managedPollFds.begin(); managedPollFd
                                                              !=
                                                              managedPollFds.end(); ++managedPollFd) {
                auto watchIterator = handlersMap.find(managedPollFd->fd);
                if (watchIterator != handlersMap.end() && managedPollFd->revents) {

                    if (managedPollFd->revents & POLLIN) {
                        bool status = watchIterator->second(managedPollFd->fd);
                        if (status) {
                            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                                "Runtime::mainLoopThread() message handled successfully");
                            managedPollFd->revents = 0;
                        } else {
                            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                                "Runtime::mainLoopThread() message handling failed, removing Fd from the list");
                            managedPollFd = managedPollFds.erase(managedPollFd);
                            handlersMap.erase(watchIterator);
                            break;
                        }
                    } else {
                        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                            "Runtime::mainLoopThread() event other than POLLIN happened on fd");
                    }

                } else if (managedPollFd->revents) {
                    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                        " MainLoop::mainLoopThread no handler found for fd %d",
                                        managedPollFd->fd);
                }
            }
        } else if (numReadyFileDescriptors == 0) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                " MainLoop::mainLoopThread poll timed out");
        } else {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                " MainLoop::mainLoopThread poll error errno: %s",
                                strerror(errno));
        }

        if (pendingPollFds.size() > 0) {
            for (auto pollFd : pendingPollFds) {
                managedPollFds.push_back(pollFd);
            }
            pendingPollFds.clear();
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::mainLoopThread() end");
    return true;
}

bool Runtime::handleTcpConnectionRequest(int fd) {
    int new_socket;
    struct sockaddr_in clientAddress;
    int addrlen = sizeof(clientAddress);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::handleTcpConnectionRequest() start");
    if ((new_socket = accept(fd, (struct sockaddr *) &clientAddress,
                             (socklen_t *) &addrlen)) < 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::handleTcpConnectionRequest() function, accept() failed, error: %s",
                            strerror(errno));
        return false;
    }

    //inform user of socket number - used in send and receive commands
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::handleTcpConnectionRequest(), New connection , socket fd is %d , ip is : %s , port : %d \n",
                        new_socket,
                        inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

    struct pollfd pfd;
    pfd.fd = new_socket;
    pfd.revents = 0;
    pfd.events = POLL_IN;
    pendingPollFds.push_back(pfd);
    handlersMap[new_socket] = std::bind(&Runtime::handleTcpData, this, placeholders::_1);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleTcpConnectionRequest() end");
    return true;
}

bool Runtime::handleTcpData(int fd) {
    bool status = false;
    ssize_t valread = 0;
    struct sockaddr_in clientAddress;
    int addrlen = sizeof(clientAddress);
    char buffer[1025];  //data buffer of 1K
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleTcpData() start");
    if ((valread = read(fd, buffer, 1024)) == 0) {
        //Somebody disconnected , get his details and print
        getpeername(fd, (struct sockaddr *) &clientAddress, (socklen_t *) &addrlen);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::handleTcpData(), Host disconnected , ip %s , port %d",
                            inet_ntoa(clientAddress.sin_addr),
                            ntohs(clientAddress.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close(fd);
    } else {
        //Echo back the message that came in
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::handleTcpData(), data read successfully on port %d",
                            ntohs(clientAddress.sin_port));
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Runtime::handleTcpData(), received message is: %s", buffer);
        //set the string terminating NULL byte on the end of the data read
        buffer[valread] = '\0';
        if (send(fd, buffer, strlen(buffer), 0) != strlen(buffer)) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "error Inside Runtime::handleTcpData() function, send() failed, error: %s",
                                strerror(errno));
        } else {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "Runtime::handleTcpData(), data sent back successfully");
            status = true;
        }
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleTcpData() end");
    return status;
}

bool Runtime::handleUdpData(int masterUdpSocket) {
    struct sockaddr_in clientAddress;
    socklen_t addressLength = sizeof(clientAddress);
    ssize_t receivedLength;
    unsigned char buf[2048];
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleUdpData() start");
    receivedLength = recvfrom(masterUdpSocket, buf, 2048, 0, (struct sockaddr *) &clientAddress,
                              &addressLength);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::handleUdpData(), received %d bytes", (int) receivedLength);
    if (receivedLength >= 0) {
        buf[receivedLength] = 0;
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Inside Runtime::handleUdpData() function, received message: %s",
                            buf);
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::handleUdpData() function, recvfrom() failed, error: %s",
                            strerror(errno));
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleUdpData() end");
    return true;
}

bool Runtime::handleMultiCastData(int multiCastReceiverSocket) {
    struct sockaddr_in multiCastServerAddress;
    socklen_t addressLength = sizeof(multiCastServerAddress);
    ssize_t receivedLength;
    unsigned char buf[2048];
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleMultiCastData() start");
    receivedLength = recvfrom(multiCastReceiverSocket, buf, 2048, 0,
                              (struct sockaddr *) &multiCastServerAddress,
                              &addressLength);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "Runtime::handleMultiCastData(), received %d bytes", (int) receivedLength);
    if (receivedLength >= 0) {
        buf[receivedLength] = 0;
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "Inside Runtime::handleMultiCastData() function, received message: %s",
                            buf);
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error Inside Runtime::handleMultiCastData() function, recvfrom() failed, error: %s",
                            strerror(errno));
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Runtime::handleMultiCastData() end");
    return true;
}

bool Runtime::sendPeriodicMessages() {
    sendMultiCastTimeMessage();
    sendTcpMessage(destinationAddress_, 30509, "hello world TCP message from Android Target");
    sendUdpMessage(destinationAddress_, 30499, "hello world UDP message from Android Target");
    return true;
}
