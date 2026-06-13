#ifndef UPSTREAMCONFIG_HPP
# define UPSTREAMCONFIG_HPP
# include <string>
# include "ConfigNode.hpp"
# include <list>

struct UpstreamServer
{
    std::string _ip;
    size_t _weight;
    bool _flag;
};

class UpstreamConfig: public ConfigLeaf{
    private:
        UpstreamConfig(const UpstreamConfig &other);
        UpstreamConfig &operator=(const UpstreamConfig &other);
        std::list<UpstreamServer> _servers;
    public:
        void addServer(const UpstreamServer& server) { _servers.push_back(server); }

        const std::list<UpstreamServer>& getServers() const { return _servers; }
};

#endif