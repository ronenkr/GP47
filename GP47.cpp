#include "GP47.h"
#include "ConfigFile.h"
#include <codecvt>
#include <locale>

GP47::GP47(CXBOXController * controller)
  : _controller(controller)
{

    _currentState = { 0 };

}

// Description:
//   Reads and parses the configuration file, assigning values to the 
//     configuration variables.
void GP47::loadConfigFile()
{
  ConfigFile cfg("config.ini");
  

  _triggerKeys = 0;
  _triggerKeys = strtol(cfg.getValueOfKey<std::string>("KEY_LB").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_RB").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_SELECT").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_START").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_UP").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_DOWN").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_LEFT").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_RIGHT").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_A").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_B").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_X").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_Y").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_LS").c_str(), 0, 0);
  _triggerKeys += strtol(cfg.getValueOfKey<std::string>("KEY_RS").c_str(), 0, 0);

  std::cout << "Triger keys: " << _triggerKeys << std::endl;

  //--------------------------------
  // Configuration bindings
  //--------------------------------


  for (int i = 1; i <= 10; i++) {
      std::string strName = "PROCESS";

      std::string strTemp = cfg.getValueOfKey<std::string>(strName+ std::to_string(i));
      if(strTemp.length()>0)
        _processes.push_back(strTemp);
  }

  std::cout << "Processes " << _processes.at(0) << std::endl;

  _debug = strtol(cfg.getValueOfKey<std::string>("DEBUG").c_str(), 0, 0) == 1 ? true : false;

  // Set the initial window visibility
  setWindowVisibility(!_debug);
}

BOOL CALLBACK EnumHomeWindowsProc(
    _In_ HWND   hwnd,
    _In_ LPARAM lParam
) {
    TCHAR text[1024];

    GetWindowText(hwnd, text, 1024);

    std::vector<std::string> *_processes= (std::vector<std::string> *)lParam;

    for (auto& s : *_processes) {
        //std::cout << s << std::endl;

        std::wstring wideString= std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);

        if (wideString.length()>0 && wcsstr(text, wideString.c_str())) {
            DWORD dwProcessID = 0;
            std::wcout << text << std::endl;
            GetWindowThreadProcessId(hwnd, &dwProcessID);
            if (dwProcessID != 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, TRUE, dwProcessID);
                if (hProcess != NULL) {
                    //Kill process
                    TerminateProcess(hProcess, 0);
                }
                else {
                    std::cout << "Error opening process id " << dwProcessID << std::endl;
                }
            }
        }
    }
    return TRUE;
}

// Description:
//   The main program loop, listen for trigger keys and executes a search for 
//   given windows and kill their processes
void GP47::loop()
{
  Sleep(SLEEP_AMOUNT);

  _currentState = _controller->GetState();

  if(_currentState.Gamepad.wButtons !=0)
    std::cout << _currentState.Gamepad.wButtons << std::endl;

  //look for configured keys

  if (_currentState.Gamepad.wButtons == _triggerKeys) {
      std::cout<<"Detected exit request"<< std::endl;

      //Do search and exit here
      EnumWindows(EnumHomeWindowsProc, (LPARAM)&_processes);
  }

}


// Description:
//   Either hides or shows the window.
// 
// Params:
//   hidden   Hides the window when true
void GP47::setWindowVisibility(const bool &hidden) const
{
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, hidden ? SW_HIDE : SW_SHOW);
}

