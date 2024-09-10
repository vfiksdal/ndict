/*!\file ndict.h
 * \brief A python-like dictionary implementation for C++
 */
#ifndef _NDICT_H_
#define _NDICT_H_

#include <algorithm>
#include <string>
#include <vector>

//! Declares version number. This is not used internally.
#define NDICT_VERSION           "1.0.1"

//! Declare a maximum array size. Will throw an exception if out of bounds.
#define NDICT_MAX_ARRAY_SIZE    1024

//! Throw an exception when accessing non-existing values
#define NDICT_CHECK_EXISTING    true

//! Enable strict type-checking when accessing values
#define NDICT_CHECK_TYPE        true

/*!\class ndict_exception
 * \brief Exception class for dictionary handling
 */
class ndict_exception: public std::exception {
    private:
        std::string msg;
    public:
        ndict_exception(const std::string &message) : msg(message) {}
        const char *what(){return msg.c_str();}
};

/*!\class ndict
 * \brief Implements a dictionary object
 */
class ndict {
    private:
        std::vector<std::string> keys;
        std::vector<ndict> items;
        std::string value;
    public:
        //! Enumerate JSON types
        enum type_t{
            TNUMBER,    //!< Value is a number
            TSTRING,    //!< Value is a string
            TBOOL,      //!< Value is a boolean
            TARRAY,     //!< Value is an array
            TOBJECT,    //!< Value is an object
            TNULL       //!< Value is not valid
        } type=TNULL;

        // Value accessors
        std::string getstring() const;
        const char *getchar() const;
        double getdouble() const;
        bool getbool() const;
        int getint() const;

        // Array and object accessors
        unsigned size() const;
        void clear();

        // Key accessors
        bool haskey(const std::string &key) const;
        std::vector<std::string> getkeys() const;

        // Merge contents from a dict into this one
        void merge(ndict &source);

        // Export to json string
        std::string getjson(const int &indent=4,const int &level=0) const;

        // Operator for recursive blocks
        ndict& operator[](const std::string &Key);
        ndict& operator[](const unsigned &Key);

        // Operators to set item value
        ndict& operator=(const std::string &Value);
        ndict& operator=(const char *Value);
        ndict& operator=(const bool &Value);
        ndict& operator=(const int &Value);
        ndict& operator=(const unsigned int &Value);
        ndict& operator=(const double &Value);

        //! Operators to assign vector objects
        template<typename T,typename A> ndict& operator=(std::vector<T,A> const &Vector){
            for(unsigned i=0;i<Vector.size();i++){
                (*this)[i]=Vector[i];
            }
            return *this;
        }
};

#endif
