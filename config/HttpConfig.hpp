#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <string>
# include <list>
# include "ConfigNode.hpp"
# include "ServerConfig.hpp"
# include "UpstreamConfig.hpp"

class HttpConfig: public ConfigContainer{
    private:
        HttpConfig(const HttpConfig &other);
        HttpConfig &operator=(const HttpConfig &other);
        std::string _include;
        size_t      _keepalive_timeout;
        std::list<ServerConfig*>   _servers;
        std::list<UpstreamConfig*> _upstreams;
    public:
        ~HttpConfig() {
            for (std::list<ServerConfig*>::iterator it = _servers.begin(); it != _servers.end(); ++it)
                delete *it;
            for (std::list<UpstreamConfig*>::iterator it = _upstreams.begin(); it != _upstreams.end(); ++it)
                delete *it;
        }

        void setInclude(const std::string& include)      { _include = include; }
        void setKeepaliveTimeout(size_t timeout)          { _keepalive_timeout = timeout; }

        const std::string&               getInclude() const          { return _include; }
        size_t                           getKeepaliveTimeout() const  { return _keepalive_timeout; }
        const std::list<ServerConfig*>&  getServers() const           { return _servers; }
        const std::list<UpstreamConfig*>& getUpstreams() const        { return _upstreams; }

        void addChild(ConfigNode* child);
};

#endif
