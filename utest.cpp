/*!\file utest.cpp
 * \brief Unit tests for ndict
 */
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include "ndict.h"
#include "njson.h"

int upassed=0;
int ufailed=0;

/*!\brief Simplistic unittest
 * \param Name Name of the test
 * \param Result True if the test passed
 */
void test(const std::string &Name,const bool &Result){
    printf("    %-75s%s\n",Name.c_str(),Result?"PASS":"FAIL");
    if(Result){
        upassed++;
    }
    else{
        ufailed++;
    }
}

/*!\brief Test basic dictionary handling
 */
void test_dict(){
    // Stage a dictionary object
    printf("\nRunning dictionary test:\n");
    ndict object;
    test("Empty dictionary has 0 root items",object.size()==0);
    object["string"]="string";
    object["int"]=123;
    object["float"]=123.456;
    object["object"]["value1"]="value1";
    object["object"]["value2"]="value2";
    object["object"]["value3"]="value3";
    object["outer"]["inner"]["value1"]="value1";
    object["outer"]["inner"]["value2"]="value2";
    object["outer"]["inner"]["value3"]="value3";

    // Test dictionary
    test("Dictionary has N root items",object.size()==5);
    test("Dictionary string value",object["string"].getstring()=="string");
    test("Dictionary char value",object["string"].getchar()==std::string("string"));
    test("Dictionary int value",object["int"].getint()==123);
    test("Dictionary float value",object["float"].getdouble()==123.456);
    test("Dictionary object has N items",object["object"].size()==3);
    test("Dictionary object first string value",object["object"]["value1"].getstring()=="value1");
    test("Dictionary object second string value",object["object"]["value2"].getstring()=="value2");
    test("Dictionary object third string value",object["object"]["value3"].getstring()=="value3");
    test("Dictionary nested object has N items",object["outer"].size()==1);
    test("Dictionary nested sub object has N items",object["outer"]["inner"].size()==3);
    test("Dictionary nested sub object first value",object["outer"]["inner"]["value1"].getstring()=="value1");
    test("Dictionary nested sub object second value",object["outer"]["inner"]["value2"].getstring()=="value2");
    test("Dictionary nested sub object third value",object["outer"]["inner"]["value3"].getstring()=="value3");

    // Copy dict and retest
    ndict copy=object;
    test("Dictionary copy has N root items",copy.size()==5);
    test("Dictionary copy string value",copy["string"].getstring()=="string");
    test("Dictionary copy char value",copy["string"].getchar()==std::string("string"));
    test("Dictionary copy int value",copy["int"].getint()==123);
    test("Dictionary copy float value",copy["float"].getdouble()==123.456);
    test("Dictionary copy object has N items",copy["object"].size()==3);
    test("Dictionary copy object first string value",copy["object"]["value1"].getstring()=="value1");
    test("Dictionary copy object second string value",copy["object"]["value2"].getstring()=="value2");
    test("Dictionary copy object third string value",copy["object"]["value3"].getstring()=="value3");
    test("Dictionary copy nested object has N items",copy["outer"].size()==1);
    test("Dictionary copy nested sub object has N items",copy["outer"]["inner"].size()==3);
    test("Dictionary copy nested sub object first value",copy["outer"]["inner"]["value1"].getstring()=="value1");
    test("Dictionary copy nested sub object second value",copy["outer"]["inner"]["value2"].getstring()=="value2");
    test("Dictionary copy nested sub object third value",copy["outer"]["inner"]["value3"].getstring()=="value3");
}

/*!\brief Test basic array handling
 */
void test_array(){
    // Stage a dictionary containing arrays
    printf("\nRunning array handling test:\n");
    ndict object;
    std::vector<std::string> sv;
    sv.push_back("this");
    sv.push_back("is a");
    sv.push_back("string");
    sv.push_back("of");
    sv.push_back("strings");
    std::vector<int> iv;
    iv.push_back(0);
    iv.push_back(1);
    iv.push_back(2);
    iv.push_back(3);
    iv.push_back(4);
    std::vector<std::vector<int>> iiv;
    iiv.push_back(iv);
    iiv.push_back(iv);

    // Test dictionary object
    object["stringarray"]=sv;
    object["intarray"]=iv;
    object["intintarray"]=iiv;
    test("Loaded vector of N string items",object["stringarray"].size()==5);
    test("Loaded vector of N integer items",object["intarray"].size()==5);
    test("Checking first string value",object["stringarray"][0].getstring()=="this");
    test("Checking second string value",object["stringarray"][1].getstring()=="is a");
    test("Checking third string value",object["stringarray"][2].getstring()=="string");
    test("Checking fourth string value",object["stringarray"][3].getstring()=="of");
    test("Checking fifth string value",object["stringarray"][4].getstring()=="strings");
    test("Checking first integer value",object["intarray"][0].getint()==0);
    test("Checking second integer value",object["intarray"][1].getint()==1);
    test("Checking third integer value",object["intarray"][2].getint()==2);
    test("Checking fourth integer value",object["intarray"][3].getint()==3);
    test("Checking fifth integer value",object["intarray"][4].getint()==4);
}

