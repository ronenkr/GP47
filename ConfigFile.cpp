#include "ConfigFile.h"
#include <iostream>
#include <fstream> 
#include <windows.h>

void ConfigFile::removeComment(std::string &line) const
{
  if (line.find('#') != line.npos)
  {
    line.erase(line.find('#'));
  }
}

bool ConfigFile::onlyWhitespace(const std::string &line) const
{
  return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigFile::validLine(const std::string &line) const
{
  std::string temp = line;
  temp.erase(0, temp.find_first_not_of("\t "));
  if (temp[0] == '=')
  {
    return false;
  }

  for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
  {
    if (temp[i] != ' ')
    {
      return true;
    }
  }

  return false;
}

void ConfigFile::extractKey(std::string &key, size_t const &sepPos, const std::string &line) const
{
  key = line.substr(0, sepPos);
  if (key.find('\t') != line.npos || key.find(' ') != line.npos)
  {
    key.erase(key.find_first_of("\t "));
  }
}
void ConfigFile::extractValue(std::string &value, size_t const &sepPos, const std::string &line) const
{
  value = line.substr(sepPos + 1);
  value.erase(0, value.find_first_not_of("\t "));
  value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extractContents(const std::string &line)
{
  std::string temp = line;
  temp.erase(0, temp.find_first_not_of("\t "));
  size_t sepPos = temp.find('=');

  std::string key, value;
  extractKey(key, sepPos, temp);
  extractValue(value, sepPos, temp);

  if (!keyExists(key))
  {
    contents.insert(std::pair<std::string, std::string>(key, value));
  }
  else
  {
    exitWithError("CFG: Can only have unique key names!\n");
  }
}

void ConfigFile::parseLine(const std::string &line, size_t const lineNo)
{
  if (line.find('=') == line.npos)
  {
    exitWithError("CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo) + "\n");
  }

  if (!validLine(line))
  {
    exitWithError("CFG: Bad format for line: " + Convert::T_to_string(lineNo) + "\n");
  }

  extractContents(line);
}

void ConfigFile::ExtractKeys()
{
  std::ifstream file;
  file.open(fName.c_str());

  if (!file)
  {
    printf("%s not found! Building a fresh one... ", fName.c_str());

    std::ofstream outfile("config.ini");

    // Begin config dump to file
    outfile << "#   GP47 DEFAULT CONFIGURATION rev0.1 - Auto generated" << std::endl;
    outfile << "#   Key combo that the program is listening for, Default is [SELECT] + [START] + [RB]" << std::endl;
    outfile << "#   Comment out or in any keys not wanted in the combination " << std::endl;
    outfile << "#KEY_LB = 0x100 #left button" << std::endl;
    outfile << "KEY_RB = 0x200 #right button" << std::endl;
    outfile << "KEY_SELECT = 0x20 #select button" << std::endl;
    outfile << "KEY_START = 0x10 #start button" << std::endl;
    outfile << "#KEY_UP = 0x1 #up button" << std::endl;
    outfile << "#KEY_DOWN = 0x2 #down button" << std::endl;
    outfile << "#KEY_LEFT = 0x4 #left button" << std::endl;
    outfile << "#KEY_RIGHT = 0x8 #right button" << std::endl;
    outfile << "#KEY_A = 0x2000 #A button" << std::endl;
    outfile << "#KEY_B = 0x1000 #B button" << std::endl;
    outfile << "#KEY_X = 0x8000 #X button" << std::endl;
    outfile << "#KEY_Y = 0x4000 #Y button" << std::endl;
    outfile << "#KEY_LS = 0x40 #left stick button" << std::endl;
    outfile << "#KEY_RS = 0x80 #right stick button" << std::endl;

    outfile << std::endl;
    outfile << "#DEBUG = 1" << std::endl;
    outfile << std::endl;
    outfile << "#   List of window titles to search for (up to 9 processes)" << std::endl;
    outfile << "PROCESS1 = xemu" << std::endl;
    outfile << "PROCESS2 = PCSX2" << std::endl;
    outfile << "#PROCESS3 = PCSX2" << std::endl;
    outfile << "#PROCESS4 = PCSX2" << std::endl;
    outfile << "#PROCESS5 = PCSX2" << std::endl;
    outfile << "#PROCESS6 = PCSX2" << std::endl;
    outfile << "#PROCESS7 = PCSX2" << std::endl;
    outfile << "#PROCESS8 = PCSX2" << std::endl;
    outfile << "#PROCESS9 = PCSX2" << std::endl;
    outfile << "#   More process can be added here" << std::endl;
    // End config dump

    outfile.close();
    
    file.open(fName.c_str());

    if (!file)
    {

      exitWithError("\nERROR! Configuration file " + fName + " still couldn't be found!\n");
    }
    else
    {

      printf("Success!\nNow using %s.\n", fName.c_str());
    }
  }

  //exitWithError("\nSafety exit!\n");

  std::string line;
  size_t lineNo = 0;
  while (std::getline(file, line))
  {
    lineNo++;
    std::string temp = line;

    if (temp.empty())
    {
      continue;
    }

    removeComment(temp);
    if (onlyWhitespace(temp))
    {
      continue;
    }

    parseLine(temp, lineNo);
  }

  file.close();
}

ConfigFile::ConfigFile(const std::string &fName)
{
  this->fName = fName;
  ExtractKeys();
}

bool ConfigFile::keyExists(const std::string &key) const
{
  return contents.find(key) != contents.end();
}

void ConfigFile::exitWithError(const std::string &error)
{
  std::cout << error;
  std::cin.ignore();
  std::cin.get();

  exit(EXIT_FAILURE);
}
