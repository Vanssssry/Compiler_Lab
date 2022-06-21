#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

typedef struct table_item{
    string name;
    string type;
    string cat;
    string L;
    int addr;

    table_item() {}  
    table_item(string _name, string _type, string _cat, string _L,int _addr):name(_name),type(_type),cat(_cat),addr(_addr), L(_L) {}        
} table_item;

unordered_map<string, int> type_len({{"int", 4}, {"void", 1}, {"char", 1}, {"float", 4}});

typedef unordered_map<string, table_item> symbol_table;
typedef unordered_map<string, int> constant_table;

class symbol_table_system{
    private:

    public:
        symbol_table ST;
        constant_table CT;
        symbol_table_system(){}
};

#endif