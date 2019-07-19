#include <iostream>
#include <ctype.h>
#include <cmath>
#include "Struct.h"

using namespace std;

NODE* checkNode;

int priority(char op);
void executeOperator(char op);

Queue* queue1 = new Queue ();
Queue* output = new Queue (); 
Stack* opStack = new Stack ();
iStack* hold = new iStack ();

int main(){

	char input;

	cout << "insert data: (for operators use ( ), +, *, -, /, or ** (power))" << endl; 
	while(true){
		
		cin >> input;	

		if(input == '^'){
				break;
			}
		else{
			if (queue1->isFull() == 1){ cout << "The stack is full. Enter the '^' to stop." <<endl;}
			else { queue1->Push(input); }
			}
	}

 	queue1->printQueue();
 	opStack->Push('%');   //set an initial char in the opStack to prevent Seg fault (know where to stop)


 	checkNode = queue1->goToFront();


 	while(checkNode != NULL){
 		if(checkNode->data == '*' && checkNode->next->data == '*'){ //parse thrugh input until ** is found
 			checkNode->data = '$'; //replace ** with $ for compatibility later 
 			queue1->deleteNode(checkNode->next);
 		}

 		checkNode = checkNode->next;
 	}

 	checkNode = queue1->goToFront();
	while(checkNode != NULL){
		NODE* tmp = checkNode;
		char checkVal = checkNode->data;
	
		if(isdigit(checkVal)){ 		
			output->Push(checkVal);  //if operand, push into 'output' Queue

		}
		else{
			if(checkVal == '(')opStack->Push(checkVal); //always push '(' into Stack to keep track of Operations
			else if(checkVal == ')'){
					while(opStack->Top() != '('){
						output->Push(opStack->Pop()); //pop out all ops between parenthasis
					}
					opStack->Pop(); //We dont want to store the '(' anymore, so we pop it out
				}
			else if(priority(checkVal)>priority(opStack->Top())||opStack->isFull() == -1){
				opStack->Push(checkVal);
			}
			else{
				while(priority(checkVal)<=priority(opStack->Top())){
					output->Push(opStack->Pop());
				}
				opStack->Push(checkVal);
			}
		}
		checkNode = checkNode->next;
		queue1->deleteNode(tmp);
		output->printQueue();
	}
	while(opStack->Top() != '%'){
		if(opStack->Top() == '('){
			opStack->Pop();
		}
		else{
			output->Push(opStack->Pop());
		}
	}

    output->printQueue();
	
    hold->Push(0);
    checkNode = output->goToFront();
   	
	while(checkNode != NULL){
		char out = checkNode->data;
		NODE* tmp = checkNode;

		if(isdigit(out)){
			hold->Push(out - '0');
		} 
		else{executeOperator(out);}

		checkNode = checkNode->next;
		output->deleteNode(tmp);
	}

	int final = hold->Pop();
	cout<<"The Answer is: "<< final <<endl;
	return 0;
}

int priority(char op){
	int num = 0; 
		switch(op){
			case '-':
				num = 1;
				break;
			case '+':
				num = 2;
				break;
			case '/':
				num = 3;
				break;
			case '*':
				num = 4;
				break;
			case '$':
				num = 5;
				break;
		}
	return num;
}

void executeOperator(char op){
	int a = hold->Pop();
	int b = hold->Pop();
	int c = 0;

	switch(op){
		case '+':
			c = a + b;
			break;
		case '*':
			c = a * b;
			break;
		case '-':
			c = b - a;
			break;
		case '/':
			if(a == 0){
				cout<< "Error: Division by 0"<<endl;
				exit(1);
			}
			else c = b/a;
			break;
		case '$':		//add the BONUS power case
			c= pow(b,a);
			break;
	}
	hold->Push((int)c);
}