#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<string>
#include<iostream>
#include<sstream>
#include "Expressions.cpp"


/*////////////////////////////////////////////////////////////
                     SPADE interpreter
                        Ver. 0.6.6.6
                    For Further details
                Refer to SPADE Interpreter
                        Ver. 0.6.6.6
////////////////////////////////////////////////////////////*/

using namespace std;

stringstream ss(stringstream::in|stringstream::out);
int currloop = -1;
Post p;

class loop{
public:

    int lcondition, lcounter, llimit, startlin;  //The loop variables
    string conds, counts, limits;
    bool isinloop;

    loop(){
        isinloop = false;                       //default constructor so that I don't need to write this annoying thing in main
        conds = counts = limits = "0";
    }

};

loop loops[30];                                 //Max 30 nested loops

bool loopcheck(){

    // Is the loop condition satisfied? If not, exit the loop

    if((loops[currloop].lcounter > 0 and loops[currloop].lcondition > loops[currloop].llimit) || (loops[currloop].lcounter < 0 and loops[currloop].lcondition <  loops[currloop].llimit) || (loops[currloop].lcounter == 0 and loops[currloop].lcondition == loops[currloop].llimit)){
        loops[currloop].isinloop = false;
        return false;
    }
    else return true;

}

int checktype(string value){
    if(value[0] == '"')
        return 0;       //      String/Character
    else if(value[0] == '\'')
        return 1;       //      Float/Double
    else
        return 2;       //      Integer
}

int checkdot(string st)
{                                               //Checks if the statement ends with the endline delimiter '.'
    size_t j = st.length();
    while(st[j-1] == ' ')
        j--;
    if ((st[j-1]not_eq'.')||(st[j-1] == '.' and st[j-2] == '\\' and st[j-3] not_eq '\\')){
        cout<<endl<<endl<<"Error - . missing in line "<<lin<<endl;
        return 0;
    }
    return 1;
}

//PRINT FUNCTION, which prints the expression or string or variable

int str_print(string st)
{                                               

    if (st.find("print") not_eq string::npos){

        if((int)st.find("print") not_eq 0){
            cout<<"\n\nError in line "<<lin<<endl;
            return 0;
        }

        string lit[20], temp;
		bool ign[20];
		for(int i = 0; i < 20; i++) ign[i] = false;
        int j, k = 0, pad = 0;
		
		if (st.find("printl") not_eq string::npos) pad = 1;

        for(int i = 5 + pad; i < (int)st.length(); i++){
			
            if(st[i] == '\"'){
                for(j = 1; st[i+j] != '\"' or st[i+j-1] == '\\'; j++);
                lit[k++] = st.substr(i+1,j-1);
                i += j;
            }
            else if(st[i] == '('){
                for(j = 1; st[i+j] != ')'; j++);
                temp = st.substr(i+1,j-1);
                p.infixToPostfix(temp);
				ss<<eval(&temp[0u]);
				clear_s();
				lit[k++] = ss.str();
				ss.str("");
				ss.clear();
                i += j; 
            }
            else if(st[i] != ' ' and st[i] != '.'){
                for(j = 1; st[i+j] != '\"' and st[i+j] != '(' and st[i+j] != '.' and st[i+j] != ' '; j++);
				if(st[i] == '~'){
					ign[k] = true;
					temp = st.substr(i+1,j-1);
				}
				else
					temp = st.substr(i,j);					//'~' is used to ignore escape sequences
				lit[k++] = prime->returnval(temp);
                i += j;
            }
        }

        for(int i = 0; i < k; i++){
			for(int j = 0; j < lit[i].length(); j++){
				if(ign[i] == true){
					cout<<lit[i][j];
				}
				else{
					if(lit[i][j] == '\\' and lit[i][j+1] == 'n'){
						cout<<'\n';
						j++;
					}
					else if(lit[i][j] == '\\' and (lit[i][j+1] == '\'' or lit[i][j+1] == '\\' or lit[i][j+1] == 'e' or lit[i][j+1] == 'f' or lit[i][j+1] == 'i' or lit[i][j+1] == 'v' or lit[i][j+1] == 'r' or lit[i][j+1] == 'd' or lit[i][j+1] == ':' or lit[i][j+1] == '\"' or lit[i][j+1] == '.')){
						cout<<lit[i][j+1];
						j++;
					}
					else
						cout<<lit[i][j];
				}
			}
		}

        if (st.find("printl") not_eq string::npos)
            cout<<endl;

      }

      return 1;
}

