#ifndef HTTP_HPP
# define HTTP_HPP

enum HttpMethod {
    GET,
    POST,
    DELETE,
};


enum LocationModifier{
    MOD_NONE,         // sem modificador
    MOD_EXACT,        // =
	MOD_REGEX,        // ~
	MOD_REGEX_CI,     // ~*
	MOD_PREFIX,       // ^~
};

enum IOMultiplexer{
    //(Linux)
    SELECT,
    POLL,
    EPOLL,
    KQUEUE, // (macOS/BSD)
};

#endif