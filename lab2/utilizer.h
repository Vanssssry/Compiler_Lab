#include <vector>
#include <string>
using namespace std;

vector<string> pre_process(string token_seq){
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
    return token_arr;
}