//VARIABLE DEFINITION, which creates variables with a name into the variable stack

int var_define(string st){

    if(st.find("\\var") not_eq string::npos)
        return 1;

    if(st.find("var") not_eq string::npos){

        if((int)st.find("var") not_eq 0){
            cout<<"\n\nError in line "<<lin<<endl;
            return 0;
        }

        bool isend = false;
        bool isvar = false;
        string vname;

        for(int i = 3; i < (int)st.length(); i++){

            if(st[i] == '.')
                isend = true;
            else if(st[i] not_eq ' '){
                isvar = true;
                int j;
                for(j = 1; st[j+i] not_eq ' ' and st[j+i] not_eq '.'; j++);
                vname = st.substr(i,j);
                prime->addvar(vname);
                i=i+j-1;
            }

        }

        if(isend == false){
            cout<<"\n\nNo termination of line in line "<<lin<<endl;
            return 0;
        }
        else if(isvar == false){
            cout<<"\n\nNo variable declared in line "<<lin<<endl;
            return 0;
        }

    }

    return 1;
}

//ASSIGNMENT OF VARIBLE VALUES, either constants or expressions

int assignment(string st){

    int flag = 1;

    for(int i = 0; i < st.length(); i++)
        if(st[i] == ':' && st[i-1] != '\\') flag = 0;
    
    if(flag == 1)
        return 1;

    if(st.find(":") not_eq string::npos){

        string vname;
        bool isvar = false;

        for(int i = 0; i < (int)st.length(); i++){
            if(st[i] not_eq ' '){
                isvar = true;
                int j;
                for(j = 1; st[j+i] not_eq ' ' and st[j+i] not_eq ':' and st[j+i] not_eq '.'; j++);
                vname = st.substr(i,j);
                break;
            }
        }

        if(isvar == false){
            cout<<"\n\nNo variable used in line "<<lin<<endl;
            return 0;
        }

        bool isvalue = false;
        bool asserror;
        string value;

        for(int i = 0; i < (int)st.length(); i++){
            if(st[i] == ':'){
                for(int k = i+1; k < (int)st.length(); k++){
                    if(st[k] not_eq ' '){
                        isvalue = true;
                        int j;
                        for(j = 1; st[j+k] not_eq '.' or st[j+k-1] == '\\'; j++);
						if(st[k] == '('){
							string temp = st.substr(k,j);
							p.infixToPostfix(temp);
							ss<<eval(&temp[0u]);
							clear_s();
							value = ss.str();
							ss.str("");
							ss.clear();
						}
						else
							value = st.substr(k,j);
                        int valtype = checktype(value);
                        asserror = prime->assignment(vname,value,valtype);
                        break;
                    }
                }
                break;
            }
        }

        if(isvalue == false){
            cout<<"\n\nNo value given in line "<<lin<<endl;
            return 0;
        }

        if(asserror == false){
            cout<<"\n\nNo assignment done in line "<<lin<<endl;
            return 0;
        }

    }

    return 1;
}

//FOR LOOP CONDITION, which initiates a for loop (also doubles as a while loop if step = 0)

