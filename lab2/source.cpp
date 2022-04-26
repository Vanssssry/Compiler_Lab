#include "parser.h"
#include "scanner.h"
// #include "utilizer.h"
using namespace std;



int main(){
    Scanner scanner;
    LL1 parser;
    // LR parser;
    string filename;
    cin >> filename;
    string token_seq = scanner.scan(filename, "no");
    vector<string> token_arr = pre_process(token_seq);
    cout << token_seq << endl;
    // cout << parser.check(token_seq) << endl;
    if(parser.check(token_arr)){
        cout << "Yes! What a genuis!" << endl;
    }
}