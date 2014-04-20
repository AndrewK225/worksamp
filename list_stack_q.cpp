/* 
   Andrew Koe 3/18/14
   Program will read in commands from a file
   and create,push,and pop stacks and queues.  NO SYNTAX CHECKING
   (I had some help from Jason Kim regarding syntax for classes. 
   ex: line 64)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>   //needed for exit.  C++11 syntax?
#include <list>

using namespace std;


void parse_file(ifstream &infile, ofstream &outfile);

int main()
{
  ifstream commandfile;
  ofstream log;
  string infile;
  string outfile;
  string line;
  /*Take in relevant file names from user*/
  cout << "Enter name of input file: ";
  cin >> infile;                           //Assuming no spaces in filenames
  cout << "Enter name of output file: ";
  cin >> outfile;
 
  /*open files and parse commands*/
  commandfile.open(infile.c_str(),ios::in);
  if(!(commandfile.is_open()))
    {
      cerr << "Could not open file " << infile << endl;
      exit(EXIT_FAILURE);
    }
  
  log.open(outfile.c_str(),ios::out | ios::trunc);
  if(!(log.is_open()))
    {
      cerr << "Could not open file " << outfile << endl;
      exit(EXIT_FAILURE);
    }

  parse_file(commandfile,log);

  return 0;
}

//abstract class that stack and queue inherit from.  
//Based on suggestions from problem statement.
template <class T> 
class SimpleList
{
private:
  class Node
  {
  public:
    T value;
    Node *next;
    Node(const T value, Node *next = NULL)
      :value(value),next(next) {}
  };
  Node *head;
  Node *tail;
  string l_name;  //name of list

protected:
  void insert_start(const T &value);
  void insert_end(const T &value);
  T remove_node(void);

public:
  SimpleList(const string name)
  {
    l_name = name;
    head = NULL;
    tail = NULL;
  }

  bool check_empty();
  string getName()
  {
    return this -> l_name;
  }

  virtual void push(const T &value) = 0;
  virtual T pop() = 0;
};

template <class T>
bool SimpleList<T>::check_empty()
{
  if (this -> head == NULL) 
    {
      return true;
    }
  else
    return false;
}

template <class T>
void SimpleList<T>::insert_start(const T &value)
{
  Node *nNode = new Node(value);  //is it bad to use new in name?
  Node *oldNode = this -> head;

  if(this -> head == NULL) 
    {
      this -> head = nNode;
      this -> tail = nNode;
    }
  else 
    {
      this -> head = nNode;
      nNode -> next = oldNode;
    }
}

template <class T>
void SimpleList<T>::insert_end(const T &value)
{
  Node *nNode = new Node(value);

  if(this -> head == NULL)
    {
      this -> tail = nNode;
      this -> head = nNode;
    }
  else
    {
      tail -> next = nNode;
      this -> tail = nNode;
    }
}

template <class T>
T SimpleList<T>::remove_node(void)
{
  Node *nStart = this -> head;
  Node *nnextNode = nStart -> next;
  if (this -> head == NULL)
    {
      this -> tail = NULL;
    }
  else 
    {
      T popped = nStart -> value;
      this -> head = nnextNode;
      delete nStart;
      return popped;
    }
  return 0;
}

template <class T>
SimpleList<T> *find_name(list<SimpleList<T> *>List, const string name) 
{
  typename list<SimpleList<T> *>::iterator i_list;  //use to iterate through general list
  for(i_list = List.begin(); i_list != List.end(); i_list++)
    {
      if((*i_list) -> getName() == name) 
	{
	  return (*i_list);
	}
    }
  return NULL;
}

template <class T>
class Stack : public SimpleList<T>
{
public:
  Stack(string name):SimpleList<T>(name){};
  void push(const T &value);
  T pop();

};

template <class T>
void Stack<T>::push(const T &value)
{
  this -> insert_start(value);
}

template <class T>
T Stack<T>::pop()
{
  T popped;
  popped = this -> remove_node();
  return popped;
}

template <class T>
class Queue : public SimpleList<T>
{
public:
  Queue(string name):SimpleList<T>(name){};
  void push(const T &value);
  T pop();
};

template <class T>
void Queue<T>::push(const T &value)
{
  this -> insert_end(value);
}

template <class T>
T Queue<T>::pop()
{
  T popped;
  popped = this -> remove_node();
  return popped;
}

