#include "lliststr.h"
#include "stackstr.h"
#include<sstream>
#include <iostream>
#include <fstream>
#include<stack>

using namespace std;

void stringparser(char * input, char *output){

	ifstream file(input);
	ofstream out(output);

	if(file.eof())
		return;

	string x;
	while(getline(file, x))
	{
		StackStr *stack = new StackStr();
		int counter=0;
		bool malform = 0 ;
		if(x.length()==0)
 			out << "Malformed" << endl;
		for (unsigned int i = 0 ; i < x.length() ; i++)
		{
			stringstream ss;
			while(x[i] == ' ')
				++i;
			if(x[i] == '(')
				counter++;
			if(x[i] == ')')
				counter--;
			if(x[i] =='!')
			{
				out << "Malformed" << endl;
				string y;
				getline(file, y);
				if(file.eof() && y.length()==0)
				{
					delete stack;
					return;
				}
				if(y.length()==0)
					out << "Malformed" << endl;
				x=y;
				ss.str("");
				i=0;
				while(!stack->empty())
					stack->pop();

			}
			if(isupper(x[i])|| isdigit(x[i]))
			{
				out << "Malformed" << endl;
				string y;
				getline(file, y);
				if(file.eof() && y.length()==0)
				{
					delete stack;
					return;
				}
				if(y.length()==0)
					out << "Malformed" << endl;
				x=y;
				ss.str("");
				i=0;
				while(!stack->empty())
					stack->pop();

			}
			else if (islower(x[i]))
			{
				bool done = false;
				while(!done && i < x.length())
				{
					if((!islower(x[i]) && x[i] !=' '))
					{
						out << "Malformed" << endl;
						string y;
						getline(file, y);
						if(file.eof() && y.length()==0)
						{
							delete stack;
							return;
						}
						if(y.length()==0)
							out << "Malformed" << endl;
						x=y;
						ss.str("");
						i=0;
						while(!stack->empty())
							stack->pop();
					}
					ss << x[i];
					i++;
					if(x[i] == '+' || x[i] == '-' ||x[i] == '>' ||x[i] == '<' ||x[i] == '(' ||x[i] == ')')
					{
						i--;
						done =1;
					}
				}
				string word;
				ss >> word;
				stack->push(word);
			}
			else if (x[i] == ')')
			{
				int operation=0;
				int minus=0;
				bool parse =1;
				if(stack->size()==1 && stack->top()=="(")
				{
					out << "Malformed" << endl;
					malform=1;
				}
				while(stack->top() != "(" && parse && !stack->empty())
				{
					string second = stack->top();
					stack->pop();
					char check = stack->top()[0];
					if(islower(check))
					{
						out << "Malformed" << endl;
						malform=1;
						while(!stack->empty())
							stack->pop();

					}
					else if(second=="+" || second=="-")
					{
						out << "Malformed" << endl;
						malform=1;
						while(!stack->empty())
							stack->pop();

					}
					else if(stack->top()== "+")
					{
						operation++;
						stack->pop();
						string first = stack->top();
						if(first=="+" || first=="-" || first =="(")
						{
							out << "Malformed" << endl;
							malform=1;
							while(!stack->empty())
								stack->pop();
						}
						else
						{
							stack->pop();
							if(stack->top() == "-")
							{
								out << "Malformed" << endl;
								malform=1;
								while(!stack->empty())
									stack->pop();
							}
							else
							{
								while (stack->top() == "<" || stack->top()==">")
								{
 									if(stack->top()=="<")
									{
										if(first.length() >1)
										{
									 		stack->pop();
											first= first.erase(first.length()-1, 1);
										}
										else
											stack->pop();
									}
									else if(stack->top()== ">")
									{
										if(first.length()>1)
										{
											stack->pop();
											first= first.erase(0,1);
										}
										else
											stack->pop();
									}
								}
								if(first.length()==0)
									stack->push(first);
								else if(second.length()==0)
									stack->push(second);
								else
								{
									first = first + second;
									stack->push(first);
								}
							}
						}
					}
					else if(stack->top()== "-")
					{
						operation++;
						minus++;
						stack->pop();
						string first = stack->top();
						if(first=="+" || first=="-"|| first =="(")
						{
							out << "Malformed" << endl;
							malform=1;							
							while(!stack->empty())
								stack->pop();
						}
						else
						{
							stack->pop();
							if(stack->top() =="+")
							{
								out << "Malformed" << endl;
								while(!stack->empty())
									stack->pop();
							}
							else
							{
								while (stack->top() == "<" || stack->top()==">")
								{
									if(stack->top()=="<")
									{
										if(first.length() >1)
										{
											stack->pop();
											first= first.erase(first.length()-1, 1);
										}
										else
											stack->pop();
									}
									else if(stack->top()== ">")
									{
										if(first.length()>1)
										{
											stack->pop();
											first= first.erase(0,1);
										}
										else
											stack->pop();
									}
								}
								int pos = first.find(second);
								if(pos >=0)
									first.erase(pos, second.length());
								stack->push(first);
							}
						} 
					}

					else if(stack->top()=="<")
					{
						if(second.length() >1)
						{
							stack->pop();
							string work = second.erase(second.length()-1, 1);
							stack->push(work);
						}
						else
						{
							stack->pop();
							stack->push(second);
						}

					}
					else if(stack->top()== ">")

					{
						if(second.length()>1)
						{
							stack->pop();
							string work = second.erase(0,1);
							stack->push(work);
						}
						else
						{
							stack->pop();
							stack->push(second);
						}
					}
					else if(stack->size() <= 1 && ! malform)
					{
						if(second.length()==0)
						{
							out << "Malformed"<< endl;
							while(!stack->empty())
								stack->pop();
							malform=1;
						}
						else if(minus>1)
						{
							out << "Malformed"<< endl;
							while(!stack->empty())
								stack->pop();
							malform=1;

						}
						else if(operation ==0)
						{
							out << "Malformed"<< endl;
							while(!stack->empty())
								stack->pop();
							malform=1;
						}
						else if(counter != 0)
						{
							out << "Malformed" << endl;
							malform=1;
							while(!stack->empty())
								stack->pop();
						}
						else if(i != x.length()-1)
						{
							stack ->push(second);
							i++;
							if(x[i] ==')')
								counter--;
						}
						else
							out << second << endl;
					}
					else if (stack->top() == "(")
					{
						if(operation==0)
						{
							out << "Malformed"<< endl;
							while(!stack->empty())
								stack->pop();
							malform=1;
						}
						stack->pop();
						stack->push(second);
						parse=0;
					}
					
				}
			}
			else
			{
				ss << x[i];
				string in;
				ss >> in;
				stack->push(in);
			}
		}
		while (stack->size()>1 && !malform)
		{
 			string second = stack->top();
			stack->pop();
			char check = second[0];
			if(!islower(check))
			{
				out<< "Malformed"<< endl;
				malform=1;
			}
			else if(stack->size()==1 && second=="(")
				{
					out << "Malformed" << endl;
					malform=1;
				}
			else if(stack->top()=="+" || stack->top()=="-")
			{
				out << "Malformed" << endl;
				malform=1;					
				while(!stack->empty())
					stack->pop();
			}
			else if(stack->top()=="<")
			{
				if(second.length() >1)
				{
					stack->pop();
					string work = second.erase(second.length()-1, 1);
					stack->push(work);
				}
				else
				{
					stack->pop();
					stack->push(second);
				}

			}
			else if(stack->top()== ">")
			{
				if(second.length()>1)
				{
					stack->pop();
					string work = second.erase(0,1);
					stack->push(work);
				}
				else
				{
					stack->pop();
					stack->push(second);
				}
			}
			else if(stack->size()==1)
			{
				char check = stack->top()[0];
				if(islower(check))
				{
					out << "Malformed" << endl;
					malform=1;
					while(!stack->empty())
						stack->pop();
				}
				else if(counter != 0)
				{
					out << "Malformed" << endl;
					malform=1;
					while(!stack->empty())
						stack->pop();
				}
				else 
					out << second<< endl;
			}
		}
		char check = stack->top()[0];
		if(stack->size()==1 && islower(check) && !malform)
			out << stack->top() << endl;
		else if (stack->top().length()==0 && stack->size()!=0)
			out << "Malformed" << endl;
		delete stack;
	}
	file.close();
	out.close();
	return;
}

int main (int argc, char* argv[]){
	stringparser(argv[1], argv[2]);
} 
