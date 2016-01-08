#ifndef _POST_H_
#define _POST_H_
#include <list.h>
#include <xstring.h>
#include <queue.h>

namespace post
{
  const size_t maxIterations = 10000;
}

class PostMachine
{
 public:
  PostMachine (); 
  ~PostMachine (); 
  bool Load  (bool batch = 0);
  void Run  (bool batch = 0);
 private:
  fsu::List  < fsu::String >                program_; //stores programs
  fsu::Queue < char , fsu::Deque < char > > tape_;    //serves as the tape
  char                                      state_;   //store internal state
};

#endif
