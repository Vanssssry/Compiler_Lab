#include <vector>
#include <string>
using namespace std;

vector<pair<string,string> > pre_process(string token_seq){
    vector<pair<string,string> > token_arr;
    for(int i = 0 ; i < token_seq.size() ; ++i){
        if(token_seq[i] == '<') continue;
        string obj = "";
        string token = "";
        while(token_seq[i] != ','){
            obj += token_seq[i];
            i++;
        }
        i++;
        while(token_seq[i] != '>'){
            token += token_seq[i];
            i++;
        }
        token_arr.push_back(make_pair(obj, token));
    }
    return token_arr;
}