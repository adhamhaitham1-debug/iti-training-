#include "NetworkManager.h"
#include <iostream>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <iomanip>

void NetworkManager::displayNetworkInfo() const {
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs failed");
        return;
    }

    std::cout << "\n=====================================================\n";
    std::cout << std::left << std::setw(15) << "INTERFACE"
              << std::setw(20) << "IPv4 ADDRESS"
              << std::setw(10) << "STATE" << "\n";
    std::cout << "=====================================================\n";

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        // Look for IPv4 addresses only
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            char ipStr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sa->sin_addr), ipStr, INET_ADDRSTRLEN);

            std::string state = (ifa->ifa_flags & IFF_UP) ? "UP" : "DOWN";

            std::cout << std::left << std::setw(15) << ifa->ifa_name
                      << std::setw(20) << ipStr
                      << std::setw(10) << state << "\n";
        }
    }

    freeifaddrs(ifaddr);
}

void NetworkManager::pingHost(const std::string& host) const {
    std::cout << "\n--- Pinging " << host << " ---\n";
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // Child process executes ping command via execvp (No system() call!)
        char* args[] = { (char*)"ping", (char*)"-c", (char*)"3", (char*)host.c_str(), nullptr };
        execvp("ping", args);
        
        // If execvp returns, an error occurred
        perror("execvp failed");
        _exit(1);
    } else {
        // Parent process waits for child
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            std::cout << "Ping completed successfully.\n";
        } else {
            std::cout << "Ping failed or host unreachable.\n";
        }
    }
}

void NetworkManager::dnsLookup(const std::string& hostname) const {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res)) != 0) {
        std::cerr << "DNS Lookup Error: " << gai_strerror(status) << "\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "      DNS LOOKUP FOR: " << hostname << "\n";
    std::cout << "========================================\n";

    for (p = res; p != nullptr; p = p->ai_next) {
        void *addr;
        std::string ipver;

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr); // Fixed: sin6_addr
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << " " << std::left << std::setw(6) << ipver << " : " << ipstr << "\n";
    }

    freeaddrinfo(res);
}