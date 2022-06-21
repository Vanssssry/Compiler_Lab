#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <queue>
#include "utilizer.h"
#include "symbol_table.h"
using namespace std;

typedef unordered_map<string, vector<string> > LL1_StringMap;
typedef unordered_map<string, LL1_StringMap> LL1_table;
typedef unordered_map<string, unordered_set<string>> symbol_set;
typedef unordered_map<string, pair<bool, string> > LR_StringMap;
typedef unordered_map<string, LR_StringMap> LR_table;

// vector<string> pre_process(string token_seq){
//     vector<string> token_arr;
//     for(int i = 0 ; i < token_seq.size() ; ++i){
//         if(token_seq[i] == '<') continue;
//         string token = ;
//         while(token_seq[i] != '>'){
//             token += token_seq[i];
//             i++;
//         }
//         token_arr.push_back(token);
//     }
//     return token_arr;
// }

class LL1{
    private:
        LL1_table analysis_table;
        // stack<char> analysis_stack;
        stack<string> SEM;
        stack<string> log;
        symbol_set Vt; 
        vector<string> QT;
        int cnt;

    public:
        LL1(){
            bulid_analysis_table();
        }
        void bulid_analysis_table();
        vector<string> get_QT();
        bool check(vector<pair<string,string> > token_arr, symbol_table_system & STM);
        void print_QT();
};


vector<string> LL1::get_QT(){
    return QT;
}

void LL1::print_QT(){
    for(string s : QT){
        cout << s << endl;
    }
}

void LL1::bulid_analysis_table(){
    cnt = 0;

    LL1_StringMap program({{"32", {"var_decl"}}, {"19", {"var_decl"}}, {"06", {"var_decl"}}, {"15", {"var_decl"}}, {"00", {"statement"}}, {"03", {"statement"}}, {"53", {"statement"}}, {"18", {"statement"}}});

    LL1_StringMap var_decl({{"32", {"var_decl_", "{ENT()}", "{name()}", "00", "{init()}", "type"}}, {"19", {"var_decl_", "{ENT()}", "{name()}", "00", "{init()}", "type"}}, {"06", {"var_decl_", "{ENT()}", "{name()}", "00", "{init()}", "type"}}, {"15", {"var_decl_", "{ENT()}", "{name()}", "00", "{init()}", "type"}}});

    LL1_StringMap statement({{"00", {"statement_", "A"}}, {"03", {"statement_", "A"}}, {"53", {"statement_", "A"}}, {"18", {"if_state"}}});

    LL1_StringMap type({{"32", {"{typ(v)}", "32"}}, {"19", {"{typ(i)}", "19"}}, {"06", {"{typ(c)}", "06"}}, {"15", {"{typ(f)}", "15"}}});

    LL1_StringMap var_decl_({{"57", {"var_decl_", "{ENT()}", "{name()}", "00", "57"}}, {"58", {}}});

    LL1_StringMap statement_({{"39", {"statement_", "{assi(=)}", "A", "39"}}, {"54", {}}, {"56", {}}, {"58", {}}});

    LL1_StringMap if_state({{"18", {"else_state", "{IE(ie)}", "56", "58", "program", "55", "{IF(if)}", "54", "A", "53","18"}}});

    LL1_StringMap else_state({{"12", {"{IE(ie)}", "56", "58", "statement", "55", "{EL(el)}", "12"}}, {"54", {}}, {"56", {}}, {"58", {}}, {"00", {"program"}}, {"03", {"program"}}, {"32", {"program"}}, {"06", {"program"}}, {"19", {"program"}}, {"15",{"program"}}, {"53", {"program"}}, {"55", {"program"}}, {"18", {"program"}}});

    LL1_StringMap A({{"00", {"A_", "E"}}, {"03", {"A_", "E"}}, {"53", {"A_", "E"}}});

    LL1_StringMap A_({{"39", {}}, {"54", {}}, {"56", {}}, {"58", {}}, {"35", {"A_", "{GEQ(w2)}", "E", "w2"}}, {"36", {"A_", "{GEQ(w2)}", "E", "w2"}}, {"37", {"A_", "{GEQ(w2)}", "E", "w2"}}, {"38", {"A_", "{GEQ(w2)}", "E", "w2"}}, {"40", {"A_", "{GEQ(w2)}", "E", "w2"}}, {"41", {"A_", "{GEQ(w2)}", "E", "w2"}}});

    LL1_StringMap E({{"00", {"E_", "T"}}, {"03", {"E_", "T"}}, {"53", {"E_", "T"}}});

    LL1_StringMap E_({{"39", {}}, {"54", {}}, {"56", {}}, {"58", {}}, {"35", {}}, {"36", {}}, {"37", {}}, {"38", {}}, {"40", {}}, {"41", {}}, {"43", {"E_", "{GEQ(w0)}", "T", "w0"}}, {"46", {"E_", "{GEQ(w0)}", "T", "w0"}}});

    LL1_StringMap T({{"00", {"T_", "F"}}, {"03", {"T_", "F"}}, {"53", {"T_", "F"}}});

    LL1_StringMap T_({{"39", {}}, {"54", {}}, {"56", {}}, {"58", {}}, {"35", {}}, {"36", {}}, {"37", {}}, {"38", {}}, {"40", {}}, {"41", {}}, {"43", {}}, {"46", {}}, {"49", {"T_", "{GEQ(w1)}", "F", "w1"}}, {"51", {"T_", "{GEQ(w1)}", "F", "w1"}}});

    LL1_StringMap F({{"00", {"{push(i)}", "00"}}, {"03", {"{push(c)}", "03"}}, {"53", {"54", "statement", "53"}}});
    
    unordered_set<string> w0({"43", "46"});
    unordered_set<string> w1({"49", "51"});
    unordered_set<string> w2({"35", "36", "37", "38", "40", "41"});

    analysis_table["program"] = program;
    analysis_table["var_decl"] = var_decl;
    analysis_table["statement"] = statement;
    analysis_table["type"] = type;
    analysis_table["var_decl_"] = var_decl_;
    analysis_table["statement_"] = statement_;
    analysis_table["if_state"] = if_state;
    analysis_table["else_state"] = else_state;
    analysis_table["A"] = A;
    analysis_table["A_"] = A_;
    analysis_table["E"] = E;
    analysis_table["E_"] = E_;
    analysis_table["T"] = T;
    analysis_table["T_"] = T_;
    analysis_table["F"] = F;

    Vt["w0"] = w0;
    Vt["w1"] = w1;
    Vt["w2"] = w2;
}

