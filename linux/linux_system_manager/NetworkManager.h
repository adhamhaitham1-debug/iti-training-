#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>

class NetworkManager {
public:
    void displayNetworkInfo() const;
    void pingHost(const std::string& host) const;
    void dnsLookup(const std::string& hostname) const;
};

#endif // NETWORK_MANAGER_H