/*!\brief Test json-dictionary parsing
 */
void test_json_string(){
    // Load a simple json string
    printf("\nRunning json-string parser test:\n");
    std::string text=""
        "{\n"
        "   \"bool\" : true,\n"
        "   \"string\" : \"string\",\n"
        "   \"sstring\" : \"st ring\",\n"
        "   \"qstring\" : \"st\\\"ring\",\n"
        "   \"int\" : 123,\n"
        "   \"float\" : 123.456000,\n"
        "   \"intarray\" : [0,1,2,3,4],\n"
        "   \"strarray\" : [\"a\",\"b\",\"c\"],\n"
        "   \"outer\" : {\n"
        "       \"inner\" : {\n"
        "           \"value1\" : \"hello\",\n"
        "           \"value2\" : \"world\",\n"
        "           \"value3\" : \"!!!\"\n"
        "       }\n"
        "   }\n"
        "}\n";

    // Parse string to a dictionary object and test
    njson json;
    ndict object=json.decode(text);
    //printf("in :%s\n",text.c_str());
    //printf("out:%s\n",object.getjson().c_str());
    test("Parsed N root items",object.size()==9);
    test("Parsed bool item",object["bool"].getbool()==true);
    test("Parsed vanilla string item",object["string"].getstring()=="string");
    test("Parsed string item with space",object["sstring"].getstring()=="st ring");
    test("Parsed string item with escape",object["qstring"].getstring()=="st\\\"ring");
    test("Parsed integer item",object["int"].getint()==123);
    test("Parsed float item",object["float"].getdouble()==123.456);
    test("Parsed integer array of N items",object["intarray"].size()==5);
    test("Parsed integer array first value",object["intarray"][0].getint()==0);
    test("Parsed integer array second value",object["intarray"][1].getint()==1);
    test("Parsed integer array third value",object["intarray"][2].getint()==2);
    test("Parsed integer array fourth value",object["intarray"][3].getint()==3);
    test("Parsed integer array fifth value",object["intarray"][4].getint()==4);
    test("Parsed string array of N items",object["strarray"].size()==3);
    test("Parsed string array first value",object["strarray"][0].getstring()=="a");
    test("Parsed string array second value",object["strarray"][1].getstring()=="b");
    test("Parsed string array third value",object["strarray"][2].getstring()=="c");
    test("Parsed subobject of N objects",object["outer"].size()==1);
    test("Parsed sub-subobject of N objects",object["outer"]["inner"].size()==3);
    test("Parsed sub-subobject first value",object["outer"]["inner"]["value1"].getstring()=="hello");
    test("Parsed sub-subobject second value",object["outer"]["inner"]["value2"].getstring()=="world");
    test("Parsed sub-subobject third value",object["outer"]["inner"]["value3"].getstring()=="!!!");
}

/*!\brief Test json-dictionary parsing
 */
