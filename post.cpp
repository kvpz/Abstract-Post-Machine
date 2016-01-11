/*
  post.cpp
  Kevin Perez
  5/28/15

  A Post machine (PM) is an abstract computer. It's architecture
  consists of:
 
  1. A programming memory, which stores a program (as a list of instructions)
  2. A processing tape
  3. An internal state
  4. A processor
  
  A PM instruction consists of a character string of the form:
  
  <current_state><ch><new_state><word>
  
  where the first 3 are single characters and <word> is a string 
  of 0 or more characters. The internal state is a value of the alphabet
  and a single character. Execution begins with the internal state initially
  set to S (for Start).

A PM instruction file is appended with .pp.
*/

#include "post.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void ClearBuffer(std::istream& is)
{
  char ch;
    do
    {
      is.get(ch);
    }
    while ((!is.eof()) && (ch != '\n'));
}

PostMachine::PostMachine() 
{
  std::cout << "** Post machine started.\n" << std::endl;  
}

PostMachine::~PostMachine()
{
  std::cout << "** Post machine stopped." << std::endl;
}

/**********************************************************
  Load () makes a query for a filename, opens the file, and 
reads the contents into its program memory (a List object),
one instruction(a line) per element.

  Run () executes according to the processing algorithm given 
and reports results. 
**********************************************************/

bool PostMachine::Load(bool batch)
{
  // I/O variables
  std::ifstream in1;
  fsu::String filename;

  do
  {
  // open program file
  std::cout << "  Name of instruction file: ";
  std::cin >> filename;
  in1.open(filename.Cstr());
  if(in1.fail())
    std::cout << "** Cannot open file '" << filename << "'. Check " 
              <<  "name and try again." << std::endl;
  } while(in1.fail());
  
  if (batch) std::cout << filename << '\n';
  
  //Insert Program's content into List object
  //avoiding all comments
  program_.Clear();
  while(!in1.eof())
  { 
    fsu::String s;
    s.GetLine(in1); // reads entire line (instruction)
    // store the index of the first occurences of '*' in [0,s.size)
    size_t pos = s.Position('*',0); // 0 if '*' is first character in line
    if(pos < s.Size()-1 && pos > 0 )
    {
      fsu::String temp(pos,' '); // temp([size], [characters to fill with])
      for(size_t j = 0; j < pos; j++)
        temp[j] = s[j];  
      program_.PushBack(temp);
    }
    else if(pos == 0 && s.Position('\n',0) == 1)
      program_.PushBack(s);
    else if(pos == s.Size())
      program_.PushBack(s);     
  }
  in1.close();
  return true;
}

void PostMachine::Run(bool batch)
{
  char ch; 

  // simulation variables
  bool finished, halt, crash, maxits_reached, match;  
  size_t its = 0;
  
  // now loop until '*' is received as (first character of) input
  while(1)
  {
    // empty the tape
    tape_.Clear();
    // read input and place on tape, followed by '#'
    // if first char is '*', purge input buffer and return - Run is over
    std::cout << "   Input string (* to end): ";
    if(its == 0)
      std::cin.ignore();
    ch = std::cin.get();
    if (batch) std::cout.put(ch);
    if (ch == '*') 
    {
      ClearBuffer(std::cin);
      if (batch) std::cout.put('\n');
      return;
    }
    while (ch != '\n')
    {
      tape_.Push(ch);
      ch = std::cin.get();
      if (batch) std::cout.put(ch);
    }
    if (batch) std::cout.put('\n');
    tape_.Push('#');

    // run Post machine
    state_ = 'S'; // state is some value of the alphabet S = 'Start'
    finished = halt = crash = maxits_reached = 0;
    its = 0;
    do
    {
      match = 0;     
      for (typename fsu::List<fsu::String>::ConstIterator i = program_.Begin(); i != program_.End(); ++i)
      {
        fsu::String s = *i;        
        if(s.Element(0) == state_)
        {
          if(!tape_.Empty())
          {
            char tapeNext = tape_.Front();
            if(s.Element(1) == tapeNext)
            {
              match = 1;
              tape_.Pop();
              state_ = s.Element(2);
              for(size_t j = 3; j < s.Size(); ++j)
                tape_.Push(s.Element(j));
            }
          }
        }        
      }
      
      ++its;
      halt           = (state_ == 'H');
      crash          = !match;
      maxits_reached = (its == post::maxIterations);
      finished       = halt || crash || maxits_reached;
    }
    while (!finished);
    
    // report results
    if (halt)
    {
      std::cout << "\tString accepted.\n" << std::endl;
      //std::cout << std::endl << std::endl;
      //std::cout << "String at halt: ";
      //tape_.Display(std::cout);
      //std::cout << "\n\n" << std::endl;
    }
    else if (crash)
    {
      std::cout << "String rejected." << std::endl;
      std::cout << "Last state: " << state_ << std::endl;
      std::cout << "Tape contents at rejection: ";
      tape_.Display(std::cout);
      std::cout << std::endl << std::endl;
    }
    else if (maxits_reached)
    {
      std::cout << "Machine stopped after " << post::maxIterations << " iterations." << std::endl;
      std::cout << "Tape contents when stopped: ";
      tape_.Display(std::cout);
      std::cout << "\nLast state: " << state_ << std::endl;
      std::cout << std::endl;
    }
    else // presumed unreachable branch
    {
      std::cerr << "** PostMachine error: bad processing termination\n";
    }
  } // end while(1)
} // end Run()



