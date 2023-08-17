#include <windows.h> // for Beep()
#include <iostream>
#include <vector>
#include <list>
#include <xinput.h> // controller
#include <stdio.h> // for printf
#include <cmath> // for abs()
#include <mmdeviceapi.h> // vol
#include <endpointvolume.h> // vol
#include <tchar.h>
#include <ShlObj.h>

#include <map>

#include "CXBOXController.h"

#pragma once
class GP47
{
private:
  const int FPS = 5;                  // Update rate of the main GP47 loop. Interpreted as cycles-per-second.
  const int SLEEP_AMOUNT = 1000 / FPS;  // Number of milliseconds to sleep per iteration.
 

  XINPUT_STATE _currentState;

  bool _debug = false;           // Show console or not


  std::vector<std::string> _processes;   // processes to kill on key combo

  DWORD _triggerKeys=0xFFFF;


  CXBOXController* _controller;

public:

  GP47(CXBOXController* controller);

  void loadConfigFile();

  void loop();

  void setWindowVisibility(const bool& hidden) const;
};
