/*-------------------------------------------------------------------------------
    GP47 free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------*/


#include <windows.h> // for Beep()
#include <iostream>


#pragma comment(lib, "XInput9_1_0.lib")

#include "GP47.h"

BOOL isRunningAsAdministrator(); // Check if administrator, makes on-screen keyboard clickable

int main()
{
  CXBOXController controller(1);
  GP47 gp47(&controller);
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTitle( TEXT( "GamePad47" ) );

  //printf("\nTo Exit the app press SELECT + START + RB");
  //SetConsoleTextAttribute(hConsole, 29);
  printf("\n\n-------------------------\n\n");

  //SetConsoleTextAttribute(hConsole, 5); // set color to purple on black (windows only)
  // 29 default

  if (!isRunningAsAdministrator())
  {
    printf("Not running as an admin! \n");
  }

  gp47.loadConfigFile();

  // dump important tips
  //printf("Tip - Press left and right bumpers simultaneously to toggle speeds! (Default is left and right bumpers, configurable in config.ini)\n");

  // Start the program loop
  while (true)
  {
      gp47.loop();
  }
}

BOOL isRunningAsAdministrator()
{
  BOOL   fRet = FALSE;
  HANDLE hToken = NULL;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
  {
    TOKEN_ELEVATION Elevation;
    DWORD cbSize = sizeof( TOKEN_ELEVATION );

    if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof( Elevation), &cbSize))
    {
      fRet = Elevation.TokenIsElevated;
    }
  }

  if (hToken)
  {
    CloseHandle(hToken);
  }

  return fRet;
}