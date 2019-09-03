#include<iostream>
#include<string>
#include<stdlib.h>
#include<conio.h>
//#include "to_string.cpp"

using namespace std;

int lin = 0;    

//Variable Linked List Stack

class var{

public:
    string name;
    int type;
    string value;
    var *next;

    var(){
        type = 'u';
        next = nullptr;
    }

    void addvar(string vname);

    bool assignment(string vname,string value,int valtype);

    string returnval(string vname);

    void intadd(string vname, int adduct);

    int delvar(string vname);

}*prime = nullptr;

//Push a varible to the stack

void var::addvar(string vname){
    if(prime == nullptr){
        prime = new var;
        prime->name = vname;
        prime->next = nullptr;
    }
    else{
        var *temp = new var;
        temp->name = vname;
        temp->next = prime;
        prime = temp;
    }
}

//Assign a value to the variable

bool var::assignment(string vname, string value, int valtype){
    var *temp = prime;

    while(temp){
        if(temp->name == vname){
            temp->type = valtype;
            temp->value = value;
            return true;
        }
        temp = temp->next;
    }

    return false;
}

//Return the value of a given variable

string var::returnval(string vname){
    var *temp = prime;

    bool flag = false;

    while(temp){
        if(temp->name == vname){
            flag = true;
            if(temp->type == 0 || temp->type == 1)
                return temp->value.substr(1,temp->value.length()-2);
            else return temp->value.c_str();
        }
        temp = temp->next;
    }

    if(flag == false){
        cout<<"\n\nInvalid variable token specified in line "<<lin<<endl;
        cout<<"\n\nProgram Terminated.";
        exit(0);
    }
    return "0";
}

//Perform addition to a variable (only used in for loop)

void var::intadd(string vname, int adduct){

    var *temp = prime;

    while(temp){
        if(temp->name == vname){
            int tem = atoi(temp->value.c_str());
            tem += adduct;
            temp->value = to_string(tem);
        }
        temp = temp->next;
    }

}

//Delete topmost occurence of the given variable in the stack

int var::delvar(string vname){

    var *temp = prime, *bridge = prime;

    while(temp){
        if(temp->name == vname){

            if(prime == temp){
                prime = prime->next;
                delete temp;
                return 1;
            }
            else{
                while(bridge->next != temp)
                    bridge = bridge->next;

                bridge->next = (bridge->next)->next;
                delete temp;
                return 1;
            }

        }
        temp = temp->next;
    }

    return 0;
}
