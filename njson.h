/*!\file njson.h
 * \brief A JSON wrapper for ndict, allowing dynamically loading and saving dictionary objects
 */
#ifndef _NJSON_H_
#define _NJSON_H_

#include <algorithm>
#include <string>
#include "ndict.h"

/*!\class njson_exception
 * \brief Exception class for json parser
 */
class njson_exception: public std::exception {
    private:
        std::string msg;
    public:
        njson_exception(const std::string &message) : msg(message) {}
        const char *what(){return msg.c_str();}
};

/*!\class njson
 * \brief Parses JSON strings to a dictionary or vice-versa
 */
class njson {
    private:
        std::string trim(std::string buffer);
        std::string parsequoted(std::string buffer,const int &pos=0);
        std::string parseunquoted(std::string buffer,const int &pos=0);
        std::string parseblock(std::string buffer,const int &pos=0);
        void parsearray(ndict &object,std::string buffer);
        void parseobject(ndict &object,std::string buffer);
        void parsevalue(ndict &object,std::string buffer);
        ndict::type_t valuetype(std::string buffer);
    public:
        ndict read(const std::string &path);
        ndict decode(const std::string &json);
        std::string encode(const ndict &dict);
};

#endif

