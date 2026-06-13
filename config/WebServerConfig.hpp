#ifndef WEBSERVERCONFIG_HPP
# define WEBSERVERCONFIG_HPP

# include <list>
# include "ConfigNode.hpp"
# include "GlobalConfig.hpp"

class WebServerConfig : public ConfigContainer {
    private:
        WebServerConfig(const WebServerConfig& other);
        WebServerConfig& operator=(const WebServerConfig& other);
        GlobalConfig* _global;
    public:
        WebServerConfig() : _global(NULL) {}
        ~WebServerConfig() { delete _global; }

        const GlobalConfig* getGlobal() const { return _global; }

        std::list<ServerConfig*> getServers() const {
            if (!_global || !_global->getHttp())
                return std::list<ServerConfig*>();
            return _global->getHttp()->getServers();
        }

        void addChild(ConfigNode* child);
};

#endif
