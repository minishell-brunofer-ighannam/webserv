#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <list>
# include <map>
# include "ConfigNode.hpp"
# include "LocationConfig.hpp"
# include "http.hpp"

struct Listen {
    std::string host;
    int port;
};

class ServerConfig: public ConfigContainer{
    private:
        ServerConfig(const ServerConfig &other);
        ServerConfig &operator=(const ServerConfig &other);
        std::list<Listen> _listen;
        std::map<int, std::string> _error_page;
        size_t _client_max_body_size;
        std::list<LocationConfig*> _locations;
    public:
        ~ServerConfig() {
            for (std::list<LocationConfig*>::iterator it = _locations.begin(); it != _locations.end(); ++it)
                delete *it;
        }
        void setClientMaxBodySize(size_t size)               { _client_max_body_size = size; }
        void addListen(const Listen& listen)                 { _listen.push_back(listen); }
        void addErrorPage(int code, const std::string& path) { _error_page[code] = path; }

        size_t                                    getClientMaxBodySize() const { return _client_max_body_size; }
        const std::list<Listen>&                  getListen() const            { return _listen; }
        const std::map<int, std::string>&         getErrorPages() const        { return _error_page; }
        const std::list<LocationConfig*>& getLocations() const { return _locations; }

        const LocationConfig* match_location(const std::string& uri) const;
        void addChild(ConfigNode*);
};

#endif