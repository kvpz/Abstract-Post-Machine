/*  
   main.cpp
   01/21/15
   Chris Lacher

   Driver program for PostMachine object
*/

#include <iostream>
#include <post.h>

int main(int argc, char**)
{
  bool batch = 0;
  if (argc > 1) batch = 1;
  PostMachine m;
  bool loaded = 0;
  char response ('x');
  do
  {
    do loaded = m.Load(batch);
    while (!loaded);
    m.Run(batch);
    std::cout << "\n** Load another program? ";
    std::cin >> response;
    if (batch) std::cout << response << '\n';
  }
  while (response == 'y' || response == 'Y');
  return EXIT_SUCCESS;
}
