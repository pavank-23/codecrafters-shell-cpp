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
      try
      {
        auto perms = std::filesystem::status(entry).permissions();

        bool executable =
            (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
            (perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
            (perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
        if (executable)
          execs[entry.path().filename()] = entry.path();
      }
      catch (std::filesystem::__cxx11::filesystem_error)
      {
        // std::cerr << "No Permissions to access subentries of: " << entry.path() << std::endl;
        continue;
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
    else if (input.substr(0, 3) == "pwd")
    {
      std::cout << std::filesystem::current_path() << std::endl;
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
      std::string cmd;
      for (char c : input)
      {
        if (c == ' ')
        {
          break;
        }
        else
        {
          cmd += c;
        }
      }
      if (execs.find(cmd) != execs.end())
      {
        system(input.c_str());
      }
      else
      {
        std::cout << input << ": command not found" << std::endl;
      }
    }
  }
}