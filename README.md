# nomjson - A JSON parser module for C++

---
                        PLEASE NOTE!

            THIS IS AN ARCHIVE OF AN OLDER PROJECT
          THAT HAS NOT BEEN UPDATED FOR SEVERAL YEARS
---

**Description:**

NomJSON is a module for converting raw JSON text (e.g. from a
web service) into a format that's more useful to a C++ program. 
The **ParseJSONFromMemory** or **ParseJSONFromFile** APIs may be
called to turn JSON text into a tree of nodes, where each node
is one of the six basic JSON data types (number, string, bool,
array, group, or null).  

**Language:** C++

**Platform:** Windows

**Source Files:**

* nomjson.h: C++ header for the NomJSON module. This would be included by any program that uses NomJSON.

* nomjson.cpp: C++ implementation for the NomJSON module.

* nomjsontest.cpp: Test program. It reads any JSON file and outputs a detailed dump of the JSON nodes to the console. 

* makefile: An NMAKE build script to compile NomJSON using Microsoft C++ compiler.

