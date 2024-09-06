#include <iostream>
#include "ndict.h"

using namespace std;

/*!\brief Simple example of how to use the json parser with the dictionary
 * \return 0
 */
int main(){
    // Set some simple values
    ndict cfg;
    cfg["string"]="string";
    cfg["float"]=123.456;

    // Add nested value
    cfg["outer"]["inner"]["value1"]="hello";

    // Add array values
    cfg["outer"]["inner"]["array"][0]="cruel";
    cfg["outer"]["inner"]["array"][1]="world";


    // Read back value
    cout << "string: " << cfg["string"].getstring() << endl;
    cout << "float: " << cfg["float"].getdouble() << endl;
    cout << "nested: " << cfg["outer"]["inner"]["value1"].getstring() << endl;
    cout << "array[0]: " << cfg["outer"]["inner"]["array"][0].getstring() << endl;
    cout << "array[1]: " << cfg["outer"]["inner"]["array"][1].getstring() << endl;
    return 0;
}

