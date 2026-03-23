#include <iostream>
#include <string>
#include <set>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::set<std::string> builtin_commands = {"echo", "exit", "type"};

  while (true)
  {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    if (input == "exit")
    {
      break;
    }
    else if (input.substr(0, 4) == "echo")
    {
      std::cout << input.substr(5) << std::endl;
    }
    else if (input.substr(0, 4) == "type")
    {
      std::string arg = input.substr(5);
      if (builtin_commands.find(arg) != builtin_commands.end())
      {
        std::cout << arg << " is a shell builtin" << std::endl;
      }
      else
      {
        std::cout << arg << ": not found" << std::endl;
      }
    }
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}