int forstep(string st){

    if(st.find("\\for") not_eq string::npos || st.find("endfor") not_eq string::npos)        
        return 1;

    if(st.find("for") not_eq string::npos){

        if((int)st.find("for") not_eq 0){
            cout<<"\n\nFor keyword not in the beginning of the line "<<lin<<endl;
            return 0;
        }

        int loopc,counter,limit;
        bool isloop = false;

        for(int i = 3; i < (int)st.length(); i++){
            if(st[i] not_eq ' ' and st[i] not_eq '.'){

                currloop++;
                isloop = true;

                int j,k;
                for(j = 1; st[i+j] not_eq ' '; j++);          //Current test syntax: for 57 step 2 till 100

                if(isalpha(st[i])){
                    loopc = atoi(prime->returnval(st.substr(i,j)).c_str());
                    loops[currloop].conds = st.substr(i,j);
                }
                else
                    loopc = atoi(st.substr(i,j).c_str());                            //57

                k = i+j+1;
                string chkstep;
                for(j = 1; st[k+j] not_eq ' '; j++);
                chkstep = st.substr(k,j);                             //step
                if(chkstep not_eq "step"){
                    cout<<"\n\nStep keyword missing in line "<<lin<<endl;
                    return 0;
                }

                k += 5;

                for(j = 1; st[k+j] not_eq ' '; j++);                      //2

                if(isalpha(st[k])){
                    counter = atoi(prime->returnval(st.substr(k,j)).c_str());
                    loops[currloop].counts = st.substr(k,j);
                }
                else
                    counter = atoi(st.substr(k,j).c_str());

                k += j+1;
                string chktill;
                for(j = 1;st[k+j] not_eq ' '; j++);
                chktill = st.substr(k,j);                              //till
                if(chktill not_eq "till"){
                    cout<<"\n\nTill keyword missing in line "<<lin<<endl;
                    return 0;
                }

                k += 5;

                for(j = 1; st[k+j] not_eq ' ' && st[k+j] not_eq '.'; j++);                      //2

                if(isalpha(st[k])){
                    limit = atoi(prime->returnval(st.substr(k,j)).c_str());
                    loops[currloop].limits = st.substr(k,j);
                }
                else
                    limit = atoi(st.substr(k,j).c_str());

                loops[currloop].isinloop = true;
                loops[currloop].lcondition = loopc;
                loops[currloop].lcounter = counter;
                loops[currloop].llimit = limit;
                loops[currloop].startlin = lin+1;

                return 1;
            }
        }

        if(isloop == false){
            cout<<"\n\nFor loop not defined properly in line "<<lin<<endl;
            return 0;
        }

    }

    return 1;
}

//CHECKS END OF LOOP CONDITIONS, and iterates the loop variable with the step value / variable

int endloop(string st, fstream &f){

    if(st.find("endfor") not_eq string::npos && (int)st.find("endfor") == 0){

        if(loops[currloop].conds != "0") 
            loops[currloop].lcondition = atoi(prime->returnval(loops[currloop].conds).c_str());
        if(loops[currloop].counts != "0") 
            loops[currloop].lcounter = atoi(prime->returnval(loops[currloop].counts).c_str());
        if(loops[currloop].limits != "0") 
            loops[currloop].llimit = atoi(prime->returnval(loops[currloop].limits).c_str());

        prime->intadd(loops[currloop].conds,loops[currloop].lcounter);
        loops[currloop].lcondition += loops[currloop].lcounter;

        if(loopcheck()){
            f.seekg(0,ios::beg);
            for(int i = 1; i < loops[currloop].startlin; i++)
                getline(f,st);
            lin = loops[currloop].startlin-1;
        }
        else
            currloop--;

    }

    return 1;
}

//IGNORES ALL LINES IN AN IF BLOCK

void ifnottrue(string st, fstream &f){
    do{
                getline(f,st);
                if(st.find("\\if") not_eq string::npos);
                else if(st.find("if") not_eq string::npos && st.find("dif") == string::npos)
                    ifnottrue(st,f);
    }while(st.find("endif") == string::npos);
    return;
}

//IF CONDITION, which checks an expression and runs a block depending on its value