void test_json_file(){
    // Stage a simple json string
    printf("\nRunning json-file parser test:\n");
    std::string text=""
        "{\n"
        "   \"bool\" : true,\n"
        "   \"string\" : \"string\",\n"
        "   \"sstring\" : \"st ring\",\n"
        "   \"qstring\" : \"st\\\"ring\",\n"
        "   \"int\" : 123,\n"
        "   \"float\" : 123.456000,\n"
        "   \"intarray\" : [0,1,2,3,4],\n"
        "   \"strarray\" : [\"a\",\"b\",\"c\"],\n"
        "   \"outer\" : {\n"
        "       \"inner\" : {\n"
        "           \"value1\" : \"hello\",\n"
        "           \"value2\" : \"world\",\n"
        "           \"value3\" : \"!!!\"\n"
        "       }\n"
        "   }\n"
        "}\n";

#if 0
    // Write json to a temporary file (theoretically unsafe, but portable)
    std::string tmpname=std::tmpnam(nullptr);
    FILE *fd=fopen(tmpname.c_str(),"w");
    if(fd){
        fwrite(text.c_str(),text.size(),1,fd);
        fclose(fd);
    }
    else{
        printf("Failed to open %s for writing\n",tmpname.c_str());
        ufailed++;
        return;
    }
#else
    // Write json to a temporary file (safe, butlinux-only)
    char fnbuffer[32];
    strcpy(fnbuffer,"/tmp/ndict_utest_XXXXXX");
    int fd=mkstemp(fnbuffer);
    std::string tmpname=fnbuffer;
    write(fd,text.c_str(),text.size());
    close(fd);
#endif

    // Parse read-back json to a dict and test
    njson json;
    ndict object=json.read(tmpname);
    unlink(tmpname.c_str());
    test("Parsed N root items",object.size()==9);
    test("Parsed bool item",object["bool"].getbool()==true);
    test("Parsed vanilla string item",object["string"].getstring()=="string");
    test("Parsed string item with space",object["sstring"].getstring()=="st ring");
    test("Parsed string item with escape",object["qstring"].getstring()=="st\\\"ring");
    test("Parsed integer item",object["int"].getint()==123);
    test("Parsed float item",object["float"].getdouble()==123.456);
    test("Parsed integer array of N items",object["intarray"].size()==5);
    test("Parsed integer array first value",object["intarray"][0].getint()==0);
    test("Parsed integer array second value",object["intarray"][1].getint()==1);
    test("Parsed integer array third value",object["intarray"][2].getint()==2);
    test("Parsed integer array fourth value",object["intarray"][3].getint()==3);
    test("Parsed integer array fifth value",object["intarray"][4].getint()==4);
    test("Parsed string array of N items",object["strarray"].size()==3);
    test("Parsed string array first value",object["strarray"][0].getstring()=="a");
    test("Parsed string array second value",object["strarray"][1].getstring()=="b");
    test("Parsed string array third value",object["strarray"][2].getstring()=="c");
    test("Parsed subobject of N objects",object["outer"].size()==1);
    test("Parsed sub-subobject of N objects",object["outer"]["inner"].size()==3);
    test("Parsed sub-subobject first value",object["outer"]["inner"]["value1"].getstring()=="hello");
    test("Parsed sub-subobject second value",object["outer"]["inner"]["value2"].getstring()=="world");
    test("Parsed sub-subobject third value",object["outer"]["inner"]["value3"].getstring()=="!!!");
}


/*!\brief Test dictionary-json-dictionary encoding/decoding
 */
void test_encode_decode(){
    // Stage a simple dictionary object
    printf("\nRunning encode-decode test:\n");
    ndict object;
    object["string"]="string";
    object["int"]=123;
    object["float"]=123.456;
    object["object"]["value1"]="value1";
    object["object"]["value2"]="value2";
    object["object"]["value3"]="value3";
    object["outer"]["inner"]["value1"]="value1";
    object["outer"]["inner"]["value2"]="value2";
    object["outer"]["inner"]["value3"]="value3";

    // Test original dictionary
    test("Original has N root items",object.size()==5);
    test("Original string value",object["string"].getstring()=="string");
    test("Original int value",object["int"].getint()==123);
    test("Original float value",object["float"].getdouble()==123.456);
    test("Original object has N items",object["object"].size()==3);
    test("Original object first string value",object["object"]["value1"].getstring()=="value1");
    test("Original object second string value",object["object"]["value2"].getstring()=="value2");
    test("Original object third string value",object["object"]["value3"].getstring()=="value3");
    test("Original nested object has N items",object["outer"].size()==1);
    test("Original nested sub object has N items",object["outer"]["inner"].size()==3);
    test("Original nested sub object first value",object["outer"]["inner"]["value1"].getstring()=="value1");
    test("Original nested sub object second value",object["outer"]["inner"]["value2"].getstring()=="value2");
    test("Original nested sub object third value",object["outer"]["inner"]["value3"].getstring()=="value3");

    // Parse to json, read back, and test new object
    njson json;
    std::string text=json.encode(object);
    object=json.decode(text);
    test("Reencoded has N root items",object.size()==5);
    test("Reencoded string value",object["string"].getstring()=="string");
    test("Reencoded int value",object["int"].getint()==123);
    test("Reencoded float value",object["float"].getdouble()==123.456);
    test("Reencoded object has N items",object["object"].size()==3);
    test("Reencoded object first string value",object["object"]["value1"].getstring()=="value1");
    test("Reencoded object second string value",object["object"]["value2"].getstring()=="value2");
    test("Reencoded object third string value",object["object"]["value3"].getstring()=="value3");
    test("Reencoded nested object has N items",object["outer"].size()==1);
    test("Reencoded nested sub object has N items",object["outer"]["inner"].size()==3);
    test("Reencoded nested sub object first value",object["outer"]["inner"]["value1"].getstring()=="value1");
    test("Reencoded nested sub object second value",object["outer"]["inner"]["value2"].getstring()=="value2");
    test("Reencoded nested sub object third value",object["outer"]["inner"]["value3"].getstring()=="value3");
}

/*!\brief Test json-dictionary merging
 */
