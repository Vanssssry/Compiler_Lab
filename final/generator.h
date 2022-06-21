#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <queue>
#include "symbol_table.h"
using namespace std;


class Generator{
    private:
        long long code_basic_addr = 0x00400000;
        long long mem_basic_addr = 0x10010000;
        int reg_basic_index = 8;
        int full_index = 0;
        int t_cnt = 0;
        int f_cnt = 0;

        vector<pair<string, string> > RDL;
        vector<string> M;
    public:
        Generator();
        vector<string> scan_QT(vector<string> QT, symbol_table_system STM);
        vector<string> generate(vector<string> QT, symbol_table_system STM);
        int get_reg(string var_name);
        int get_empty_reg();
        string alloc_reg(string A, string QT_A_L, symbol_table_system stm, vector<string> & obj, bool load);
};

Generator::Generator(){
    for(int i = 0 ; i < 16 ; ++i){
        RDL.push_back(make_pair("", ""));
    }
}

string Generator::alloc_reg(string A, string QT_A_L, symbol_table_system STM, vector<string> & obj, bool load = true){
    string reg_a;
    if(get_empty_reg() != -1){
        if(get_reg(A) == -1){
            int reg = get_empty_reg();
            RDL[reg] = make_pair(A, QT_A_L);
            string reg_a = "$" + to_string(reg_basic_index + reg);
            if(load){
                string code = "lw " + reg_a + ", " + to_string(mem_basic_addr + STM.ST[A].addr);
                obj.push_back(code);
            }
            return reg_a;
        }
        else{
            string reg_a = "$" + to_string(reg_basic_index + get_reg(A));
            RDL[get_reg(A)].second = QT_A_L;
            return reg_a;
        }
    }
    else if(get_empty_reg() == -1){
        if(get_reg(A) == -1){
            int reg = full_index % 16;
            full_index++;
            string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
            obj.push_back(code_1);
            RDL[reg] = make_pair(A, QT_A_L);
            string reg_a = "$" + to_string(reg_basic_index + reg);
            if(load){
                string code_2 = "lw " + reg_a + ", " + to_string(mem_basic_addr + STM.ST[A].addr);
                obj.push_back(code_2);
            }
            return reg_a;
        }
        else{
            string reg_a = "$" + to_string(reg_basic_index + get_reg(A));
            RDL[get_reg(A)].second = QT_A_L;
            return reg_a;
        }
    }
    return reg_a;
}

int Generator::get_empty_reg(){
    int index = -1;
    for(int i = 0; i < RDL.size() ; ++i){
        if(RDL[i].first == "") return i;
    }
    for(int i = 0; i < RDL.size() ; ++i){
        if(RDL[i].second == "n") return i;
    }
    return -1;
} 

int Generator::get_reg(string var_name){
    int index = -1;
    for(int i = 0; i < RDL.size() ; ++i){
        if(RDL[i].first == var_name) return i;
    }
    return -1;
}

