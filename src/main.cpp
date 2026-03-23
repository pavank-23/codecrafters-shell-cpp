#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::set<std::string> builtin_commands = {"echo", "exit", "type"};
  std::vector<std::string> paths;
  const char *env_p = std::getenv("PATH");

  std::string curr = "";

  for (const char *p = env_p; *p != '\0'; ++p)
  {
    char c = *p;
    if (c == ':')
    {
      paths.push_back(curr);
      curr = "";
    }
    else
    {
      curr += c;
    }
  }

  std::unordered_map<std::string, std::string> execs;

  for (std::string p : paths)
  {
    for (const auto &entry : std::filesystem::directory_iterator(p))
    {
      if (entry.is_directory() == true)
      {
        try
        {
          for (const auto &subentry : std::filesystem::directory_iterator(entry.path()))
          {
            execs[subentry.path().filename()] = subentry.path();
          }
        }
        catch (std::filesystem::__cxx11::filesystem_error)
        {
          // std::cerr << "No Permissions to access subentries of: " << entry.path() << std::endl;
          continue;
        }
      }
    }
  }

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
        if (execs.find(arg) != execs.end())
        {
          std::cout << arg << " is " << execs[arg] << std::endl;
        }
        else
          std::cout << arg << ": not found" << std::endl;
      }
    }
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}