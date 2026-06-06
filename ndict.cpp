#include "ndict.h"

#define QUOTE(STR)      (std::string("\"")+std::string(STR)+std::string("\""))
#define SET(TYPE,VALUE) {type=TYPE; value=VALUE; return *this;}


/*!\brief Comparison operator for dictionary object
 * \param Value Dictionary object to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const ndict &Value) const {return value==Value.value;}

/*!\brief Comparison operator for string
 * \param Value String to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const std::string &Value) const {return value==Value;}

/*!\brief Comparison operator for character string
 * \param Value String to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const char *Value) const {return value==Value;}

/*!\brief Comparison operator for boolean
 * \param Value Value to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const bool &Value) const {try{return getbool()==Value;}catch(...){return false;}}

/*!\brief Comparison operator for double
 * \param Value Value to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const double &Value) const {char *e; return strtod(value.c_str(),&e)==Value && *e==0;}

/*!\brief Comparison operator for integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const int &Value) const {char *e; return strtol(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Comparison operator for long integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const long &Value) const {char *e; return strtol(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Comparison operator for long long integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const long long &Value) const {char *e; return strtoll(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Comparison operator for unsigned integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const unsigned int &Value) const {char *e; return strtoul(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Comparison operator for unsigned long integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const unsigned long &Value) const {char *e; return strtoul(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Comparison operator for unsigned long long integer
 * \param Value Integer to compare to
 * \return True if values are identical
 */
bool ndict::operator==(const unsigned long long &Value) const {char *e; return strtoull(value.c_str(),&e,10)==Value && (*e==0||*e=='.');}

/*!\brief Assignemnt operator for boolean values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const bool &Value) SET(TBOOL,Value?"true":"false")

/*!\brief Assignemnt operator for boolean values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const std::string &Value) SET(TSTRING,Value)

/*!\brief Assignemnt operator for boolean values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const char *Value) SET(TSTRING,Value)

/*!\brief Assignemnt operator for long long integer values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const long long &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for long integer values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const long &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for integer values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const int &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for unsigned integer values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const unsigned int &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for unsigned long values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const unsigned long &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for unsigned long long values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const unsigned long long &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for double values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const double &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Subscript operator for keyed dictionary values
 * \param Key Key to return object for
 * \return Reference to keyed dictionary object
 */
ndict& ndict::operator[](const std::string &Key){
    // Clear existing array values
    if(type==TARRAY){
        keys.clear();
        items.clear();
    }

    // Find existing value
    for(unsigned i=0;i<keys.size();i++){
        if(keys[i]==Key){
            return items[i];
        }
    }

    // Push new value
    type=TOBJECT;
    keys.push_back(Key);
    items.push_back(ndict());
    return items.back();
}

/*!\brief Removes a named item from object
 * \param key Name of item to remove
 */
void ndict::remove(const std::string &key){
    for(unsigned i=0;type==TOBJECT && i<keys.size();i++){
        if(keys[i]==key){
            keys.erase(keys.begin()+i);
            items.erase(items.begin()+i);
            return;
        }
    }
}

/*!\brief Removes indexed item from array
 * \param index Index of item to remove
 */
void ndict::remove(const unsigned &index){
    if(type==TARRAY && index<keys.size()){
        keys.erase(keys.begin()+index);
        items.erase(items.begin()+index);
        for(unsigned i=0;i<keys.size();i++){
            keys[i]=std::to_string(i);
        }
    }
}

/*!\brief Subscript operator for indexed dictionary values
 * \param Index Numerical index to return object for
 * \return Reference to indexed dictionary object
 */
ndict& ndict::operator[](const unsigned &Index){
    // Check bounds
    if(Index>NDICT_MAX_ARRAY_SIZE){
        throw ndict_exception("Array index is out of bound");
    }

    // Clear non-array values
    if(type!=TARRAY){
        keys.clear();
        items.clear();
    }

    // Assert array contents
    type=TARRAY;
    for(unsigned i=keys.size();i<=Index;i++){
        keys.push_back(std::to_string(i));
        items.push_back(ndict());
    }
    return items[Index];
}

/*!\brief Get size of dictionary object
 * \return Number of child members or array size
 */
unsigned ndict::size() const {
    return keys.size();
}

/*!\brief Clear all child items
 */
void ndict::clear(){
    keys.clear();
    items.clear();
    value="";
    type=TNULL;
}

/*!\brief Check if key is present in this object
 * \return true if key was found with a valid value
 */
bool ndict::haskey(const std::string &key) const{
    for(unsigned i=0;i<keys.size();i++){
        if(keys[i]==key and items[i].type!=TNULL) return true;
    }
    return false;
}

