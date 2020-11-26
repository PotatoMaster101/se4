# Sniper Elite 4 Anti-Cheat Engine Bypass
Bypasses [Sniper Elite 4](https://sniperelitegames.com/en/games/sniper-elite-4)'s anti-cheat engine feature.

Tested in the latest steam release (both DX11 and DX12).

## How it Works
The game uses a low priority thread to constantly monitor opened processes and search for cheat engine. If the process is found by the game, the game will be closed. The bypass will terminate the low priority thread.

## How to Compile
1. Install [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)
2. Open the project file `se4.sln` using Visual Studio 2019
3. Build -> Build Solution

## How to Use
1. Run Sniper Elite 4
2. Run the program via CMD or PowerShell
3. If no error, run cheat engine
