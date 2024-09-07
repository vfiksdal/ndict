#include <string.h>
#include "njson.h"

/*!\brief Removes whitespace from input string
 * \param buffer JSON string to trim
 * \return Trimmed JSON string without whitespace
 */
std::string njson::trim(std::string buffer){
    bool quoted=false;
    bool escaped=false;
    size_t size=0;
    for(unsigned i=0;i<buffer.size();i++){
        // Track quotation and escaped characters
        if(quoted && escaped){
            escaped=false;
        }
        else if(quoted && buffer[i]=='\"'){
            quoted=false;
        }
        else if(!quoted && buffer[i]=='\"'){
            quoted=true;
        }

        if(!quoted && std::isspace(buffer[i])){
            // Drop unquoted whitespace
        }
        else{
            // Keep everything else
            escaped=(buffer[i]=='\\');
            buffer[size++]=buffer[i];
        }
    }
    return buffer.substr(0,size);
}

/*!\brief Parses a quoted string
 * \param buffer JSON string containing quoted string
 * \param pos Position in buffer to start search
 * \return Found quoted string
 *
 * Throws njson_exception upon error
 */
std::string njson::parsequoted(std::string buffer,const int &pos){
    bool escaped=false;
    if(buffer[pos]!='\"') throw njson_exception("Expected quoted string");
    for(unsigned i=pos+1;i<buffer.size();i++){
        // Track quotation and escaped characters
        if(escaped){
            escaped=false;
        }
        else if(buffer[i]=='\"'){
            return buffer.substr(pos,i-pos+1);
        }
        else if(buffer[i]=='\\'){
            escaped=true;
        }
    }
    throw njson_exception("String was not unquoted");
}

/*!\brief Parses an unquoted string
 * \param buffer JSON string containing string
 * \param pos Position in buffer to start search
 * \return Found string
 *
 * Throws njson_exception upon error
 */
std::string njson::parseunquoted(std::string buffer,const int &pos){
    for(unsigned i=pos;i<buffer.size();i++){
        if(buffer[i]==',' || buffer[i]=='}'){
            return buffer.substr(pos,i-pos);
        }
    }
    return buffer.substr(pos);
}

/*!\brief Parses a json block value
 * \param buffer JSON string containing [] or {} block
 * \param pos Position in buffer to start search
 * \return Found block
 *
 * Throws njson_exception upon error
 */
std::string njson::parseblock(std::string buffer,const int &pos){
    // Check bracket type
    char end=0,beg=buffer[pos];
    if(beg=='{') end='}';
    if(beg=='[') end=']';

    // Parse block
    bool quoted=false;
    bool escaped=false;
    int depth=0;
    for(unsigned i=pos+1;i<buffer.size();i++){
        // Track quotation and escaped characters
        if(quoted && escaped){
            escaped=false;
        }
        else if(quoted && buffer[i]=='\"'){
            quoted=false;
        }
        else if(!quoted && buffer[i]=='\"'){
            quoted=true;
        }
        if(quoted){
            escaped=(buffer[i]=='\\');
        }
        else{
            // Check for brackets
            if(buffer[i]==beg){
                depth++;
            }
            else if(buffer[i]==end){
                if(depth--==0){
                    return buffer.substr(pos,i-pos+1);
                }
            }
        }
    }
    if(beg=='{')    throw njson_exception("JSON object incorrectly formatted");
    if(beg=='[')    throw njson_exception("JSON array incorrectly formatted");
    throw njson_exception("Invalid JSON block formatting");
}

/*!\brief Parses a json array
 * \param object ndict object to populate with members
 * \param buffer JSON string containing [] block
 *
 * Throws njson_exception upon error
 */
void njson::parsearray(ndict &object,std::string buffer){
    // Remove outer brackets
    if(buffer[0]!='[' || buffer[buffer.size()-1]!=']'){
        throw njson_exception("Invalid JSON array formatting");
    }
    buffer=buffer.substr(1,buffer.size()-2);

    // Parse block
    bool quoted=false;
    std::vector<std::string> array;
    std::string value;
    object=array;
    for(unsigned i=0;i<buffer.size();i++){
        // Track quotation and escaped characters
        if(quoted && buffer[i]=='\"'){
            quoted=false;
        }
        else if(!quoted && buffer[i]=='\"'){
            quoted=true;
        }

        // Check for separator
        if(!quoted && buffer[i]==','){
            array.push_back(value);
            value.clear();
        }
        else{
            value+=buffer[i];
        }
    }
    array.push_back(value);
    for(unsigned i=0;i<array.size();i++){
        parsevalue(object[i],array[i]);
    }
}

