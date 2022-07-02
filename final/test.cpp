int a, b, e;
a = 1;
b = 2;
e = a * b + a - b / a;
c = 2;
if (a > b){
    int c;
    c = 1;
    b = a;
    a = a + 1;
}
else{
    a = b;
    b = b + 1;
};
b = 5;
if(a < b){
    if(a < b){
        a = b + 1;
        if(a < b){
            a = b + 1;
        };
    }
    else{
        a = b + 1;
    };
    if(a < b){
        a = b + 1;
    };
}
// if(a > b){
//     b = a;
// }
else{
    a = b;
    if(a > b){
        b = a;
    };
};
