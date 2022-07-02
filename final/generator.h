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
        vector<pair<int, string> > scan_QT(vector<pair<int, string> > QT, symbol_table_system STM);
        vector<string> generate(vector<string> QT, symbol_table_system STM);
        vector<vector<pair<int, string> > > get_block(vector<string> QT);
        int get_reg(string var_name);
        int get_empty_reg();
        string alloc_reg(string A, string QT_A_L, symbol_table_system stm, vector<string> & obj, string var_name, bool load);

        vector<string> complie(vector<vector<pair<int,string> > > block, symbol_table_system STM, int code_num);
};

Generator::Generator(){
    for(int i = 0 ; i < 16 ; ++i){
        RDL.push_back(make_pair("", ""));
    }
}

string Generator::alloc_reg(string A, string QT_A_L, symbol_table_system STM, vector<string> & obj, string var_name, bool load = true){
    string reg_a;
    if(get_empty_reg() != -1){
        if(get_reg(A) == -1){
            int reg = get_empty_reg();
            if(RDL[reg].first == var_name){
                int var_reg = reg;
                RDL[reg].second = "y";
                reg = get_empty_reg();
                RDL[reg] = make_pair(A, QT_A_L);
                RDL[var_reg].second = "n";
                string reg_a = "$" + to_string(reg_basic_index + reg);
                if(load){
                    string code = "lw " + reg_a + ", " + to_string(mem_basic_addr + STM.ST[A].addr);
                    obj.push_back(code);
                }
                return reg_a;
            }
            else{
                RDL[reg] = make_pair(A, QT_A_L);
                string reg_a = "$" + to_string(reg_basic_index + reg);
                if(load){
                    string code = "lw " + reg_a + ", " + to_string(mem_basic_addr + STM.ST[A].addr);
                    obj.push_back(code);
                }
                return reg_a;
            }
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
        if(RDL[i].first == "" || RDL[i].second == "n") return i;
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

vector<pair<int, string> > Generator::scan_QT(vector<pair<int, string> > QT, symbol_table_system STM){
    vector<pair<int, string> > ans;
    for(int i = QT.size() - 1 ; i >= 0 ; --i){
        string B, C, A, op;
        int cnt = 0;
        for(int j = 0 ; j < QT[i].second.size() ; ++j){
            if(cnt == 0 && QT[i].second[j] == ' '){
                int index = j + 1;
                int len = 0;
                while(QT[i].second[index] != ' '){
                    len++;
                    index++;
                }
                op = QT[i].second.substr(j + 1, len);
                // cout << op << endl;
                cnt++;
            }
            if(QT[i].second[j] == ',' && cnt == 1){
                int len = 0;
                int index = j;
                while(QT[i].second[index] != ' '){
                    len++;
                    index--;
                }

                B = QT[i].second.substr(index + 1, len - 1);
                // cout << B << endl;
                cnt++;
            }
            else if(QT[i].second[j] == ',' && cnt == 2){
                int index = j;
                int len = 0;
                while(QT[i].second[index] != ' '){
                    len++;
                    index--;
                }
                C = QT[i].second.substr(index + 1, len - 1);
                // cout << C << endl;
                index = j + 2;
                len = 0;
                while(QT[i].second[index] != ' '){
                    len++;
                    index++;
                }
                A = QT[i].second.substr(j + 2, len);
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
        ans.push_back(make_pair(QT[i].first, tmp));
    }
    reverse(ans.begin(), ans.end());
    return ans;
}



vector<vector<pair<int, string> > > Generator::get_block(vector<string> QT){
    vector<vector<pair<int, string> > > block;
    vector<pair<int, string> > block_0;
    int index = 0;
    while(QT[index + 1].substr(2, 2) != "if"){
        // cout << QT[index].substr(2, 2) << endl;
        block_0.push_back(make_pair(index, QT[index]));
        index++;
    }
    index++;
    block.emplace_back(block_0);
    // cout << "block_0" << endl;
    // for(auto s: block_0){
    //     cout << s.first << " " << s.second << endl;
    // }
    
    for(int i = index ; i < QT.size() ; ++i){
        // cout << QT[i].substr(2, 2) << endl;
        if(QT[i].substr(2, 2) == "if"){
            stack<pair<int,string> > st;
            // cout << QT[i - 1] << "**" << endl;
            st.push(make_pair(i - 1, QT[i - 1]));
            // cout << QT[i] << "**" << endl;
            st.push(make_pair(i, QT[i]));
            i++;
            while(!st.empty()){
                vector<pair<int, string> > tmp;
                while(QT[i].substr(2, 2) != "ie"){
                    // cout << QT[i] << "**" << endl;
                    st.push(make_pair(i, QT[i]));
                    i++;
                }
                // cout << QT[i] << "*" << endl;
                tmp.push_back(make_pair(i, QT[i]));
                i++;
                while(st.top().second.substr(2, 2) != "if" && st.top().second.substr(2, 2) != "el"){
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
                if(st.top().second.substr(2, 2) == "if"){    
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                    curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
                else{
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
                reverse(tmp.begin(), tmp.end());
                // cout << "block" << endl;
                // for(auto s: tmp){
                //     cout << s.first << " " << s.second << endl;
                // }
                block.emplace_back(tmp);
            }
            i--;
        }
        else if(QT[i].substr(2, 2) == "el"){
            stack<pair<int,string> > st;
            // cout << QT[i] << "**" << endl;
            st.push(make_pair(i,QT[i]));
            i++;
            while(!st.empty()){
                vector<pair<int, string> > tmp;
                while(QT[i].substr(2, 2) != "ie"){
                    // cout << QT[i] << "**" << endl;
                    st.push(make_pair(i, QT[i]));
                    i++;
                }
                // cout << QT[i] << "*" << endl;
                tmp.push_back(make_pair(i, QT[i]));
                i++;
                while(st.top().second.substr(2, 2) != "if" && st.top().second.substr(2, 2) != "el"){
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
            
                if(st.top().second.substr(2, 2) == "if"){    
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                    curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
                else{
                    string curr = st.top().second;
                    tmp.emplace_back(make_pair(st.top().first, curr));
                    // cout << curr << "*" << endl;
                    st.pop();
                }
            
                reverse(tmp.begin(), tmp.end());
                // cout << "block" << endl;
                // for(auto s: tmp){
                //     cout << s << endl;
                // }
                block.emplace_back(tmp);
            }
            i--;
        }
        else{
            vector<pair<int, string> > tmp;
            while(QT[i].substr(2, 2) != "if"){
                tmp.emplace_back(make_pair(i, QT[i]));
                i++;
            }
            i--;
            block.emplace_back(tmp);
        }
        // cout << "block_tmp" << endl;
        // for(auto s: tmp){
        //     cout << s << endl;
        // }
    }
    // cout << block.size() << endl;
    // for(int i = 1 ; i < block.size() ; ++i){
    //     // cout << block[i][0] << endl;
    //     if(block[i][0].substr(2, 2) == "if"){
    //         block[i].insert(block[i].begin(), block[i - 1][block[i - 1].size() - 1]);
    //         block[i - 1].pop_back();
    //         // cout << block[i][0] << endl;
    //     }
    // }
    // for(int i = 0 ; i < block.size() ; ++i){
    //     cout << "block" << i << endl;
    //     for(auto s: block[i]){
    //         cout << s << endl;
    //     }
    // }
    // cout << block.size() << endl;
    return block;
}


vector<string> Generator::generate(vector<string> QT, symbol_table_system STM){
    vector<string> obj;
    obj.emplace_back(".text");
    // obj.push_back("");
    stack<string> j_st;
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
                // cout << B << endl;
                QT_B_L = QT[i].substr(j + 1, 1);
                // cout << QT_B_L << endl;
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
                // cout << C << endl;
                QT_C_L = QT[i].substr(j + 1, 1);
                // cout << QT_C_L << endl;
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
                // cout << A << endl;
                QT_A_L = QT[i].substr(j + 1, 1);
                // cout << QT_A_L << endl;
                cnt++;
            }
        }
        
        string reg_a = "", reg_b = "", reg_c = "";
        if(STM.ST.find(B) != STM.ST.end()){
            reg_b = alloc_reg(B, QT_B_L, STM, obj, C);
            // obj.push_back(B + ": " + reg_b);
        }
        if(STM.ST.find(C) != STM.ST.end()){
            reg_c = alloc_reg(C, QT_C_L, STM, obj, B);
            // obj.push_back(C + ": " + reg_c);
        }
        if(STM.ST.find(A) != STM.ST.end()){
            // cout << A << endl;
            reg_a = alloc_reg(A, QT_A_L, STM, obj, B, false);
            // obj.push_back(A + ": " + reg_a);
        }
        // cout << "a " << reg_a << "b " << reg_b << "c " << reg_c << endl;
        if(op == "+"){
            if(QT_B_L == " " && QT_C_L == " "){
                reg_c = to_string(stoi(B) + stoi(C));
                string code1 = "sub " + reg_a + ", " + reg_a + ", " + reg_a;
                obj.push_back(code1);
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
                // obj.push_back(reg_a + " " + reg_b);
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
            // cout << "if" << endl;
            string code = "beqz " + reg_b + ", fj" + to_string(f_cnt);
            f_cnt++;
            j_st.push(code);
            obj.push_back(code);

        }  
        else if(op == "el"){
            // cout << "el" << endl;
            // int f_index = fj_st.top();
            // fj_st.pop();
            // string code = "fj" + to_string(f_index) + ": ";
            string code = "b tj" + to_string(t_cnt);
            j_st.push(code);
            t_cnt++;
            obj.push_back(code);
            string tmp = obj[obj.size() - 1];
            obj[obj.size() - 1] = obj[obj.size() - 2];
            obj[obj.size() - 2] = tmp;

        }
        else if(op == "ie"){
            // cout << j_st.top() << endl;   
            string j_state = j_st.top(); 
            j_st.pop(); 
            if(j_state.substr(0,2) == "be"){
                string code = "";
                for(int k = j_state.size() - 1 ; k >= 0 ; --k){
                    if(j_state[k] == ' '){
                        break;
                    }
                    code += j_state[k];
                }
                reverse(code.begin(), code.end());
                code += ": ";
                // cout << code;
                obj.push_back(code);
            }      
            else if(j_state.substr(0,2) == "b "){
                string code = "";
                for(int k = j_state.size() - 1 ; k >= 0 ; --k){
                    if(j_state[k] == ' '){
                        break;
                    }
                    code += j_state[k];
                }
                reverse(code.begin(), code.end());
                code += ": ";

                // cout << code;
                obj.push_back(code);
            }
            // for(int k = obj.size() - 1 ; k >= 0 ; --k){
            //     if(obj[k].substr(0,2) == "be"){
            //         for(int i = 0 ; i < RDL.size() ; ++i){
            //             if(RDL[i].second == "y"){
            //                 string code = "sw $" + to_string(reg_basic_index + i) + ", " + to_string(mem_basic_addr + STM.ST[RDL[i].first].addr);
            //                 obj.push_back(code);
            //                 RDL[i].first = "";
            //                 RDL[i].second = "n";
            //             }
            //         }
            //         string j_state = j_st.top();
            //         int f_index = fj_st.top();
            //         fj_st.pop();
            //         string code = "fj" + to_string(f_index) + ": ";
            //         // string code = "b tj" + to_string(t_cnt);
            //         // tj_st.push(t_cnt);
            //         // t_cnt++;
            //         // string code = "fj" + to_string(f_cnt) + ": ";
            //         obj.push_back(code);
            //         // f_cnt++;
            //         break;
            //         // return obj;
            //     }
            //     else if(obj[k].substr(0,2) == "b "){
            //         for(int i = 0 ; i < RDL.size() ; ++i){
            //             if(RDL[i].second == "y"){
            //                 string code = "sw $" + to_string(reg_basic_index + i) + ", " + to_string(mem_basic_addr + STM.ST[RDL[i].first].addr);
            //                 obj.push_back(code);
            //                 RDL[i].first = "";
            //                 RDL[i].second = "n";
            //             }
            //         }
            //         int tj_index = tj_st.top();
            //         tj_st.pop();
            //         string code = "tj" + to_string(tj_index) + ": ";
            //         // t_cnt++;
            //         obj.push_back(code);
            //         break;
            //         // return obj;
            //     }
            // }
        }
    }
    for(int i = 0 ; i < RDL.size() ; ++i){
        if(RDL[i].second == "y"){
            string code = "sw $" + to_string(reg_basic_index + i) + ", " + to_string(mem_basic_addr + STM.ST[RDL[i].first].addr);
            obj.push_back(code);
            RDL[i].first = "";
            RDL[i].second = "n";
        }
    }
    return obj;
}


vector<string> Generator::complie(vector<vector<pair<int,string> > > block, symbol_table_system STM, int code_num){
    vector<string> obj;
    vector<pair<int,string> > QT_block;
    vector<string> QT_F;
    // vector<string> head;
    // head.push_back(".text");
    // obj.emplace_back(".text");
    for(int i = 0 ; i < block.size() ; ++i){
        vector<pair<int,string> > QT_L = scan_QT(block[i], STM);
        for(auto p : QT_L){
            QT_block.push_back(p);
        }
        // if(QT_L.size() > 0){
        //     vector<string> block_obj = generate(QT_L, STM);
        //     obj.emplace_back(block_obj);
        // }
    }
    for(int i = 0 ; i < code_num ; ++i){
        for(int j = 0 ; j < QT_block.size(); ++j){
            if(QT_block[j].first == i){
                QT_F.push_back(QT_block[j].second);
            }
            // break;
        }
    }
    // for(auto s : QT_F){
    //     cout << s << endl;
    // }
    obj = generate(QT_F, STM);
    return obj;
}