/* very repetitive.  There must be a better way*/
void parse_file(ifstream &infile, ofstream &outfile) 
{
  string command, list_name, type, str_value;
  int int_value;
  double dbl_value;

  /* c++ list used for lists of lists */
  list <SimpleList <int> *> gen_int;
  list <SimpleList <double> *> gen_dbl;
  list <SimpleList <string> *> gen_str;
  SimpleList<int> *ptr_iL;
  SimpleList<double> *ptr_dL;
  SimpleList<string> *ptr_sL;
  
  while(!infile.eof()) 
    {
      command = "\0";
      list_name = "\0";
      infile >> command;
      infile >> list_name;
      
      /*parse  based on command*/
  
      if(command == "create") 
	{
	  infile >> type;
	  outfile << "PROCESSING COMMAND: " << command << " " << list_name <<" "<< type << endl;
	  if(list_name.find("i", 0) == 0) //datatype to be stored
	    {
	      if(find_name(gen_int, list_name) != NULL)    //look in gen list to make sure it doesn't exist already 
		{
		  outfile << "ERROR: This name already exists!" << endl;
		}
	      else //all good, create the stack or queue and add to gen list
		{
		  if(type == "stack") 
		    {
		      ptr_iL = new Stack<int>(list_name);
		      gen_int.push_front(ptr_iL);
		    }
		  else 
		    {
		      ptr_iL = new Queue<int>(list_name);
		      gen_int.push_front(ptr_iL);
		    }
		}
	    }
	  else if(list_name.find("d", 0) == 0) 
	    {
	      if(find_name(gen_dbl, list_name) != NULL) 
		{
		  outfile << "ERROR: This name already exists!" << endl;
		}
	      else 
		{
		  if(type == "stack") 
		    {
		      ptr_dL = new Stack<double>(list_name);
		      gen_dbl.push_front(ptr_dL);
		    }
		  else 
		    {
		      ptr_dL =  new Queue<double>(list_name);
		      gen_dbl.push_front(ptr_dL);
		    }
		}
	    }
	  else if(list_name.find("s",0) == 0) 
	    {
	      if(find_name(gen_str, list_name) != NULL) 
		{
		  outfile << "ERROR: This name already exists!" << endl;
		}
	      else 
		{
		  if (type == "stack") 
		    {
		      ptr_sL = new Stack<string>(list_name);
		      gen_str.push_front(ptr_sL);
		    }
		  else 
		    {
		      ptr_sL =  new Queue<string>(list_name);
		      gen_str.push_front(ptr_sL);
		    }
		}
	    }
	}
    else if(command == "push") 
      {
	if(list_name.find("i", 0) == 0) 
	  {
	    infile >> int_value;
	    outfile << "PROCESSING COMMAND: " << command <<" " << list_name <<" " << int_value << endl;
	    ptr_iL = find_name(gen_int, list_name);  //find the list
	    if(ptr_iL != NULL) 
	      {
		ptr_iL -> push(int_value);
	      }
	    else 
	      {
		outfile << "ERROR: This name does not exist!" << endl;
	      }
	  }
      else if(list_name.find("d", 0) == 0) 
	{
	  infile >> dbl_value;
	  outfile << "PROCESSING COMMAND: " << command <<" " << list_name <<" "<< dbl_value << endl;
	  ptr_dL = find_name(gen_dbl, list_name);
	  if(ptr_dL != NULL) 
	  {
	    ptr_dL -> push(dbl_value);
	  }
	  else 
	    {
	      outfile << "ERROR: This name does not exist!" << endl;
	    }
	}
      else if(list_name.find("s", 0) == 0) 
	{
	  infile >> str_value;
	  outfile << "PROCESSING COMMAND: " << command << " " << list_name << " " << str_value << endl;
	  ptr_sL = find_name(gen_str, list_name);
	  if(ptr_sL != NULL) 
	    {
	      ptr_sL -> push(str_value);
	    }
	  else 
	    {
	      outfile << "ERROR: This name does not exist!" << endl;
	    }
	}
      }
    else 
      {
	if(list_name.find("i", 0) == 0) 
	  {
	    outfile << "PROCESSING COMMAND: " << command << " " << list_name << endl;
	    ptr_iL = find_name(gen_int, list_name);
	    if(ptr_iL == NULL) 
	      {
		outfile << "ERROR: This name does not exist!" << endl;
	      }
	    else 
	      {
		if(ptr_iL -> check_empty()==true) 
		  {
		    outfile << "ERROR: This list is empty!" << endl;
		  }
		else 
		  {
		    int_value = ptr_iL -> pop();
		    outfile << "Value popped: " << int_value<<endl;
		  }
	      }
	  }
	else if(list_name.find("d", 0) == 0) 
	  {
	    outfile << "PROCESSING COMMAND: " << command << " " << list_name << endl;
	    ptr_dL = find_name(gen_dbl, list_name);
	    if(ptr_dL == NULL) 
	      {
		outfile << "ERROR: This name does not exist!" << endl;
	      }
	    else 
	      {
		if(ptr_dL -> check_empty()==true) 
		  {
		    outfile << "ERROR: This list is empty!" <<endl;
		  }
		else 
		  {
		    dbl_value = ptr_dL -> pop();
		    outfile << "Value popped: " << dbl_value << endl;
		  }
	      }
	  }
	else if(list_name.find("s", 0) == 0) 
	  {
	    outfile << "PROCESSING COMMAND: " << command <<" " << list_name << endl;
	    ptr_sL = find_name(gen_str, list_name);
	    if(ptr_sL == NULL) 
	      {
		outfile << "ERROR: This name does not exist!" << endl;
	      }
	    else 
	      {
		if(ptr_sL -> check_empty()==true) 
		  {
		    outfile << "ERROR: This list is empty!" << endl;
		  }
		else 
		  {
		    str_value = ptr_sL -> pop();
		    outfile << "Value popped: " << str_value << endl;
		  }
	      }
	  }
      }
    }
  infile.close();
  outfile.close();
}
 
/*Note to self: stop using this bracing style.  It wastes space*/