/*!\brief Get a copy of dictionary keys for external iteration
 * \return Copy of dictionary keys for external iteration
 */
std::vector<std::string> ndict::getkeys() const{
    return keys;
}

/*!\brief Get dictionary value as a string
 * \return String representation of value
 */
std::string ndict::getstring() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TSTRING) throw ndict_exception("Value is not string!");
#endif
    return value;
}

/*!\brief Get dictionary value as a char array
 * \return String representation of value
 */
const char *ndict::getchar() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TSTRING) throw ndict_exception("Value is not string!");
#endif
    return value.c_str();
}

/*!\brief Get dictionary value as an integer
 * \return Integer representation of value (0 on failure)
 */
int ndict::getint() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    int v=strtol(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as a long integer
 * \return Integer representation of value (0 on failure)
 */
long ndict::getlong() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    long v=strtol(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as a long long integer
 * \return Integer representation of value (0 on failure)
 */
long long ndict::getlonglong() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    long long v=strtoll(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as an unsigned integer
 * \return Integer representation of value (0 on failure)
 */
unsigned int ndict::getuint() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    unsigned int v=strtoul(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as an unsigned long integer
 * \return Integer representation of value (0 on failure)
 */
unsigned long ndict::getulong() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    unsigned long v=strtoul(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as an unsigned long long integer
 * \return Integer representation of value (0 on failure)
 */
unsigned long long ndict::getulonglong() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    unsigned long long v=strtoull(value.c_str(),&e,10);
    return (*e==0||*e=='.')?v:0;
}

/*!\brief Get dictionary value as a float
 * \return Float representation of value (0 on failure)
 */
double ndict::getdouble() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TNUMBER) throw ndict_exception("Value is not numeric!");
#endif
    char *e=0;
    double v=strtod(value.c_str(),&e);
    return *e==0?v:0;
}

/*!\brief Get dictionary value as a boolean
 * \return Boolean representation of value (false on failure)
 */
bool ndict::getbool() const{
#if NDICT_CHECK_EXISTING
    if(type==TNULL) throw ndict_exception("Value is not set!");
#endif
#if NDICT_CHECK_TYPE
    if(type!=TBOOL) throw ndict_exception("Value is not boolean!");
#endif
    std::string v=value;
    std::transform(v.begin(),v.end(),v.begin(),::toupper);
    return v=="TRUE"?true:atoi(value.c_str());
}

/*!\brief Recursively merge keyed values from another dictionary
 * \param source Dictionary object to copy values from
 *
 * Values unique to the source will be copied verbatim, existing values will
 * be overwritten or retained depending on their existence in the source.
 */
void ndict::merge(ndict &source){
    for(unsigned i=0;i<source.keys.size();i++){
        if(source[source.keys[i]].type==ndict::TOBJECT){
            (*this)[source.keys[i]].merge(source[source.keys[i]]);
        }
        else{
            (*this)[source.keys[i]]=source[source.keys[i]];
        }
    }
}

/*!\brief Recursively encode dictionary object as a JSON string
 * \param indent Number of spaces to use for indentation
 * \param level Number of indents (Increments automatically on recursive calls)
 * \return A JSON string representing this object and it's children
 */
std::string ndict::getjson(const int &indent,const int &level) const{
    // Special case: Format as a json array member
    if(type==TARRAY){
        std::string retval;
        for(unsigned i=0;i<keys.size();i++){
            for(unsigned j=0;j<keys.size();j++){
                if(keys[j]==std::to_string(i)){
                    if(retval.size()) retval+=",";
                    switch(items[i].type){
                        case TOBJECT:   retval+=items[i].getjson(indent,level+1);   break;
                        case TARRAY:    retval+=items[i].getjson(indent,level+1);   break;
                        case TSTRING:   retval+=QUOTE(items[i].value);              break;
                        case TNULL:     retval+="null";                             break;
                        default:        retval+=items[i].value;                     break;
                    }

                }
            }
        }
        return std::string("[")+retval+std::string("]");
    }

    // Normal case: Format as keyed json value
    std::string retval="{\n";
    for(unsigned i=0;i<keys.size();i++){
        retval+=std::string(indent*(level+1),' ')+QUOTE(keys[i])+" : ";
        switch(items[i].type){
            case TOBJECT:   retval+=items[i].getjson(indent,level+1);   break;
            case TARRAY:    retval+=items[i].getjson(indent,level+1);   break;
            case TSTRING:   retval+=QUOTE(items[i].value);              break;
            case TNULL:     retval+="null";                             break;
            default:        retval+=items[i].value;                     break;
        }
        if(i+1<keys.size()) retval+=",";
        retval+="\n";
    }
    retval+=std::string(indent*level,' ')+"}";
    return retval;
}