vector<string> Generator::scan_QT(vector<string> QT, symbol_table_system STM){
    vector<string> ans;
    for(int i = QT.size() - 1 ; i >= 0 ; --i){
        string B, C, A, op;
        int cnt = 0;
        for(int j = 0 ; j < QT[i].size() ; ++j){
            if(cnt == 0 && QT[i][j] == ' '){
                int index = j + 1;
                int len = 0;
                while(QT[i][index] != ' '){
                    len++;
                    index++;
                }
                op = QT[i].substr(j + 1, len);
                // cout << op << endl;
                cnt++;
            }
            if(QT[i][j] == ',' && cnt == 1){
                int len = 0;
                int index = j;
                while(QT[i][index] != ' '){
                    len++;
                    index--;
                }

                B = QT[i].substr(index + 1, len - 1);
                // cout << B << endl;
                cnt++;
            }
            else if(QT[i][j] == ',' && cnt == 2){
                int index = j;
                int len = 0;
                while(QT[i][index] != ' '){
                    len++;
                    index--;
                }
                C = QT[i].substr(index + 1, len - 1);
                // cout << C << endl;
                index = j + 2;
                len = 0;
                while(QT[i][index] != ' '){
                    len++;
                    index++;
                }
                A = QT[i].substr(j + 2, len);
                // cout << A << endl;
                break;
            }
        }
        string QT_A_L = " ";
        if(STM.ST.find(A) != STM.ST.end()){
            QT_A_L = STM.ST[A].L;
            STM.ST[A].L = "n";
        }
        string QT_B_L = " ";
        string QT_C_L = " ";
        if(STM.ST.find(B) != STM.ST.end()){
            QT_B_L = STM.ST[B].L;
            STM.ST[B].L = to_string(i);
        }
        if(STM.ST.find(C) != STM.ST.end()){
            QT_C_L = STM.ST[C].L;
            STM.ST[C].L = to_string(i);
        }
        string tmp = "(" + op + " " + B + "(" + QT_B_L + ")" + C + "(" + QT_C_L + ")" + A + "(" + QT_A_L + "))";
        ans.push_back(tmp);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}


vector<string> Generator::generate(vector<string> QT, symbol_table_system STM){
    vector<string> obj;
    obj.push_back(".text");
    for(int i = 0 ; i < QT.size() ; ++i){
        string op, B, C, A, QT_A_L, QT_B_L, QT_C_L; 
        int cnt = 0;
        // obj.push_back(QT[i]);
        for(int j = 0 ; j < QT[i].size() ; ++j){
            if(cnt == 0 && QT[i][j] == '('){
                int index = j;
                int len = 0;
                while(QT[i][index] != ' '){
                    len++;
                    index++;
                }
                op = QT[i].substr(j + 1, len - 1);
                // cout << op << endl;
                cnt++;
            }
            else if(QT[i][j] == '(' && cnt == 1){
                int index = j;
                int len = 0;
                while(QT[i][index] != ' '){
                    len++;
                    index--;
                }
                B = QT[i].substr(index + 1, len - 1);
                cout << B << endl;
                QT_B_L = QT[i].substr(j + 1, 1);
                cout << QT_B_L << endl;
                cnt++;
            }
            else if(QT[i][j] == '(' && cnt == 2){
                int index = j;
                int len = 0;
                while(QT[i][index] != ')'){
                    len++;
                    index--;
                }
                C = QT[i].substr(index + 1, len - 1);
                cout << C << endl;
                QT_C_L = QT[i].substr(j + 1, 1);
                cout << QT_C_L << endl;
                cnt++;
            }
            else if(QT[i][j] == '(' && cnt == 3){
                int index = j;
                int len = 0;
                while(QT[i][index] != ')'){
                    len++;
                    index--;
                }
                A = QT[i].substr(index + 1, len - 1);
                cout << A << endl;
                QT_A_L = QT[i].substr(j + 1, 1);
                cout << QT_A_L << endl;
                cnt++;
            }
        }
        
        string reg_a = "", reg_b = "", reg_c = "";
        if(STM.ST.find(B) != STM.ST.end()){
            reg_b = alloc_reg(B, QT_B_L, STM, obj);
        }
        if(STM.ST.find(C) != STM.ST.end()){
            reg_c = alloc_reg(C, QT_C_L, STM, obj);
        }
        if(STM.ST.find(A) != STM.ST.end()){
            // cout << A << endl;
            reg_a = alloc_reg(A, QT_A_L, STM, obj, false);
        }
        // cout << "a " << reg_a << "b " << reg_b << "c " << reg_c << endl;
        if(op == "+"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) + stoi(C));
                reg_b = reg_a;
            }
            else{
                if(QT_B_L == " "){
                    reg_b = reg_c;
                    reg_c = B;
                }
                if(QT_C_L == " "){
                    reg_c = C;
                }
            }
            string code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
            obj.push_back(code);
        }
        else if(op == "-"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) - stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "sub " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "sub " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "sub " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "*"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) * stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    reg_b = reg_c;
                    reg_c = B;
                }
                if(QT_C_L == " "){
                    reg_c = C;
                }
            }
            string code = "mul " + reg_a + ", " + reg_b + ", " + reg_c;
            obj.push_back(code);
        }
        else if(op == "/"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) / stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "div " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "div " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "div " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "="){
            if(reg_a == reg_b){
                continue;
            }
            string code_1 =  "sub " + reg_a + ", " + reg_a + ", " + reg_a;
            obj.push_back(code_1);
            if(QT_B_L == " "){
                reg_b = B;
                string code_2 = "add " + reg_a + ", " + reg_a + ", " + reg_b;
                obj.push_back(code_2);
            }
            else{
                string code_2 = "add " + reg_a + ", " + reg_a + ", " + reg_b;
                obj.push_back(code_2);
            }
        }
        else if(op == ">"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) > stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "sgt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "sgt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "sgt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "<"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) < stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "slt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "slt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "slt " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == ">="){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) >= stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "sge " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "sge " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "sge " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "<="){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) <= stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    int tmp_reg = get_empty_reg();
                    if(tmp_reg == -1){
                        int reg = full_index % 16;
                        full_index++;
                        string code_1 = "sw $" + to_string(reg_basic_index + reg) + ", " + to_string(mem_basic_addr + STM.ST[RDL[reg].first].addr);
                        obj.push_back(code_1);
                        RDL[reg] = make_pair("", "n");
                        reg_b = "$" + to_string(reg_basic_index + reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    else{
                        reg_b = "$" + to_string(reg_basic_index + tmp_reg);
                        string code_2 = "sub " + reg_b + ", " + reg_b + ", " + reg_b;                        
                        obj.push_back(code_2);
                        string code_3 = "add " + reg_b + ", " + reg_b + ", " + B;
                        obj.push_back(code_3);
                    }
                    string code = "sle " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "sle " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "sle " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "=="){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) == stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    reg_b = reg_c;
                    reg_c = B;
                    string code = "seq " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "seq " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "seq " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "!="){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) != stoi(C));
                reg_b = reg_a;
                string code = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code);
                code = "add " + reg_a + ", " + reg_b + ", " + reg_c;
                obj.push_back(code);
            }
            else{
                if(QT_B_L == " "){
                    reg_b = reg_c;
                    reg_c = B;
                    string code = "sne " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else if(QT_C_L == " "){
                    reg_c = C;
                    string code = "sne " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
                else{
                    string code = "sne " + reg_a + ", " + reg_b + ", " + reg_c;
                    obj.push_back(code);
                }
            }
        }
        else if(op == "if"){
            string code = "beqz " + reg_b + ", fj" + to_string(f_cnt);
            obj.push_back(code);

        }  
        else if(op == "el"){
            string code = "b tj" + to_string(t_cnt);
            obj.push_back(code);
            string tmp = obj[obj.size() - 1];
            obj[obj.size() - 1] = obj[obj.size() - 2];
            obj[obj.size() - 2] = tmp;
        }
        else if(op == "ie"){            
            for(int k = obj.size() - 1 ; k >= 0 ; --k){
                if(obj[k].substr(0,2) == "be"){
                    string code = "fj" + to_string(f_cnt) + ": ";
                    obj.push_back(code);
                    f_cnt++;
                    break;
                }
                else if(obj[k].substr(0,2) == "b "){
                    string code = "tj" + to_string(t_cnt) + ": ";
                    t_cnt++;
                    obj.push_back(code);
                    break;
                }
            }
        }
    }
    for(int i = 0 ; i < RDL.size() ; ++i){
        if(RDL[i].second == "y"){
            string code = "sw $" + to_string(reg_basic_index + i) + ", " + to_string(mem_basic_addr + STM.ST[RDL[i].first].addr);
            obj.push_back(code);
            RDL[i].first = "";
        }
    }
    return obj;
}
