# nDICT
nDICT is a lightweight dictionary object which emulates the python dict type. It's designed as two drop-in classes
that you can compile directly into your projects. The dictionary is implemented as a standalone class, whereas the
JSON parser is a supplementary class to save/load dictionary object to disk for persistent storage.

# Usage
## The ndict class
This is the dictionary class. You can use this standalone to dynamically create, read and write dictionaries
objects for configuration and runtime value storage as you'd like. It is modelled after the JSON standard,
supporting string, numbers and booean values -- Each can be accessed as scalar values, in an array or as nested
objects.

The datatypes are mapped to native C++ types such as bool, int, string and vector.

The following C++ code demonstrates the ndict class:
```
ndict dict;
dict["mystring"]="hello world!";
dict["myint"]=123;

printf("%s",dict["mystring"].getchar());
printf("%s",dict["myint"].getchar());
printf("%d",dict["myint"].getint());
```

This will produce the following output:
```
hello world!
123
123
```

## The njson class
Dictionary objects kept in memory is neat, but in most cases you'd want to save and load the objects on 
non-volatile storage. Fret not! This can be achieved through the njson class which can parse a dictionary
class to a JSON string and save it to a file, and on subsequent run you load the JSON data and parse it back
to a dictionary object.

The following C++ code demonstrates the njson class:
```
ndict dict;
dict["mystring"]="Hello World!";
dict["myint"]=123;

njson parser;
string text=parser.encode(dict);
printf("%s",text.c_str());
```

This will produce the following output:
```
{
    "mystring" : "Hello World!",
    "myint" : 123
}
```

# Other

## Dependencies
No external libraries are needed. It only uses the standard C++ libraries.

## Examples
You can see example_dict.cpp and example_json.cpp for usage examples.

These can be built with the included Makefile:
```
make
```

## Documentation
Only the doxygen reference is available for now. You can generate this with:
```
make doxygen
```

