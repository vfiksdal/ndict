#include "ndict.h"

#define QUOTE(STR)      (std::string("\"")+std::string(STR)+std::string("\""))
#define SET(TYPE,VALUE) {type=TYPE; value=VALUE; return *this;}

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

/*!\brief Assignemnt operator for boolean values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const int &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for boolean values
 * \param Value Value to assign to dictionary object
 * \return Reference to assigned dictionary object
 */
ndict& ndict::operator=(const unsigned int &Value) SET(TNUMBER,std::to_string(Value))

/*!\brief Assignemnt operator for boolean values
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
    for(unsigned i=0;i<=Index;i++){
        bool hit=false;
        for(unsigned j=0;!hit && j<keys.size();j++){
            hit|=(keys[j]==std::to_string(i));
        }
        if(!hit){
            keys.push_back(std::to_string(i));
            items.push_back(ndict());
        }
    }

    // Find item
    for(unsigned i=0;i<keys.size();i++){
        if(keys[i]==std::to_string(Index)){
            return items[i];
        }
    }

    // Fallback to satisfy compiler: TODO: Throw
    throw ndict_exception("Invalid array allocation");
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
    return atoi(value.c_str());
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
    return atof(value.c_str());
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
    std::vector<std::string> keys=source.getkeys();
    for(unsigned i=0;i<keys.size();i++){
        if(source[keys[i]].type==ndict::TOBJECT){
            (*this)[keys[i]].merge(source[keys[i]]);
        }
        else{
            (*this)[keys[i]]=source[keys[i]];
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

