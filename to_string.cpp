#include<iostream>
#include<string.h>

using namespace std;

string to_string(int val){

    char temp[10];
    int k = 0;

    if(val < 0){
        temp[k] = '-';
        k++;
        val = -val;
    }

    int mod;
    int digits[10],n = 0;
    int tem = val;

    while(tem != 0){
        mod = tem%10;
        digits[n] = mod;
        tem = tem/10;
        n++;
    }

    for(int i = 1; i <= n; i++){
        temp[k] = digits[n-i]+48;
        k++;
    }

    temp[k] = '\0';

    return (string)temp;
}


