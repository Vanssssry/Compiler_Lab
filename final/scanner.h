#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "symbol_table.h"
using namespace std;

typedef unordered_map<string, string> StringMap;
typedef unordered_map<string, int> CharMap;

StringMap KT({{"break","04"},{"case","05"},{"char","06"},{"const","07"},{"continue","08"},{"default","09"},
             {"do","10"},{"double","11"},{"else","12"},{"enum","13"},{"extern","14"},{"float","15"},
             {"for","16"},{"goto","17"},{"if","18"},{"int","19"},{"long","20"},{"register","21"},{"return","22"},
             {"short","23"},{"signed","24"},{"sizeof","25"},{"static","26"},{"struct","27"},{"switch","28"},{"typedef","29"},
             {"union","30"},{"unsigned","31"},{"void","32"},{"volatile","33"},{"while","34"}});

StringMap PT({{">=","35"},{"<=","36"},{"==","37"},{"!=","38"},{"=","39"},{">","40"},{"<","41"},{"%","42"},
             {"+","43"},{"+=","44"},{"++","45"},{"-","46"},{"-=","47"},{"--","48"},{"*","49"},{"*=","50"},
             {"/","51"},{"/=","52"},{"(","53"},{")","54"},{"{","55"},{"}","56"},{",","57"},{";","58"},{"[","59"},
             {"]","60"},{"|","61"},{"&","62"},{"^","63"},{"!","64"},{"<<","65"},{">>","66"},{"->","67"},{".","68"},
             {"#","69"},{"||","70"},{"&&","71"}});



class Scanner
{
    private:
        vector<CharMap> DFA;
        StringMap identify_T;
        StringMap char_T;
        StringMap string_T;
        StringMap constant_T;

    public:
        void build_DFA();
        Scanner(){
            build_DFA();
        }
        string scan(string file_name, string mode, symbol_table_system & STM);
        int state_change(int state, char c);
        string state2code(int state, string token);
};

void Scanner::build_DFA(){
    CharMap state1({{"blank", 1}, {"letter", 2}, {"digit", 3}, {"single quote", 4}, {"double quote", 5}, {"greater", 6}, {"less", 7}, 
                   {"equal", 8}, {"not", 9}, {"plus", 10}, {"minus", 11}, {"multiply", 12}, {"divide", 13}, {"or", 14}, {"and", 15}, 
                   {"symbol", 16}});
    CharMap state2({{"letter", 2}, {"digit", 2}, {"other", 0}, {"end", -1}});
    CharMap state3({{"digit", 3}, {"dot", 17}, {"e", 18},{"other", 0}, {"end", -2}});
    CharMap state4({{"char", 19},{"trans", 38}, {"single quote", 40}});
    CharMap state5({{"char", 5}, {"double quote", 39}});
    CharMap state6({{"equal", 20}, {"greater", 21}, {"end", -5}, {"other", 0}});
    CharMap state7({{"equal", 22}, {"less", 23}, {"end", -5}, {"other", 0}});
    CharMap state8({{"equal", 24}, {"end", -5}, {"other", 0}});
    CharMap state9({{"equal", 25}, {"end", -5}, {"other", 0}});
    CharMap state10({{"equal", 26}, {"plus", 35},{"end", -5}, {"other", 0}});
    CharMap state11({{"equal", 27}, {"minus", 36}, {"greater", 37}, {"end", -5}, {"other", 0}});
    CharMap state12({{"equal", 28}, {"end", -5}, {"other", 0}});
    CharMap state13({{"equal", 29}, {"divide", 41}, {"star", 42}, {"end", -5}, {"other", 0}});
    CharMap state14({{"or", 30}, {"end", -5}, {"other", 0}});
    CharMap state15({{"and", 31}, {"end", -5}, {"other", 0}});
    CharMap state16({{"end", -5}, {"other", 0}});
    CharMap state17({{"digit", 32}, {"other", 0}});
    CharMap state18({{"digit", 33}, {"negtive", 34}, {"end", -2}, {"other", 0}});
    CharMap state19({{"single quote", 40}, {"other", 0}});
    CharMap state20({{"end", -5}});
    CharMap state21({{"end", -5}});
    CharMap state22({{"end", -5}});
    CharMap state23({{"end", -5}});
    CharMap state24({{"end", -5}});
    CharMap state25({{"end", -5}});
    CharMap state26({{"end", -5}});
    CharMap state27({{"end", -5}});
    CharMap state28({{"end", -5}});
    CharMap state29({{"end", -5}});
    CharMap state30({{"end", -5}});
    CharMap state31({{"end", -5}});
    CharMap state32({{"digit", 32}, {"e", 18}, {"end", -2}});
    CharMap state33({{"digit", 33}, {"dot", 17},{"end", -2}});
    CharMap state34({{"digit", 33},{"other", 0}});
    CharMap state35({{"end", -5}});
    CharMap state36({{"end", -5}});
    CharMap state37({{"end", -5}});
    CharMap state38({{"char", 19}});
    CharMap state39({{"end", -4}});
    CharMap state40({{"end", -3}});
    CharMap state41({{"end", -6}});
    CharMap state42({{"char", 42}, {"star", 43}});
    CharMap state43({{"divide", 44},{"other", 0}});
    CharMap state44({{"end", -7}});

    DFA.resize(45);
    DFA[1] = state1;
    DFA[2] = state2;
    DFA[3] = state3;
    DFA[4] = state4;
    DFA[5] = state5;
    DFA[6] = state6;
    DFA[7] = state7;
    DFA[8] = state8;
    DFA[9] = state9;
    DFA[10] = state10;
    DFA[11] = state11;
    DFA[12] = state12;
    DFA[13] = state13;
    DFA[14] = state14;
    DFA[15] = state15;
    DFA[16] = state16;
    DFA[17] = state17;
    DFA[18] = state18;
    DFA[19] = state19;
    DFA[20] = state20;
    DFA[21] = state21;
    DFA[22] = state22;
    DFA[23] = state23;
    DFA[24] = state24;
    DFA[25] = state25;
    DFA[26] = state26;
    DFA[27] = state27;
    DFA[28] = state28;
    DFA[29] = state29;
    DFA[30] = state30;
    DFA[31] = state31;
    DFA[32] = state32;
    DFA[33] = state33;
    DFA[34] = state34;
    DFA[35] = state35;
    DFA[36] = state36;
    DFA[37] = state37;
    DFA[38] = state38;
    DFA[39] = state39;
    DFA[40] = state40;
    DFA[41] = state41;
    DFA[42] = state42;
    DFA[43] = state43;
    DFA[44] = state44;
}

