#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <pthread.h>
using namespace std;

void *client1(void *);
int listenFd;
pthread_mutex_t selectLock = PTHREAD_MUTEX_INITIALIZER;

int main (int argc, char* argv[])
{
    int portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    
    if(argc < 3)
    {
        cerr<<"Syntax : ./client <host name> <port>"<<endl;
        return 0;
    }
    
    portNo = atoi(argv[2]);       
    
    //create client skt
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    server = gethostbyname(argv[1]);
    
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
    
    cout << "socket fd=" << listenFd << endl;

    #if 0
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&selectLock, &mutex_attr);
    #else
    pthread_mutex_init(&selectLock, 0);
    #endif

    int timeout = 200;

    struct timeval select_timeout;
    
    if(timeout >= 0){
        select_timeout.tv_sec = timeout/1000;
        select_timeout.tv_usec = (timeout % 1000) * 1000;
    }

    pthread_t client_pid;
    int ret = pthread_create(&client_pid, NULL, client1, NULL); 
    cout << "pthread_create=" << ret << endl;

    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(listenFd, &writefds);

    char s[300];
    memset(s, 'a', sizeof(s));
    //send stuff to server
    for(;;)
    {
        
        //cin.clear();
        //cin.ignore(256, '\n');
        //cout << "Enter stuff: ";
        //bzero(s, 301);
        //cin.getline(s, 300);
        //sleep(5);
        int pc = pthread_mutex_lock(&selectLock);
        cout << "--mutex_lock=" << pc << endl;
        int rc = select(listenFd+1, 0, &writefds, NULL, &select_timeout);
        cout << "--select=" << rc << endl;

        if(FD_ISSET(listenFd, &writefds)){
            int len = write(listenFd, s, strlen(s));
            cout << "--write=" << len << endl;
        }
        pc = pthread_mutex_unlock(&selectLock);
        cout << "--mutex_unlock=" << pc << endl;
    }
}

void *client1(void *)
{    
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 300);
    bool loop = false;
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(listenFd, &readfds);

    while(!loop)
    {    
        bzero(test, 300);
        int pc = pthread_mutex_lock(&selectLock);
        cout << "++mutex_lock=" << pc << endl;
        int rc  = select(listenFd+1, &readfds, 0, NULL, NULL);
        cerr << "++select=" << rc << endl;

        if(FD_ISSET(listenFd, &readfds))
        {
            int len = read(listenFd, test, 300);            
            cout << "++read= "<< len << endl;
            //string tester (test);
            //cout << "tester " << tester << "rc " << len << endl;
                
            //if(tester == "exit")
            //    break;
        }
        pc = pthread_mutex_unlock(&selectLock);
        cout << "++mutex_unlock=" << pc << endl;
    }
    cout << "\nClosing thread and conn" << endl;
    close(listenFd);
}
