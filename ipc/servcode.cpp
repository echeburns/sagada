#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
using namespace std;

void *task1(void *);
void *client1(void *);

static int connFd;
static int listenFd;

int main(int argc, char* argv[])
{
    int pId, portNo;
    socklen_t len; //store size of the address
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;
    
    pthread_t threadA[3];
    pthread_t client_pid;

    if (argc < 2)
    {
        cerr << "Syntam : ./server <port>" << endl;
        return 0;
    }
    
    portNo = atoi(argv[1]);
    
    if((portNo > 65535) || (portNo < 2000))
    {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }
    
    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    cout << "Server socket=" << listenFd << endl;

    bzero((char*) &svrAdd, sizeof(svrAdd));
    
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);
    
    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }
    
    listen(listenFd, 5);
    
    len = sizeof(clntAdd);
    
    int noThread = 0;

    pthread_create(&client_pid, NULL, client1, NULL); 

    while (noThread < 3)
    {
        cout << "Listening" << endl;

        //this is where client connects. svr will hang in this mode until client conn
        connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
            cout << "Connection successful" << endl;
        }
        
        pthread_create(&threadA[noThread], NULL, task1, NULL); 
        
        noThread++;
    }
    
    for(int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }
    
    
}

void *client1(void *)
{
    int listenFd, portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;

    #if 0
    if(argc < 3)
    {
        cerr<<"Syntax : ./client <host name> <port>"<<endl;
        return 0;
    }
    
    portNo = atoi(argv[2]);
    
    if((portNo > 65535) || (portNo < 2000))
    {
        cerr<<"Please enter port number between 2000 - 65535"<<endl;
        return 0;
    }       
    #else

    portNo = 5001;
    
    #endif

    //create client skt
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    #if 0
    server = gethostbyname(argv[1]);
    #else
    server = gethostbyname("10.30.10.73");
    #endif

    if(server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
    svrAdd.sin_port = htons(portNo);
    
    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    
    cout << "cliend socket fd=" << listenFd << endl;

    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(listenFd, &writefds);
    char s[300];
    memset(s, 'a', sizeof(s));
    int timeout = -1;

    struct timeval select_timeout;
    
    //if(timeout >= 0){
        select_timeout.tv_sec = timeout/1000;
        select_timeout.tv_usec = (timeout % 1000) * 1000;
    //}

    long timeout_msec = select_timeout.tv_sec * 1000;
    timeout_msec += select_timeout.tv_usec / 1000000;
    if(timeout_msec > 86400000){
        /*Max timeout on VRX is one day*/
        timeout_msec = 86400000;
    }
    else if(timeout_msec < 1){
        timeout_msec = 1;
    }
    cout << "timeout ms="<< timeout_msec << endl;    
    //send stuff to server
    for(;;)
    {
        //cin.clear();
        //cin.ignore(256, '\n');
        //cout << "Enter stuff: ";
        //bzero(s, 301);
        //cin.getline(s, 300);
        //int rc = select(listenFd+1, 0, &writefds, NULL, &select_timeout);
        int rc = select(listenFd+1, 0, &writefds, NULL, NULL);
        cout << "select=" << rc << endl;

        if(FD_ISSET(listenFd, &writefds)){
            int len = write(listenFd, s, strlen(s));
            cout << ">>sent=" << len << endl;
        }
    }
}

void *task1 (void *dummyPt)
{
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 301);
    bool loop = false;
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(connFd, &readfds);


    while(!loop)
    {    
        bzero(test, 301);
        int rc  = select(connFd+1, &readfds, 0, NULL, NULL);
        cerr << "select" << rc << endl;

        if(FD_ISSET(connFd, &readfds))
        {
            int len = read(connFd, test, 300);            
            cout << "++recv= "<< len << endl;
            //string tester (test);
            //cout << "tester " << tester << "rc " << len << endl;
                
            //if(tester == "exit")
            //    break;
        }
    }
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
}