string Scanner::state2code(int state, string token){
    string token_code = "";
    switch (state)
    {
        case -1:
            if(KT.count(token)){
                token_code = KT[token];
            }
            else{
                if(identify_T.count(token) == 0)
                    identify_T[token] = "00";
                token_code = identify_T[token];
            }
            break;
        case -2:
            if(constant_T.count(token) == 0)
                constant_T[token] = "03";
            token_code = constant_T[token];
            break;
        case -3:
            if(char_T.count(token) == 0)
                char_T[token] = "01";
            token_code = char_T[token];
            break;
        case -4:
            if(string_T.count(token) == 0)
                string_T[token] = "02";
            token_code = string_T[token];
            break;
        case -5:
            if(PT.count(token)){
                token_code = PT[token];
            }
            else{
                token_code = "-1";
            }
            break;
        default:
            token_code = "-1";
            break;
    }
    return token_code;
}

int Scanner::state_change(int state, char c){
    int next_state = 0;
    switch (state)
    {
        case 1:
            if(c == ' ' || c == '\n'){
                next_state = DFA[state]["blank"];
            }
            else if(c == '_' || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
                next_state = DFA[state]["letter"];
            }
            else if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else if(c == '\''){
                next_state = DFA[state]["single quote"];
            }
            else if(c == '"'){
                next_state = DFA[state]["double quote"];
            }
            else if(c == '>'){
                next_state = DFA[state]["greater"];
            }
            else if(c == '<'){
                next_state = DFA[state]["less"];
            }
            else if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '!'){
                next_state = DFA[state]["not"];
            }
            else if(c == '+'){
                next_state = DFA[state]["plus"];
            }
            else if(c == '-'){
                next_state = DFA[state]["minus"];
            }
            else if(c == '*'){
                next_state = DFA[state]["multiply"];
            }
            else if(c == '/'){
                next_state = DFA[state]["divide"];
            }
            else if(c == '|'){
                next_state = DFA[state]["or"];
            }
            else if(c == '&'){
                next_state = DFA[state]["and"];
            }
            else{
                next_state = DFA[state]["symbol"];
            }
            break;
        case 2:
            if(c == '_' || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
                next_state = DFA[state]["letter"];
            }
            else if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 3:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else if(c == '.'){
                next_state = DFA[state]["dot"];
            }
            else if(c == 'e'){
                next_state = DFA[state]["e"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 4:
            if(c == '\\'){
                next_state = DFA[state]["trans"];
            }
            if(c == '\''){
                next_state = DFA[state]["single quote"];
            }
            else{
                next_state = DFA[state]["char"];
            }
            break;
        case 5:
            if(c == '"'){
                next_state = DFA[state]["double quote"];
            }
            else{
                next_state = DFA[state]["char"];
            }
            break;
        case 6:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '>'){
                next_state = DFA[state]["greater"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 7:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '<'){
                next_state = DFA[state]["less"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 8:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 9:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 10:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '+'){
                next_state = DFA[state]["plus"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 11:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '-'){
                next_state = DFA[state]["minus"];
            }
            else if(c == '>'){
                next_state = DFA[state]["greater"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 12:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 13:
            if(c == '='){
                next_state = DFA[state]["equal"];
            }
            else if(c == '/'){
                next_state = DFA[state]["divide"];
            }
            else if(c == '*'){
                next_state = DFA[state]["star"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 14:
            if(c == '|'){
                next_state = DFA[state]["or"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 15:
            if(c == '&'){
                next_state = DFA[state]["and"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 16:
            next_state = DFA[state]["end"];
            break;
        case 17:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else{
                next_state = DFA[state]["other"];
            }
            break;
        case 18:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else if(c == '-'){
                next_state = DFA[state]["negtive"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 19:
            if(c == '\''){
                next_state = DFA[state]["single quote"];
            }
            else{
                next_state = DFA[state]["other"];
            }
            break;
        case 20:
            next_state = DFA[state]["end"];
            break;
        case 21:
            next_state = DFA[state]["end"];
            break;
        case 22:
            next_state = DFA[state]["end"];
            break;
        case 23:
            next_state = DFA[state]["end"];
            break;
        case 24:
            next_state = DFA[state]["end"];
            break;
        case 25:
            next_state = DFA[state]["end"];
            break;
        case 26:
            next_state = DFA[state]["end"];
            break;
        case 27:
            next_state = DFA[state]["end"];
            break;
        case 28:
            next_state = DFA[state]["end"];
            break;
        case 29:
            next_state = DFA[state]["end"];
            break;
        case 30:
            next_state = DFA[state]["end"];
            break;
        case 31:
            next_state = DFA[state]["end"];
            break;
        case 32:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else if(c == 'e'){
                next_state = DFA[state]["e"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 33:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else if(c == '.'){
                next_state = DFA[state]["dot"];
            }
            else{
                next_state = DFA[state]["end"];
            }
            break;
        case 34:
            if(c >= 48 && c <= 57){
                next_state = DFA[state]["digit"];
            }
            else{
                next_state = DFA[state]["other"];
            }
            break;
        case 35:
            next_state = DFA[state]["end"];
            break;
        case 36:
            next_state = DFA[state]["end"];
            break;
        case 37:
            next_state = DFA[state]["end"];
            break;
        case 38:
            next_state = DFA[state]["char"];
            break;
        case 39:
            next_state = DFA[state]["end"];
            break;
        case 40:
            next_state = DFA[state]["end"];
            break;
        case 41:
            next_state = DFA[state]["end"];
            break;
        case 42:
            if(c == '*'){
                next_state = DFA[state]["star"];
            }
            else{
                next_state = DFA[state]["char"];
            }            
            break;
        case 43:
            if(c == '/'){
                next_state = DFA[state]["divide"];
            }
            else{
                next_state = DFA[state]["other"];
            }
            break;
        case 44:
            next_state = DFA[state]["end"];
            break;


        default:
            next_state = DFA[state]["other"];
            break;
    }
    return next_state;
}

string Scanner::scan(string filename, string mode, symbol_table_system & STM){
    ifstream infile;
    infile.open(filename, ios::in);
    int state = 1;
    string line;
    string token_code;
    string token = "";
    string seq = "";
    while(getline(infile, line)){
        int i = 0;
        // cout << line << endl;
        while(i < line.size()){
            char c = line[i];
            state = state_change(state, c);
            if(state > 1){
                token += c;
                seq += to_string(state) + ",";
            }
            if(state == -6){
                state = 1;  
                token = "";
                break;
            }                
            if(state <= 0){
                if(state == -7){
                    state = 1;  
                    token = "";
                    seq = "";
                    continue;
                }
                // token_code += "<" + token + ", " + state2code(state, token) + ">" + to_string(state) + " " + seq + "\n";
                string code = state2code(state, token);
                if(code == "-1"){
                    cout << "token error!" << endl;
                    return "";
                }
                token_code += "[" + token + "," + code + "]";
                if(code == "00"){
                    table_item tmp(token, "", "", "", -1);
                    STM.ST[token] = tmp;
                }
                if(code == "03"){
                    STM.CT[token] = stoi(token);
                }
                // token_code += "<" + state2code(state, token) + ">";
                state = 1;  
                token = "";
                seq = "";
                i--;
            }
            i++;
        }
    }
    if(state > 0 && state != 1){
        state = state_change(state, '#');
        token_code += "[" + token + "," + state2code(state, token) + "]";
        // token_code += "<" + state2code(state, token) + ">";
    }
    infile.close();
    if(mode == "o"){
        ofstream outfile("token.txt", ios::out);
        outfile << token_code;
        outfile.close();
        return token_code;
    }
    else{
        return token_code;
    }
}