/*!\brief Finds type of value by formatting
 * \param buffer String with JSON-formatted value
 * \return ndict enumeration value
 */
ndict::type_t njson::valuetype(std::string buffer){
    if(buffer.size()){
        if(buffer[0]=='\"'){
            return ndict::TSTRING;
        }
        else if(std::isdigit(buffer[0])){
            return ndict::TNUMBER;
        }
        else{
            std::string v=buffer;
            std::transform(v.begin(),v.end(),v.begin(),::toupper);
            if(v=="TRUE" || v=="FALSE"){
                return ndict::TBOOL;
            }
            if(v=="NULL"){
                return ndict::TNULL;
            }
            throw njson_exception(std::string("Invalid JSON value: ")+buffer);
        }
    }
    return ndict::TNULL;
}

/*!\brief Assigns a value with the correct type
 * \param object ndict object to populate with members
 * \param buffer String with JSON-formatted value
 */
void njson::parsevalue(ndict &object,std::string buffer){
    ndict::type_t type=valuetype(buffer);
    if(type==ndict::TSTRING){
        object=buffer.substr(1,buffer.size()-2);
    }
    if(type==ndict::TNUMBER){
        if(buffer.find(".")==std::string::npos){
            object=atoi(buffer.c_str());
        }
        else{
            object=atof(buffer.c_str());
        }
    }
    if(type==ndict::TBOOL){
        std::string v=buffer;
        std::transform(v.begin(),v.end(),v.begin(),::toupper);
        object=(v=="TRUE")?true:false;
    }
}

/*!\brief Parses a json object
 * \param object ndict object to populate with members
 * \param buffer JSON string containing {} block
 *
 * Throws njson_exception upon error
 */
void njson::parseobject(ndict &object,std::string buffer){
    // Remove outer brackets
    if(buffer[0]!='{' || buffer[buffer.size()-1]!='}'){
        throw njson_exception("Invalid JSON object formatting");
    }
    buffer=buffer.substr(1,buffer.size()-2);

    // Parse keyed values
    unsigned pos=0;
    while(pos<buffer.size()){
        // Parse key
        std::string value,key=parsequoted(buffer,pos);
        pos+=key.size();
        if(buffer[pos++]!=':'){
            throw njson_exception("Key and value must be separated by :");
        }

        // Parse value
        switch(buffer[pos]){
            case '\"':  value=parsequoted(buffer,pos);  break;
            case '{':   value=parseblock(buffer,pos);   break;
            case '[':   value=parseblock(buffer,pos);   break;
            default:    value=parseunquoted(buffer,pos);break;
        }
        pos+=value.size();
        if(buffer[pos]==',') pos++;

        // Load key:value
        key=key.substr(1,key.size()-2);
        if(value[0]=='['){
            parsearray(object[key],value);
        }
        else if(value[0]=='{'){
            parseobject(object[key],value);
        }
        else{
            parsevalue(object[key],value);
        }
    }
}

/*!\brief Reads a JSON string from a file and decodes the input to a dictionary object
 * \param path Path to JSON file to read
 * \return ndict object of the decoded file
 *
 * Throws njson_exception upon error
 */
ndict njson::read(const std::string &path){
    FILE *fd=fopen(path.c_str(),"r");
    if(fd){
        fseek(fd,0,SEEK_END);
        size_t size=ftell(fd);
        fseek(fd,0,SEEK_SET);
        char buffer[size+1];
        size=fread(buffer,1,size,fd);
        fclose(fd);
        buffer[size]=0;
        return decode(buffer);
    }
    else{
        throw njson_exception(std::string("Failed to open input file: ")+strerror(errno));
    }
}

/*!\brief Decodes a JSON string to a dictionary object
 * \param json String containing JSON text to be decoded
 * \return ndict object of the decoded string
 *
 * Throws njson_exception upon error
 */
ndict njson::decode(const std::string &json){
    ndict object;
    std::string buffer=trim(json);
    parseobject(object,buffer);
    return object;
}

/*!\brief Encodes a dictionary object as a JSON string
 * \param dict Dictionary object to encode
 * \return JSON string representing the dictionary object
 */
std::string njson::encode(const ndict &dict){
    return dict.getjson();
}

/*!\brief Merges a JSON string with a dictionary object
 * \param json JSON string to decode and merge
 * \param dict Dictionary object to merge with
 * \return A new merged object
 *
 * Throws njson_exception upon decoding errors
 */
ndict njson::merge(const std::string &json,const ndict &dict){
    ndict newdict=decode(json);
    ndict mrgdict=dict;
    mrgdict.merge(newdict);
    return mrgdict;
}

