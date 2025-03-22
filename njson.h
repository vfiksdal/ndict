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
        static std::string trim(std::string buffer);
        static std::string parsequoted(std::string buffer,const int &pos=0);
        static std::string parseunquoted(std::string buffer,const int &pos=0);
        static std::string parseblock(std::string buffer,const int &pos=0);
        static void parsearray(ndict &object,std::string buffer);
        static void parseobject(ndict &object,std::string buffer);
        static void parsevalue(ndict &object,std::string buffer);
        static ndict::type_t valuetype(std::string buffer);
    public:
        static ndict read(const std::string &path);
        static ndict decode(const std::string &json);
        static std::string encode(const ndict &dict);
        static ndict merge(const std::string &json,const ndict &dict);

};

#endif