int ifcondition(string st, fstream &f){

    if(st.find("endif") not_eq string::npos || st.find("\\if") not_eq string::npos)
        return 1;

    if(st.find("if") not_eq string::npos){
        if((int)st.find("if") not_eq 0){
            cout<<"\n\nInvalid use of if keyword in line "<<lin<<endl;
            return 0;
        }

        bool isif = false;
        string condition;
        for(int i = 3; i < (int)st.length(); i++){
            if(st[i] not_eq ' ' && st[i] not_eq '.'){
                isif = true;
                int j;
                for(j = 1; st[i+j] not_eq ' ' && st[i+j] not_eq '.'; j++);				//IF format -> if 5. ... endif.
                if(st[i] == '('){
					string temp = st.substr(i,j);
					p.infixToPostfix(temp);
					ss<<eval(&temp[0u]);
					clear_s();
					condition = ss.str();
					ss.str("");
					ss.clear();
				}
				else
					condition = st.substr(i,j);
                break;
            }
        }

        if(!isif){
            cout<<"\n\nInvalid if syntax in line "<<lin<<endl;
            return 0;
        }

        if(condition[0] == '0'){
            ifnottrue(st,f);
        }
    }

    return 1;

}

//GOTO FUNCTION, which is extremely important in SPADE and allows large manouverability as well as recursion

int gotoescape(string st, fstream &f){

    if(st.find("goto") not_eq string::npos && (int)st.find("goto") == 0){

        bool isgoto = false;

        for(int i = 5; i < st.length(); i++){
            if(st[i] not_eq ' ' and st[i] not_eq '.'){
                isgoto = true;

                int j;
                int givenline;
                for(j = 1; st[i+j] not_eq ' ' and st[i+j] not_eq '.'; j++);
                givenline = atoi(st.substr(i,j).c_str());

                f.seekg(0,ios::beg);
                lin = 0;
                for(int i = 0; i < givenline-1; i++){
                    getline(f,st);
                    lin++;
                }

                return 1;
            }
        }

        if(isgoto == false){
            cout<<"\n\nInvalid syntax of goto statement in line "<<lin<<endl;
            return 0;
        }

    }

    return 1;
}

//DELETES TOPMOST OCCURENCE OF VARIABLE whose name has been mentioned. Allows even more powerful functions to work

int del_var(string st){
    
    if(st.find("\\del") != string::npos)
        return 1;

    if(st.find("del") != string::npos && (int)st.find("del") == 0){
        
        int j;
        bool flag = false;
        string vname;

        for(int i = 3; i < st.length(); i++){

            if(st[i] != ' ' && st[i] != '.'){
                flag = true;
                for(j = 1; st[i+j] != '.' && st[i+j] != '.'; j++);
                vname = st.substr(i,j);

                if(!(prime->delvar(vname))){
                    cout<<"\n\nVaraible to delete does not exist in line "<<lin<<endl;
                    return 0;
                }

            }

        }

    }
    
    return 1;
}


//MAIN STARTS HERE.


int main()
{
    fstream f;
    string s;
    string st;

    cout<<"\n\nEnter name of file to interpret: ";
    cin>>s;
    cout<<"\n";

    if(s.find(".spd") == string::npos)     // If does not end with .spd
		s = s + (".spd");

    f.open(s, ios::in);
    

    while(getline(f,st))
    {
        lin++;

        if(!checkdot(st)){
            break;
        }

        if(!str_print(st)){
            break;
        }
        else if(!var_define(st)){
            break;
        }
        else if(!assignment(st)){
            break;
        }
        else if(!forstep(st)){
            break;
        }
        else if(!endloop(st,f)){
            break;
        }
        else if(!ifcondition(st,f)){
            break;
        }
        else if(!gotoescape(st,f)){
            break;
        }
        else if(!del_var(st)){
            break;
        }
    }

    if(lin == 0)
        cout<<"\n\nNo code written."<<endl;

    if (!f.good() and !f.eof())           //don't use f.bad()
        cout<<"\n\nFile not found."<<endl;

    f.close();

    cout << "\n\nProgram terminated." << endl;
	
	return 0;
}
