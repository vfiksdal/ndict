/*!\file ndict.h
 * \brief A python-like dictionary implementation for C++
 */
#ifndef _NDICT_H_
#define _NDICT_H_

#include <algorithm>
#include <string>
#include <vector>

//! Declares version number. This is not used internally.
#define NDICT_VERSION           "1.1.2"

//! Declare a maximum array size. Will throw an exception if out of bounds.
#define NDICT_MAX_ARRAY_SIZE    1024*8

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
        long getlong() const;
        long long getlonglong() const;
        unsigned int getuint() const;
        unsigned long getulong() const;
        unsigned long long getulonglong() const;

        // Array and object accessors
        void remove(const std::string &key);
        void remove(const unsigned &index);
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
        ndict& operator=(const long &Value);
        ndict& operator=(const long long &Value);
        ndict& operator=(const unsigned int &Value);
        ndict& operator=(const unsigned long &Value);
        ndict& operator=(const unsigned long long &Value);
        ndict& operator=(const double &Value);

        // Comparison operators
        template<typename T> bool operator!=(const T &Value) {return !(*this==Value);}
        bool operator==(const ndict &Value);
        bool operator==(const std::string &Value);
        bool operator==(const char *Value);
        bool operator==(const bool &Value);
        bool operator==(const int &Value);
        bool operator==(const long &Value);
        bool operator==(const long long &Value);
        bool operator==(const unsigned int &Value);
        bool operator==(const unsigned long &Value);
        bool operator==(const unsigned long long &Value);
        bool operator==(const double &Value);

        //! Operators to assign vector objects
        template<typename T,typename A> ndict& operator=(std::vector<T,A> const &Vector){
            // Check bounds
            if(Vector.size()>NDICT_MAX_ARRAY_SIZE){
                throw ndict_exception("Array index is out of bound");
            }

            // Copy array verbatim
            (*this).clear();
            type=TARRAY;
            for(unsigned i=0;i<Vector.size();i++){
                keys.push_back(std::to_string(i));
                ndict item;
                item=Vector[i];
                items.push_back(item);
            }
            return *this;
        }
};

#endif
