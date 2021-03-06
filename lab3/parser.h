#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <queue>
#include "utilizer.h"
using namespace std;

typedef unordered_map<string, string> StringMap;
typedef unordered_map<string, pair<bool, string> > LR_StringMap;
typedef unordered_map<char, StringMap> LL1_table;
typedef unordered_map<string, LR_StringMap> LR_table;
typedef unordered_map<char, unordered_set<string>> symbol_set;

// vector<string> pre_process(string token_seq){
//     vector<string> token_arr;
//     for(int i = 0 ; i < token_seq.size() ; ++i){
//         if(token_seq[i] == '<') continue;
//         string token = "";
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
        bool check(vector<pair<string,string> >);
        void get_QT();
};


void LL1::get_QT(){
    for(string s : QT){
        cout << s << endl;
    }
}

void LL1::bulid_analysis_table(){
    cnt = 0;
    StringMap E({{"00", "AT"},{"03", "AT"}, {"53", "AT"}, {"46", "AT"}, {"45", "AT"}, {"48", "AT"}});

    StringMap A({{"54", ""},{"57", ""}, {"58", ""},
                 {"35", "AbTB"}, {"36", "AbTB"},{"37", "AbTB"}, {"38", "AbTB"}, 
                 {"39", "AbTB"}, {"40", "AbTB"}, {"41", "AbTB"}, {"43", "AbTB"}, 
                 {"44", "AbTB"}, {"46", "AbTB"}, {"47", "AbTB"}, {"50", "AbTB"}, 
                 {"52", "AbTB"},  {"61", "AbTB"}, {"62", "AbTB"}, {"63", "AbTB"}, 
                 {"65", "AbTB"}, {"66", "AbTB"}, {"70", "AbTB"}, {"71", "AbTB"}});

    StringMap T({{"00", "CF"},{"03", "CF"},{"53", "CF"},{"46", "CF"},{"45", "CF"},{"48", "CF"}});

    StringMap C({{"54", ""},{"57", ""}, {"58", ""},{"35", ""}, {"36", ""},
                 {"37", ""}, {"38", ""}, {"39", ""}, {"40", ""}, 
                 {"41", ""}, {"43", ""}, {"44", ""}, {"46", ""}, 
                 {"47", ""}, {"50", ""}, {"52", ""},  {"61", ""}, 
                 {"62", ""}, {"63", ""}, {"65", ""}, {"66", ""}, 
                 {"70", ""}, {"71", ""}, 
                 {"42", "CdFD"}, {"49", "CdFD"}, {"51", "CdFD"}});

    StringMap F({{"00", "pI"},{"03", "pI"}, {"53", ")E("}, {"46", "pI"}, {"45", "pI"}, {"48", "pI"}});
    
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

bool LL1::check(vector<pair<string,string> >token_arr){
    // vector<string> token_arr = pre_process(token_seq);
    stack<char> analysis_stack;

    // stack<string> SEM;
    // stack<string> log;

    int index = 0;
    int assign_flag = 0;
    while(index < token_arr.size()){
        analysis_stack.push('#');
        analysis_stack.push('E');
        while(!analysis_stack.empty()){
            char curr = analysis_stack.top();
            analysis_stack.pop();
            // cout << curr << " " << token_arr[index].second << endl;
            if(index == token_arr.size()){
                cout << "Missing terminator!" << endl;
                return false;
            }
            if(islower(curr)){
                if(curr == 'p'){
                    string tk = log.top();
                    // cout << tk << endl;
                    log.pop();
                    SEM.push(tk);
                }
                else if(curr == 'b'){
                    string op = log.top();
                    log.pop();
                    if(op == "="){
                        assign_flag++; 
                        if(assign_flag > 1){
                            cout << "Repeated Assignment!" << endl;
                            return false;
                        }
                    }
                    else{
                        string a = SEM.top();
                        SEM.pop();
                        string b = SEM.top();
                        SEM.pop();
                        string res = "t" + to_string(cnt);
                        SEM.push(res);
                        cnt++;
                        string qt = "( " + op + " " + a + " " + b + " " + res + " )";
                        // cout << qt << endl;
                        QT.push_back(qt);
                    }
                }
                else if(curr == 'd'){
                    string op = log.top();
                    log.pop();
                    string a = SEM.top();
                    SEM.pop();
                    string b = SEM.top();
                    SEM.pop();
                    string res = "t" + to_string(cnt);
                    SEM.push(res);
                    cnt++;
                    string qt = "( " + op + " " + a + " " + b + " " + res + " )";
                    // cout << qt << endl;
                    QT.push_back(qt);
                }
            }
            else{
                if(curr == 'B'){
                    if(Vt[curr].find(token_arr[index].second) == Vt[curr].end()){
                        cout << "Missing operator! (+,-,...)" << endl;
                        return false;
                    }
                    else{
                        log.push(token_arr[index].first);
                        index++;
                    }
                }
                else if(curr == 'D'){
                    if(Vt[curr].find(token_arr[index].second) == Vt[curr].end()){
                        cout << "Missing operator! (*,/,...)" << endl;
                        return false;
                    }
                    else{
                        log.push(token_arr[index].first);
                        index++;
                    }
                }
                else if(curr == 'I'){
                    if(token_arr[index].second == "46"){
                        if(token_arr[index + 1].second == "03" || token_arr[index + 1].second == "00"){
                            string a = token_arr[index + 1].first;
                            string b = "t" + to_string(cnt++);
                            string res = "( - " + a + " _ " + b + " )";
                            QT.push_back(res);
                            log.push(b);
                            index += 2;
                        }
                        else if(token_arr[index + 1].second == "53"){
                            int right = index + 1, left = index + 1;
                            for( ; token_arr[right].second != "54" ; ++right){
                                // cout << token_arr[right] << endl;
                                if(right == token_arr.size() - 1){
                                    cout << "Missing \')\'" << endl;
                                    return false;
                                }
                            }
                            right++;
                            vector<pair<string, string> > temp{&token_arr[left], &token_arr[right]};
                            temp.push_back(make_pair(";", "58"));
                            if(check(temp)){
                                string a = SEM.top();
                                SEM.pop();
                                string b = "t" + to_string(cnt++);
                                string res = "( - " + a + " _ " + b + " )";
                                QT.push_back(res);
                                log.push(b);
                                index = right;
                                // cout << index << endl;
                            }
                            else{
                                cout << "invaild sub-expression" << endl;
                                return false;
                            }
                        }
                        else{
                            cout << "Invaild negative number!" << endl;
                            return false;
                        }
                    }
                    else if(token_arr[index].second == "45" || token_arr[index].second == "48"){
                        if(token_arr[index + 1].second == "00"){
                            if(token_arr[index].second == "45"){
                                string a = token_arr[index + 1].first;
                                string b = "t" + to_string(cnt++);
                                string res = "( ++ " + a + " _ " + b + " )";
                                QT.push_back(res);
                                log.push(b);
                            }
                            else{
                                string a = token_arr[index + 1].first;
                                string b = "t" + to_string(cnt++);
                                string res = "( -- " + a + " _ " + b + " )";
                                QT.push_back(res);
                                log.push(b);
                            }
                            index += 2;
                        }
                        else{
                            cout << "Invaild ++ or --" << endl;
                            return false;
                        }
                    }
                    else if(token_arr[index].second == "00"){
                        if(token_arr[index + 1].second == "45"){
                            string a = token_arr[index].first;
                            string b = "t" + to_string(cnt++);
                            string res = "( ++ " + a + " _ " + b + " )";
                            QT.push_back(res);
                            log.push(b);
                            index += 2;
                        }
                        else if(token_arr[index + 1].second == "48"){
                            string a = token_arr[index].first;
                            string b = "t" + to_string(cnt++);
                            string res = "( -- " + a + " _ " + b + " )";
                            QT.push_back(res);
                            log.push(b);
                            index += 2; 
                        }
                        else{
                            log.push(token_arr[index].first);
                            index++;
                        }
                    }
                    else if(token_arr[index].second != "00" && token_arr[index].second != "03"){
                        cout << "Missing operand!" << endl;
                        return false;
                    }
                    else{
                        log.push(token_arr[index].first);
                        index++;
                    }
                }
                else if(curr == '('){
                    if(token_arr[index].second != "53"){
                        cout << "Missing \'(\' !" << endl;
                        return false;
                    }
                    else{
                        index++;
                    }
                }
                else if(curr == ')'){
                    if(token_arr[index].second != "54"){
                        cout << "Missing \')\' !" << endl;
                        return false;
                    }
                    else{
                        index++;
                    }
                }
                else if(curr == '#'){
                    if(token_arr[index].second != "57" && token_arr[index].second != "58"){
                        cout << "Missing terminator!" << endl;
                        return false;
                    }
                    else{
                        if(assign_flag){
                            string a = SEM.top();
                            SEM.pop();
                            string b = SEM.top();
                            SEM.pop();
                            string res = "( = " + a + " _ " + b + " )";
                            // cout << res << endl;
                            QT.push_back(res);
                            assign_flag = 0;
                        }
                        cnt = 0;
                        QT.push_back("Product Over");
                        index++;
                    }
                }
                else{
                    if(analysis_table[curr].find(token_arr[index].second) == analysis_table[curr].end()){
                        cout << "error " << curr << endl;
                        return false;
                    }
                    else{
                        string next = analysis_table[curr][token_arr[index].second];
                        // bool op_flag = false;
                        // for(char c : next){
                        //     if(islower(c))
                        //         op_flag = true;
                        // }
                        // if(op_flag){
                        //     log.push(token_arr[index].first);
                        //     cout << token_arr[index].first << endl;
                        // }
                        for(int i = 0 ; i < next.size() ; ++i){
                            analysis_stack.push(next[i]);
                        }
                    }
                }
            }
        }
    }
    // cout << "expression correct!" <<endl;
    
    return true;
}



class LR{
    private:
        // stack<char> symbol_stack;
        // stack<string> state_stack;
        LR_table analysis_table;


    public:
        LR(){ build_analysis_table(); }
        void build_analysis_table();
        bool check(vector<pair<string,string> > token_arr);
};

void LR::build_analysis_table(){
    LR_StringMap state_0({{"00", {false, "I2"}}, {"03", {false, "I2"}}, {"46", {false, "I2"}}, {"45", {false, "I2"}}, {"48", {false, "I2"}},
                          {"53", {false, "(6"}},
                          {"E", {false, "E1"}},{"T", {false, "T4"}}, 
                          {"F", {false, "F5"}}
                         });
    LR_StringMap state_1({{"57", {true, "OK"}}, {"58", {true, "OK"}}, 
                          {"35", {false, "B3"}}, {"36", {false, "B3"}},{"37", {false, "B3"}}, {"38", {false, "B3"}}, 
                          {"39", {false, "B3"}}, {"40", {false, "B3"}}, {"41", {false, "B3"}}, {"43", {false, "B3"}}, 
                          {"44", {false, "B3"}}, {"46", {false, "B3"}}, {"47", {false, "B3"}}, {"50", {false, "B3"}}, 
                          {"52", {false, "B3"}},  {"61", {false, "B3"}}, {"62", {false, "B3"}}, {"63", {false, "B3"}}, 
                          {"65", {false, "B3"}}, {"66", {false, "B3"}}, {"70", {false, "B3"}}, {"71", {false, "B3"}}
                         });  
    
    LR_StringMap state_2({{"00", {true, "F/I"}},{"03", {true, "F/I"}}, {"53", {true, "F/I"}},
                          {"54",  {true, "F/I"}},{"57",  {true, "F/I"}}, {"58",  {true, "F/I"}},{"35",  {true, "F/I"}}, {"36",  {true, "F/I"}},
                          {"37",  {true, "F/I"}}, {"38",  {true, "F/I"}}, {"39",  {true, "F/I"}}, {"40",  {true, "F/I"}}, 
                          {"41",  {true, "F/I"}}, {"43",  {true, "F/I"}}, {"44",  {true, "F/I"}}, {"46",  {true, "F/I"}}, 
                          {"47",  {true, "F/I"}}, {"50",  {true, "F/I"}}, {"52",  {true, "F/I"}},  {"61",  {true, "F/I"}}, 
                          {"62",  {true, "F/I"}}, {"63",  {true, "F/I"}}, {"65",  {true, "F/I"}}, {"66",  {true, "F/I"}}, 
                          {"70",  {true, "F/I"}}, {"71",  {true, "F/I"}}, 
                          {"42",  {true, "F/I"}}, {"49",  {true, "F/I"}}, {"51",  {true, "F/I"}},
                          {"45",  {true, "F/I"}}, {"48", {true, "F/I"}}
                         });
    
    LR_StringMap state_3({{"00", {false, "I2"}}, {"03", {false, "I2"}}, {"46", {false, "I2"}}, {"45", {false, "I2"}}, {"48", {false, "I2"}},
                          {"53", {false, "(6"}}, 
                          {"T",{false, "T8"}}, {"F", {false, "F5"}}
                         });                   
    
    LR_StringMap state_4({{"54", {true, "E/T"}}, {"57", {true, "E/T"}}, {"58", {true, "E/T"}},
                          {"35", {true, "E/T"}}, {"36", {true, "E/T"}}, {"37", {true, "E/T"}}, {"38", {true, "E/T"}}, 
                          {"39", {true, "E/T"}}, {"40", {true, "E/T"}}, {"41", {true, "E/T"}}, {"43", {true, "E/T"}}, 
                          {"44", {true, "E/T"}}, {"46", {true, "E/T"}}, {"47", {true, "E/T"}}, {"50", {true, "E/T"}}, 
                          {"52", {true, "E/T"}}, {"61", {true, "E/T"}}, {"62", {true, "E/T"}}, {"63", {true, "E/T"}}, 
                          {"65", {true, "E/T"}}, {"66", {true, "E/T"}}, {"70", {true, "E/T"}}, {"71", {true, "E/T"}},
                          {"42", {false, "D7"}}, {"49", {false, "D7"}}, {"51", {false, "D7"}}
                         });
    
    LR_StringMap state_5({{"00",  {true, "T/F"}}, {"03",  {true, "T/F"}}, {"53",  {true, "T/F"}},
                          {"54",  {true, "T/F"}}, {"57",  {true, "T/F"}}, {"58",  {true, "T/F"}}, {"35",  {true, "T/F"}}, {"36",  {true, "T/F"}},
                          {"37",  {true, "T/F"}}, {"38",  {true, "T/F"}}, {"39",  {true, "T/F"}}, {"40",  {true, "T/F"}}, 
                          {"41",  {true, "T/F"}}, {"43",  {true, "T/F"}}, {"44",  {true, "T/F"}}, {"46",  {true, "T/F"}}, 
                          {"47",  {true, "T/F"}}, {"50",  {true, "T/F"}}, {"52",  {true, "T/F"}}, {"61",  {true, "T/F"}}, 
                          {"62",  {true, "T/F"}}, {"63",  {true, "T/F"}}, {"65",  {true, "T/F"}}, {"66",  {true, "T/F"}}, 
                          {"70",  {true, "T/F"}}, {"71",  {true, "T/F"}}, 
                          {"42",  {true, "T/F"}}, {"49",  {true, "T/F"}}, {"51",  {true, "T/F"}},
                          {"45",  {true, "T/F"}}, {"48",  {true, "T/F"}}
                         });

    LR_StringMap state_6({{"00", {false, "I2"}}, {"03", {false, "I2"}}, {"46", {false, "I2"}}, {"45", {false, "I2"}}, {"48", {false, "I2"}},
                          {"53", {false, "(6"}},
                          {"E", {false, "E10"}}, {"T", {false, "T4"}}, {"F", {false, "F5"}}
                         });

    LR_StringMap state_7({{"00", {false, "I2"}}, {"03", {false, "I2"}}, {"46", {false, "I2"}}, {"45", {false, "I2"}}, {"48", {false, "I2"}},
                          {"53", {false, "(6"}},{"F", {false, "F9"}}
                         });

    LR_StringMap state_8({{"54", {true, "E/EBT"}}, {"57", {true, "E/EBT"}}, {"58", {true, "E/EBT"}},
                          {"35", {true, "E/EBT"}}, {"36", {true, "E/EBT"}}, {"37", {true, "E/EBT"}}, {"38", {true, "E/EBT"}}, 
                          {"39", {true, "E/EBT"}}, {"40", {true, "E/EBT"}}, {"41", {true, "E/EBT"}}, {"43", {true, "E/EBT"}}, 
                          {"44", {true, "E/EBT"}}, {"46", {true, "E/EBT"}}, {"47", {true, "E/EBT"}}, {"50", {true, "E/EBT"}}, 
                          {"52", {true, "E/EBT"}}, {"61", {true, "E/EBT"}}, {"62", {true, "E/EBT"}}, {"63", {true, "E/EBT"}}, 
                          {"65", {true, "E/EBT"}}, {"66", {true, "E/EBT"}}, {"70", {true, "E/EBT"}}, {"71", {true, "E/EBT"}},
                          {"42", {false, "D7"}}, {"49", {false, "D7"}}, {"51", {false, "D7"}}
                         });
    LR_StringMap state_9({{"00",  {true, "T/TDF"}}, {"03",  {true, "T/TDF"}}, {"53",  {true, "T/TDF"}},
                          {"54",  {true, "T/TDF"}}, {"57",  {true, "T/TDF"}}, {"58",  {true, "T/TDF"}}, {"35",  {true, "T/TDF"}}, {"36",  {true, "T/TDF"}},
                          {"37",  {true, "T/TDF"}}, {"38",  {true, "T/TDF"}}, {"39",  {true, "T/TDF"}}, {"40",  {true, "T/TDF"}}, 
                          {"41",  {true, "T/TDF"}}, {"43",  {true, "T/TDF"}}, {"44",  {true, "T/TDF"}}, {"46",  {true, "T/TDF"}}, 
                          {"47",  {true, "T/TDF"}}, {"50",  {true, "T/TDF"}}, {"52",  {true, "T/TDF"}}, {"61",  {true, "T/TDF"}}, 
                          {"62",  {true, "T/TDF"}}, {"63",  {true, "T/TDF"}}, {"65",  {true, "T/TDF"}}, {"66",  {true, "T/TDF"}}, 
                          {"70",  {true, "T/TDF"}}, {"71",  {true, "T/TDF"}}, 
                          {"42",  {true, "T/TDF"}}, {"49",  {true, "T/TDF"}}, {"51",  {true, "T/TDF"}},
                          {"45",  {true, "T/TDF"}}, {"48",  {true, "T/TDF"}}
                         });
    LR_StringMap state_10({{"35", {false, "B3"}}, {"36", {false, "B3"}},{"37", {false, "B3"}}, {"38", {false, "B3"}}, 
                           {"39", {false, "B3"}}, {"40", {false, "B3"}}, {"41", {false, "B3"}}, {"43", {false, "B3"}}, 
                           {"44", {false, "B3"}}, {"46", {false, "B3"}}, {"47", {false, "B3"}}, {"50", {false, "B3"}}, 
                           {"52", {false, "B3"}},  {"61", {false, "B3"}}, {"62", {false, "B3"}}, {"63", {false, "B3"}}, 
                           {"65", {false, "B3"}}, {"66", {false, "B3"}}, {"70", {false, "B3"}}, {"71", {false, "B3"}},
                           {"54", {false, ")11"}}
                          });
    LR_StringMap state_11({{"00", {true, "F/(E)"}}, {"03",  {true, "F/(E)"}}, {"53",  {true, "F/(E)"}},
                           {"54",  {true, "F/(E)"}}, {"57",  {true, "F/(E)"}}, {"58",  {true, "F/(E)"}}, {"35",  {true, "F/(E)"}}, {"36",  {true, "F/(E)"}},
                           {"37",  {true, "F/(E)"}}, {"38",  {true, "F/(E)"}}, {"39",  {true, "F/(E)"}}, {"40",  {true, "F/(E)"}}, 
                           {"41",  {true, "F/(E)"}}, {"43",  {true, "F/(E)"}}, {"44",  {true, "F/(E)"}}, {"46",  {true, "F/(E)"}}, 
                           {"47",  {true, "F/(E)"}}, {"50",  {true, "F/(E)"}}, {"52",  {true, "F/(E)"}}, {"61",  {true, "F/(E)"}}, 
                           {"62",  {true, "F/(E)"}}, {"63",  {true, "F/(E)"}}, {"65",  {true, "F/(E)"}}, {"66",  {true, "F/(E)"}}, 
                           {"70",  {true, "F/(E)"}}, {"71",  {true, "F/(E)"}}, 
                           {"42",  {true, "F/(E)"}}, {"49",  {true, "F/(E)"}}, {"51",  {true, "F/(E)"}},
                           {"45",  {true, "F/(E)"}}, {"48",  {true, "F/(E)"}}
                         });

    analysis_table["0"] = state_0;
    analysis_table["1"] = state_1;
    analysis_table["2"] = state_2;
    analysis_table["3"] = state_3;
    analysis_table["4"] = state_4;
    analysis_table["5"] = state_5;
    analysis_table["6"] = state_6;
    analysis_table["7"] = state_7;
    analysis_table["8"] = state_8;
    analysis_table["9"] = state_9;
    analysis_table["10"] = state_10;
    analysis_table["11"] = state_11;

}

bool LR::check(vector<pair<string,string> > token_arr){
    // vector<string> token_arr = pre_process(token_seq);
    stack<string> state_stack;
    stack<char> symbol_stack;

    int index = 0;
    while(index < token_arr.size()){
        symbol_stack.push('#');
        state_stack.push("0");
        while(!symbol_stack.empty()){
            string state = state_stack.top();
            if(index == token_arr.size()){
                cout << "Missing terminator!" << endl;
                return false;
            }
            // cout << "State " << state << " " << token_arr[index].second << endl;
            if(analysis_table[state].find(token_arr[index].second) == analysis_table[state].end()){
                string in_stack = "";
                unordered_set<string> candidate_left;
                string ac = "";
                string vaild_left = "";
                while(!symbol_stack.empty()){
                    in_stack += state_stack.top() + symbol_stack.top();
                    state_stack.pop();
                    symbol_stack.pop();
                }
                reverse(in_stack.begin(), in_stack.end());
                for(LR_StringMap::iterator it = analysis_table[state].begin() ; it != analysis_table[state].end() ; ++it){
                    ac += it->first + " ";
                    candidate_left.insert(it->second.second);
                }
                for(unordered_set<string>::iterator it = candidate_left.begin() ; it != candidate_left.end() ; ++it){
                    vaild_left += *it + " ";
                }
                cout << "Error!" << endl; 
                cout << "We got " << in_stack << " in stack!" << endl;
                cout << "We can accept token: " << ac << endl;
                cout << "To regress " << vaild_left << endl;
                cout << "But we got " << token_arr[index].second << " now." << endl;
                cout << "Error may be: " << endl; 
                if(state == "0"){
                    cout << "\tMissing operand!" << endl;
                }
                if(state == "1"){
                    cout << "\tMissing operator or terminator!" << endl;
                }
                if(state == "3"){
                    cout << "\tMissing operand or \'(\'!" << endl;
                }
                if(state == "4"){
                    cout << "\tMissing operator!" << endl;
                }
                if(state == "6"){
                    cout << "\tMissing operand or \'(\'!" << endl;
                }
                if(state == "7"){
                    cout << "\tMissing operand or \'(\'!" << endl;
                }
                if(state == "8"){
                    cout << "\tMissing operator!" << endl;
                }
                if(state == "10"){
                    cout << "\tMissing operand or \')\'!" << endl;
                }
                return false;
            }
            else if(!analysis_table[state][token_arr[index].second].first){
                string s = analysis_table[state][token_arr[index].second].second;
                char cur_symbol = s[0];
                string cur_state = s.substr(1, s.size() - 1);
                // cout << "Wi " << cur_symbol << " " << cur_state << endl;
                state_stack.push(cur_state);
                symbol_stack.push(cur_symbol);
                if(token_arr[index].second == "46" && state != "1" && state != "10"){
                    // cout << state << endl;
                    if(token_arr[index + 1].second == "00" || token_arr[index + 1].second == "03")
                        index += 2;
                    else if(token_arr[index + 1].second == "53"){
                        int right = index + 1, left = index + 1;
                        for( ; token_arr[right].second != "54" ; ++right){
                            // cout << token_arr[right] << endl;
                            if(right == token_arr.size() - 1){
                                cout << "Missing \')\'" << endl;
                                return false;
                            }
                        }
                        right++;
                        vector<pair<string,string> > temp{&token_arr[left], &token_arr[right]};
                        temp.push_back(make_pair(";", "58"));
                        // for(int i = 0 ; i < temp.size() ; ++i){
                        //     cout << temp[i] << " ";
                        // }
                        // cout << endl;
                        if(check(temp)){
                            index = right;
                            // cout << index << endl;
                            // cout << "curr" << token_arr[index].second << endl;
                        }
                        else{
                            cout << "invaild sub-expression" << endl;
                            return false;
                        }
                    }
                    else{
                        cout << "invaild negative number!" << endl;
                        return false;
                    }
                }
                else if(token_arr[index].second == "00" && state != "1" && state != "10"){
                    // cout << "++ --" << state << endl;
                    if(token_arr[index + 1].second == "45" || token_arr[index + 1].second == "48")
                        index += 2;
                    else{
                        index++;
                    }
                }
                else if((token_arr[index].second == "45" || token_arr[index].second == "48") && state != "1" && state != "10"){
                    // cout << "++ --" << state << endl;
                    if(token_arr[index + 1].second == "00")
                        index += 2;
                    else{
                        if(token_arr[index].second == "45")
                            cout << "invaild ++ number!" << endl;
                        if(token_arr[index].second == "48")
                            cout << "invaild -- number!" << endl;
                        return false;
                    }
                }
                else{
                    index++;
                }
            }
            else{
                string s = analysis_table[state][token_arr[index].second].second;
                // cout << "R " << s << " ";
                if(s == "OK"){
                    while(!symbol_stack.empty()) symbol_stack.pop();
                    while(!state_stack.empty()) state_stack.pop();
                    index++;
                    break;
                }
                string left = "";
                string right = "";
                int i = 0;
                for( ; s[i] != '/' ; ++i){
                    left += s[i];
                }
                for(i += 1; i < s.size() ; ++i){
                    right += s[i];
                }
                for(int j = right.size() - 1 ; j >= 0 ; --j){
                    char cur_symbol = symbol_stack.top();
                    if(cur_symbol != right[j]){
                        cout << "wrong right!" << endl;
                        return false;
                    }
                    else{
                        symbol_stack.pop();
                        state_stack.pop();
                    }
                }
                string cur_state = state_stack.top();
                // cout << cur_state << " left: " << left << " right: " << right << endl;
                if(analysis_table[cur_state].find(left) == analysis_table[cur_state].end()){
                    cout << "wrong left!" << endl;
                    return false;
                }
                else{
                    string r_s = analysis_table[cur_state][left].second;
                    char cur_symbol = r_s[0];
                    string cur_state = r_s.substr(1, r_s.size() - 1);
                    // cout << "Rr " << cur_symbol << " " << cur_state << endl;
                    state_stack.push(cur_state);
                    symbol_stack.push(cur_symbol);
                }
            }
        }
    }
    // cout << "expression correct!" <<endl;
    return true;
}