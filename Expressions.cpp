#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<bits/stdc++.h>
#include "var.cpp"

using namespace std;

//Function to return precedence of operators
class Post// for converting a given infix expression t postfix expression
{
    int prec(char ch) {
     switch (ch) {
      case '^': return 6;
      case '/':
      case '%':
      case '*': return 5;
      case '+':
      case '-': return 4;
      case '>':
      case '<': return 3;
      case '=':
      case '~': return 2;
      case '&':
      case '|': return 1;
      case ':': return -1;
      default : return 0;
       }
  }

   public:

    void infixToPostfix(string &s)// & operator because string should be returned to main, it is not an array
    {   
        string ns;
        stack<char> st;
        st.push('N');
        int l = s.length();

        for(int i = 0; i < l; i++)
        {
            // If the scanned character is an operand, add it to output string.
            if(isalnum(s[i])){

                int j;
                for(j = 1; isalnum(s[i+j]); j++);
                ns += s.substr(i,j);
                i += j-1;
                ns += (string)" ";
            }

            // If the scanned character is an �(�, push it to the stack.
            else if(s[i] == '(')

            st.push('(');

            // If the scanned character is an �)�, pop and to output string from the stack
            // until an �(� is encountered.
            else if(s[i] == ')')
            {
                while(st.top() != 'N' && st.top() != '(')
                {
                    char c = st.top();
                    st.pop();
                    ns += c;
                    ns += ' ';
                }
                if(st.top() == '(')
                {
                    char c = st.top();
                    st.pop();
                }
            }

            //If an operator is scanned
            else{
                while(st.top() != 'N' && prec(s[i]) <= prec(st.top()))
                {
                    char c = st.top();
                    st.pop();
                    ns += c;
                    ns +=' ';
                }
                st.push(s[i]);
            }

        }
        //Pop all the remaining elements from the stack
        while(st.top() != 'N')
        {
            char c = st.top();
            st.pop();
            ns += c;
            ns += ' ';
        }
	
        s = ns;

    }
};// end of class post



double s[50];
int top=-1; /* Global declarations */
int flag=0;

double pop()
{                      /* Function for POP operation */
  return(s[top--]);
}

void push(double elem) //double
{ /* Function for PUSH operation */

  if(flag==1){
    double num; //double
    num=pop();
    s[++top]=10*num+elem;

  }
  else if(flag==0){
    s[++top]=elem;
    flag=1;
  }
}


double eval(char* pofx)// double
{                         /* Main Program */
  char ch;
  int i=0;
  double op1,op2;//double
  while( (ch=pofx[i++]) != 0)
  {
    if(isdigit(ch)) push(ch-'0'); /* Push the operand */    // double
	else if(isalpha(ch)){
		int j;
		for(j = 1; pofx[i+j-1] != ' '; j++);
		string st(pofx);
		st = st.substr(i-1,j);
		s[++top] = atof(prime->returnval(st).c_str());
		i += j;
	}
    else if(ch==' ')
      flag=0;
    else if (ch!=0)
    {
               /* Operator,pop two  operands */
      flag=0;
      op2=pop();
      op1=pop();
      switch(ch)
      {
        case '+':push(op1+op2);break;
        case '-':push(op1-op2);break;
        case '*':push(op1*op2);break;
        case '/':if(op2==0)
                 {
                    cout<<"error: division by 0"<<endl;
                    break;
                  }
                  push(op1/op2);break;

                case '%':push(fmod(op1,op2));break;
                case '^':push(pow(op1,op2));break;
                case '>':push(op1>op2);break;
                case '<':push(op1<op2);break;
                case '=':push(op1==op2);break;
                case '~':push(op1!=op2);break;
                case '&':push(op1&&op2);break;
                case '|':push(op1||op2);break;


        default:
                 cout<<"Input invalid ... give proper input\n";
                 return 0;
      }
    }
  }

  return s[top];
}

void clear_s(){
	top = -1;
}


/*Driver program to test above functions
int main()
{
    string infix;
    Post p;
    cout<<"\nEnter an expression to convert to postfix:\n";
    cin>>infix;
    cout<<endl;
    p.infixToPostfix(infix);
   cout<<"\nPostfix Expression :: "<<infix;
   cout<<endl;
   //int result=eval("10 12 + 3 4 - +");
   double result=eval(&infix[0u]);
   cout<<"result="<<result;
}

*/