#include "scanner.h"
#include <string>
using namespace std;

int main(int argc, char** argv){
    Scanner scanner;
    if(argv[1] != NULL){
        string filename = argv[1];
        scanner.scan(filename);
    }
    else{
        string filename;
        cin >> filename;
        scanner.scan(filename);
    }
}