void test_json_merge(){
    // Create a simple dictionary
    printf("\nRunning json-dictionary merging test:\n");
    ndict ogobject;
    ogobject["bool"]=false;
    ogobject["string2"]="test";
    ogobject["outer"]["inner"]["value3"]="test";

    // Load a simple json string and merge it into the dictionary
    std::string text=""
        "{\n"
        "   \"bool\" : true,\n"
        "   \"string\" : \"string\",\n"
        "   \"float\" : 123.456000,\n"
        "   \"intarray\" : [0,1,2,3,4],\n"
        "   \"outer\" : {\n"
        "       \"inner\" : {\n"
        "           \"value1\" : \"hello\",\n"
        "           \"value2\" : \"world\",\n"
        "       }\n"
        "   }\n"
        "}\n";

    njson json;
    ndict object=json.merge(text,ogobject);

    // Test merged dictionary
    test("Merged object has N root items",object.size()==6);
    test("Destination retains unique values",object["string2"].getstring()=="test");
    test("Destination got a copy of unique values",object["string"].getstring()=="string");
    test("Destination had matching values overwritten",object["bool"].getbool()==true);
    test("Destination retains nested unique values",object["outer"]["inner"]["value3"].getstring()=="test");
    test("Destination got a copy of nested unique values",object["outer"]["inner"]["value1"].getstring()=="hello");
    test("Destination got a copy of array",object["intarray"].size()==5);
    test("Destination got a copy of array values",object["intarray"][3].getint()==3);
    //printf("og:%s\n",ogobject.getjson().c_str());
    //printf("src:%s\n",json.decode(text).getjson().c_str());
    //printf("merged:%s\n",object.getjson().c_str());
}

/*!\brief Test error handling
 */
void test_error(){
    // Stage a dictionary object
    printf("\nRunning error handling test:\n");
    ndict object;
    object["string"]="string";
    object["int"]=123;
    object["float"]=123.456;

    // Test dictionary
    test("Dictionary string value resolves to 0 as int",object["string"].getint()==0);
    test("Dictionary string value resolves to 0 as float",object["string"].getdouble()==0);
    test("Dictionary string value resolves to false as bool",object["string"].getbool()==false);
    test("Invalid dictionary value resolves to empty string",object["invalid"].getstring()=="");
    test("Invalid dictionary value resolves to 0 as int",object["invalid"].getint()==0);
    test("Invalid dictionary value resolves to 0 as float",object["invalid"].getdouble()==0);
    test("Invalid dictionary value resolves to false as bool",object["invalid"].getbool()==0);

    // Test encoding with trailing comma
    njson json;
    {
        std::string text=""
            "{\n"
            "   \"bool\" : true,\n"
            "   \"string\" : \"string\",\n"
            "   \"int\" : 123,\n"
            "   \"float\" : 123.456000,\n"
            "   \"intarray\" : [0,1,2,3,4],\n"
            "}\n";
            object=json.decode(text);
            test("Can decode json with trailing comma",object.size()==5);
    }

    // Test encoding with trailing trailing junk
    {
        bool result=false;
        std::string text=""
            "{\n"
            "   \"bool\" : true,\n"
            "   \"string\" : \"string\",\n"
            "   \"int\" : 123,\n"
            "   \"float\" : 123.456000,\n"
            "   \"intarray\" : [0,1,2,3,4],\n"
            "}xx\n";
            try{
                object=json.decode(text);
            }
            catch(njson_exception e){
                result=true;
            }
            test("Decoding json with trailing junk throws exception",result);
    }

    // Test encoding with trailing trailing junk
    {
        bool result=false;
        std::string text=""
            "{\n"
            "   \"bool\" : rue,\n"
            "}\n";
            try{
                object=json.decode(text);
                printf("%s\n",object.getjson().c_str());
            }
            catch(njson_exception e){
                result=true;
            }
            test("Decoding json with invalid keywords throws exception",result);
    }

    // Test encoding with trailing trailing junk
    {
        bool result=false;
        std::string text=""
            "{\n"
            "   \"bool\" : true,\n"
            "   \"float\" : 123,456000,\n"
            "   \"intarray\" : [0,1,2,3,4],\n"
            "}\n";
            try{
                object=json.decode(text);
            }
            catch(njson_exception e){
                result=true;
            }
            test("Decoding json with invalid decimal separator throws exception",result);
    }
}

/*!\brief Run baby! RUN!
 */
int main(){
    printf("utest %s\n",NDICT_VERSION);
    printf("ndict unittest system\n");
    printf("Vegard Fiksdal (C) 2024\n");
    test_dict();
    test_array();
    test_json_string();
    test_json_file();
    test_encode_decode();
    test_json_merge();
    test_error();
    printf("\nPassed %d/%d tests\n",upassed,upassed+ufailed);
    if(ufailed==0){
        printf("All clear!\n");
    }
    else{
        printf("Failed %d tests!\n",ufailed);
    }
    return ufailed;
}

