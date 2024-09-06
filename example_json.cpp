#include <iostream>
#include "njson.h"

using namespace std;

/*!\brief Simple example of how to use the dictionary class
 * \return 0
 */
int main(){
    // Set some simple values
    ndict cfg;
    cfg["bool"]=true;
    cfg["string"]="string";
    cfg["int"]=123;
    cfg["float"]=123.456;


    // Encode to a json string
    njson parser;
    string json=parser.encode(cfg);
    cout << "json:" << endl << json << endl << endl;

    // Decode back to a dictionary
    cfg=parser.decode(json);

    // Read back values from new dictionary
    cout << "Values:" << endl;
    cout << "int: " << cfg["int"].getint() << endl;
    cout << "string: " << cfg["string"].getstring() << endl;
    return 0;
}

