#include "parser.h"
#include "scanner.h"
#include "generator.h"
#include "symbol_table.h"
// #include "utilizer.h"
using namespace std;



int main(){
    Scanner scanner;
    LL1 parser;
    symbol_table_system stm;
    Generator gen;
    // LR parser;
    string filename;
    cin >> filename;
    string token_seq = scanner.scan(filename, "no", stm);
    // cout << "token sequence: " << token_seq << endl;
    vector<pair<string,string> > token_arr = pre_process(token_seq);
    // for(int i = 0 ; i < token_arr.size() ; ++i){
    //     cout << token_arr[i].first << " " << token_arr[i].second << endl;
    // }
    // cout << token_seq << endl;
    // cout << parser.check(token_arr) << endl;
    if(parser.check(token_arr, stm)){
        cout << "Expression LL1 correct!" << endl;
        parser.print_QT();
        cout << "************* " << endl; 
    }
    // for(auto it : stm.ST){
    //     cout << it.first << " " << it.second.name << " " << it.second.type << " " << it.second.cat << " " << it.second.L << " " << it.second.addr << " " << endl;
    // }
    // for(auto it : stm.CT){
    //     cout << it.first << " " << it.second << endl;
    // }
    int code_num = parser.get_QT().size();
    vector<vector<pair<int, string> > > block = gen.get_block(parser.get_QT());
    vector<string> obj = gen.complie(block, stm, code_num);
    // for(auto v: block){
    //     cout << "block" << endl;
    //     for(auto s: v){
    //         cout << s.first << " " << s.second << endl;
    //     }
    // }
    for(auto s : obj){
        cout << s << endl;
    }
    // vector<string> QT = gen.scan_QT(parser.get_QT(), stm);
    // cout << "QT with L :" << endl;
    // for(auto s : QT){
    //     cout << s << endl;
    // }
    // vector<string> obj = gen.generate(QT, stm);
    // cout << "target code: " << endl;
    // for(auto s : obj){
    //     cout << s << endl;
    // }
}