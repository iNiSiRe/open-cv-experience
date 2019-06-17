//
// Created by inisire on 6/8/2019.
//

#ifndef CV_TCPCLIENT_H
#define CV_TCPCLIENT_H

#include<iostream>	//cout
#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<string>	//string
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<netdb.h>	//hostent

using namespace std;

class TCPClient
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    TCPClient();
    bool conn(string, int);
    bool send_data(string data);
    string receive(int);
};


#endif //CV_TCPCLIENT_H