bool LL1::check(vector<pair<string,string> >token_arr, symbol_table_system & STM){
    // vector<string> token_arr = pre_process(token_seq);
    stack<string> analysis_stack;

    // stack<string> SEM;
    // stack<string> log;

    int index = 0;
    int assign_flag = 0;
    int offset = 0;
    while(index < token_arr.size()){
        analysis_stack.push("#");
        analysis_stack.push("program");
        table_item curr_var;
        stack<string> var_name_seq;
        while(!analysis_stack.empty()){
            string curr = analysis_stack.top();
            analysis_stack.pop();
            if(index >= token_arr.size()){
                if(curr == "#" || curr == "else_state"){
                    return true;
                }
                if(token_arr[index - 1].second == "56"){
                    QT.push_back("( ie _, _, _ )");
                }
                cout << "Missing terminator!" << endl;
                return false;
            }
            cout << curr << " " << token_arr[index].second << " " << token_arr[index].first << " " << index << " " << token_arr.size() << endl;
            if(curr == "{init()}"){
                curr_var.cat = "v";
                curr_var.L = "y";
            }
            else if(curr == "{name()}"){
                string cur_name = log.top();
                cout << "{name()}" << cur_name << endl;
                log.pop();
                var_name_seq.push(cur_name);
            }
            else if(curr == "{typ(v)}"){
                curr_var.type = "void";
                curr_var.addr = 1;
            }
            else if(curr == "{typ(i)}"){
                curr_var.type = "int";
                curr_var.addr = 4;
            }
            else if(curr == "{typ(c)}"){
                curr_var.type = "char";
                curr_var.addr = 1;
            }
            else if(curr == "{typ(f)}"){
                curr_var.type = "float";
                curr_var.addr = 4;
            }
            else if(curr == "{ENT()}"){
                // cout << "enter: " << var_name_seq.empty() << endl;
                while(!var_name_seq.empty()){
                    string tmp_name = var_name_seq.top();
                    cout << "enter: " << tmp_name << endl;
                    var_name_seq.pop();
                    STM.ST[tmp_name].cat = curr_var.cat;
                    STM.ST[tmp_name].type = curr_var.type;
                    STM.ST[tmp_name].L = curr_var.L;
                    STM.ST[tmp_name].addr = offset;
                    offset += curr_var.addr;
                    cout << "curr_off: " << offset << endl;
                }
            }
            else if(curr == "{assi(=)}"){
                string a = SEM.top();
                SEM.pop();
                string b = SEM.top();
                SEM.pop();
                string res = "( = " + a + ", _, " + b + " )";
                // cout << res << endl;
                QT.push_back(res);
            }
            else if(curr == "{GEQ(w2)}" || curr == "{GEQ(w0)}" || curr == "{GEQ(w1)}"){
                string op = log.top();
                string a = SEM.top();
                SEM.pop();
                string b = SEM.top();
                SEM.pop();
                string res = "t" + to_string(cnt);
                SEM.push(res);
                cnt++;
                string curr_type = STM.ST.find(a) == STM.ST.end() ? STM.ST[b].type : STM.ST[a].type;
                cout << curr_type << endl;
                table_item tmp(res, curr_type, "v", "n", offset);
                offset += type_len[curr_type];
                STM.ST[res] = tmp;
                string qt = "( " + op + " " + b + ", " + a + ", " + res + " )";
                // cout << qt << endl;
                QT.push_back(qt);
            }
            // else if(curr == "{GEQ(w0)}"){
                
            // }
            // else if(curr == "{GEQ(w1)}"){
                
            // }
            else if(curr == "{push(i)}"){
                string tmp = log.top();
                log.pop();
                SEM.push(tmp);
            }
            else if(curr == "{push(c)}"){
                string tmp = log.top();
                log.pop();
                SEM.push(tmp);
            }
            else if(curr == "{IF(if)}"){
                cout << SEM.empty() << endl;
                string a = SEM.top();
                SEM.pop();
                string tmp = "( if " + a + ", _, _ )";
                QT.push_back(tmp);
            }
            else if(curr == "{EL(el)}"){
                string tmp = "( el _, _, _ )";
                QT.push_back(tmp);
            }
            else if(curr == "{IE(ie)}"){
                string tmp = "( ie _, _, _ )";
                QT.push_back(tmp);
            }
            else if(curr == "w0"){
                if(Vt[curr].find(token_arr[index].second) == Vt[curr].end()){
                    cout << "Missing operator! (+,-,...)" << endl;
                    return false;
                }
                else{
                    log.push(token_arr[index].first);
                    index++;
                }
            }
            else if(curr == "w1"){
                if(Vt[curr].find(token_arr[index].second) == Vt[curr].end()){
                    cout << "Missing operator! (*,/,...)" << endl;
                    return false;
                }
                else{
                    log.push(token_arr[index].first);
                    index++;
                }
            }
            else if(curr == "w2"){
                if(Vt[curr].find(token_arr[index].second) == Vt[curr].end()){
                    cout << "Missing operator! (==,!=,...)" << endl;
                    return false;
                }
                else{
                    log.push(token_arr[index].first);
                    index++;
                }
            }
            else if(curr == "00"){   
                if(token_arr[index].second != "00"){
                    cout << "Missing operand!" << endl;
                    return false;
                }
                else{
                    log.push(token_arr[index].first);
                    index++;
                }
            }
            else if(curr == "03"){   
                if(token_arr[index].second != "03"){
                    cout << "Missing operand!" << endl;
                    return false;
                }
                else{
                    log.push(token_arr[index].first);
                    index++;
                }
            }
            else if(curr == "53"){
                if(token_arr[index].second != "53"){
                    cout << "Missing \'(\' !" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "54"){
                if(token_arr[index].second != "54"){
                    cout << "Missing \')\' !" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "#" || curr == "58"){
                // if(token_arr[index - 1].second == "56"){
                //     index++;
                // }
                if(token_arr[index].second != "58"){
                    cout << "Missing terminator!" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "32"){
                if(token_arr[index].second != "32"){
                    cout << "Wrong type!" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "19"){
                if(token_arr[index].second != "19"){
                    cout << "Wrong type!" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "15"){
                if(token_arr[index].second != "15"){
                    cout << "Wrong type!" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "06"){
                if(token_arr[index].second != "06"){
                    cout << "Wrong type!" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "57"){
                if(token_arr[index].second != "57"){
                    cout << ",?" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "39"){
                if(token_arr[index].second != "39"){
                    cout << "invaild assignment" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "55"){   
                if(token_arr[index].second != "55"){
                    cout << "{ ?" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "56"){   
                if(token_arr[index].second == "00" || token_arr[index].second == "32" || token_arr[index].second == "19" || token_arr[index].second == "06" || token_arr[index].second == "15"){
                    analysis_stack.push("56");
                    analysis_stack.push("#");
                    analysis_stack.push("program");
                }
                else if(token_arr[index].second != "56"){
                    cout << "} ?" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "18"){   
                if(token_arr[index].second != "18"){
                    cout << "if ?" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else if(curr == "12"){   
                if(token_arr[index].second != "12"){
                    cout << "else ?" << endl;
                    return false;
                }
                else{
                    index++;
                }
            }
            else{
                if(analysis_table[curr].find(token_arr[index].second) == analysis_table[curr].end()){
                    cout << "error " << curr << " " << token_arr[index].second << endl;
                    return false;
                }
                else{
                    vector<string> next = analysis_table[curr][token_arr[index].second];
                    for(int i = 0 ; i < next.size() ; ++i){
                        analysis_stack.push(next[i]);
                    }
                }
            }
        }
    }
    // cout << "expression correct!" <<endl;
    return true;
}



