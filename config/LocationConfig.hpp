#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <list>
# include <map>
# include "ConfigNode.hpp"
# include "http.hpp"

class LocationConfig: public ConfigLeaf{
    private:
        LocationConfig(const LocationConfig &other);
        LocationConfig &operator=(const LocationConfig &other);
        std::string _path;
        LocationModifier _modifier;
        std::list<HttpMethod> _methods;
        std::string _redirect;
        std::string _root;
        bool _autoindex;
        std::list<std::string> _index;
        std::string _upload_dir;
        std::map<std::string, std::string> _cgi;
    public:
        void setPath(const std::string& path)        { _path = path; }
        void setModifier(LocationModifier modifier)   { _modifier = modifier; }
        void setRedirect(const std::string& redirect) { _redirect = redirect; }
        void setRoot(const std::string& root)         { _root = root; }
        void setAutoindex(bool autoindex)             { _autoindex = autoindex; }
        void setUploadDir(const std::string& dir)     { _upload_dir = dir; }
        const std::string &getPath() const {return (_path);}
        const LocationModifier &getModifier() const {return (_modifier);}
        const std::string &getRedirect() const {return (_redirect);}
        const std::string &getRoot() const {return (_root);}
        bool getAutoIndex() const {return _autoindex;}
        const std::string &getUploadDir() const {return (_upload_dir);}
        const std::list<HttpMethod> &getMethods() const {return _methods;}
        const std::list<std::string> &getIndex() const {return _index;}
        const std::map<std::string, std::string> &getCgi() const {return _cgi;}
        void addMethod(HttpMethod method){
            _methods.push_back(method);
        }
        void addIndex(const std::string& index){
            _index.push_back(index);
        }
        void addCgi(const std::pair<const std::string, const std::string>& cgi){
            _cgi[cgi.first] = cgi.second;
        }
        bool matches(const std::string &uri);
};

#endif