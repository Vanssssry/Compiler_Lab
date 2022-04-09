#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
using namespace std;

typedef unordered_map<string, string> StringMap;
typedef unordered_map<char, StringMap> LL1_table;
typedef unordered_map<char, unordered_set<string>> symbol_set;

class LL1{
    private:
        LL1_table analysis_table;
        stack<char> analysis_stack;
        symbol_set Vt; 
        
    public:
        LL1(){
            bulid_analysis_table();
        }
        void bulid_analysis_table();
        bool check(string token_seq);
};

class LR{
    private:

    public:

};

void LL1::bulid_analysis_table(){
    StringMap E({{"00", "AT"},{"03", "AT"}, {"53", "AT"}});
    StringMap A({{"54", ""},{"57", ""}, {"58", ""},
                 {"35", "ATB"}, {"36", "ATB"},{"37", "ATB"}, {"38", "ATB"}, 
                 {"39", "ATB"}, {"40", "ATB"}, {"41", "ATB"}, {"43", "ATB"}, 
                 {"44", "ATB"}, {"46", "ATB"}, {"47", "ATB"}, {"50", "ATB"}, 
                 {"52", "ATB"},  {"61", "ATB"}, {"62", "ATB"}, {"63", "ATB"}, 
                 {"65", "ATB"}, {"66", "ATB"}, {"70", "ATB"}, {"71", "ATB"}});
    StringMap T({{"00", "CF"},{"03", "CF"},{"53", "CF"}});
    StringMap C({{"54", ""},{"57", ""}, {"58", ""},{"35", ""}, {"36", ""},
                 {"37", ""}, {"38", ""}, {"39", ""}, {"40", ""}, 
                 {"41", ""}, {"43", ""}, {"44", ""}, {"46", ""}, 
                 {"47", ""}, {"50", ""}, {"52", ""},  {"61", ""}, 
                 {"62", ""}, {"63", ""}, {"65", ""}, {"66", ""}, 
                 {"70", ""}, {"71", ""}, 
                 {"42", "CFD"}, {"49", "CFD"}, {"51", "CFD"}});
    StringMap F({{"00", "I"},{"03", "I"}, {"53", ")E("}});
    
    unordered_set<string> B({"35", "36","37", "38", "39", "40", "41", "43", 
                             "44", "46", "47", "50", "52",  "61", "62", "63", 
                             "65", "66", "70", "71"});
    unordered_set<string> D({"42", "49", "51"});

    analysis_table['E'] = E;
    analysis_table['A'] = A;
    analysis_table['T'] = T;
    analysis_table['C'] = C;
    analysis_table['F'] = F;

    Vt['B'] = B;
    Vt['D'] = D;
}

bool LL1::check(string token_seq){
    vector<string> token_arr;
    for(int i = 0 ; i < token_seq.size() ; ++i){
        if(token_seq[i] == '<') continue;
        string token = "";
        while(token_seq[i] != '>'){
            token += token_seq[i];
            i++;
        }
        token_arr.push_back(token);
    }
    int index = 0;
    while(index < token_arr.size()){
        analysis_stack.push('#');
        analysis_stack.push('E');
        while(!analysis_stack.empty()){
            char curr = analysis_stack.top();
            analysis_stack.pop();
            // cout << curr << " " << token_arr[index] << endl;
            if(curr == 'B'){
                if(Vt[curr].find(token_arr[index]) == Vt[curr].end()){
                    cout << "err B" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == 'D'){
                if(Vt[curr].find(token_arr[index]) == Vt[curr].end()){
                    cout << "err D" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == 'I'){
                if(token_arr[index] != "00" && token_arr[index] != "03"){
                    cout << "err I" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == '('){
                if(token_arr[index] != "53"){
                    cout << "err (" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == ')'){
                if(token_arr[index] != "54"){
                    cout << "err )" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == '#'){
                if(token_arr[index] != "57" && token_arr[index] != "58"){
                    cout << "err #" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else{
                if(analysis_table[curr].find(token_arr[index]) == analysis_table[curr].end()){
                    cout << "err " << curr << endl;
                    return false;
                }
                else{
                    string next = analysis_table[curr][token_arr[index]];
                    for(int i = 0 ; i < next.size() ; ++i){
                        analysis_stack.push(next[i]);
                    }
                }
            }
        }
    }
    cout << "expression correct!" <<endl;
    return true;
}