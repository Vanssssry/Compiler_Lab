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
    vector<pair<string,string> > token_arr = pre_process(token_seq);
    for(int i = 0 ; i < token_arr.size() ; ++i){
        cout << token_arr[i].first << " " << token_arr[i].second << endl;
    }
    // cout << token_seq << endl;
    // cout << parser.check(token_arr) << endl;
    if(parser.check(token_arr, stm)){
        cout << "Expression LL1 correct!" << endl;
        parser.print_QT();
    }
    // for(auto it : stm.ST){
    //     cout << it.first << " " << it.second.name << " " << it.second.type << " " << it.second.cat << " " << it.second.L << " " << it.second.addr << " " << endl;
    // }
    // for(auto it : stm.CT){
    //     cout << it.first << " " << it.second << endl;
    // }
    vector<string> QT = gen.scan_QT(parser.get_QT(), stm);
    for(auto s : QT){
        cout << s << endl;
    }
    vector<string> obj = gen.generate(QT, stm);
    for(auto s : obj){
        cout << s << endl;
    }
}