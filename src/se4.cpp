////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bypasses Sniper Elite 4's cheat engine detection.
// https://github.com/PotatoMaster101/se4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <utility>      // std::pair
#include <cstring>      // std::memset
#include <windows.h>
#include <tlhelp32.h>

namespace {
    constexpr auto SE4_DX11{ L"SniperElite4_DX11.exe" };    // game process name (DX11)
    constexpr auto SE4_DX12{ L"SniperElite4_DX12.exe" };    // game process name (DX12)
    constexpr LONG SE4_THREADPRI_DX11{ 2 };                 // CE detection thread priority (DX11)
    constexpr LONG SE4_THREADPRI_DX12{ 2 };                 // CE detection thread priority (DX12)
}

// Returns the pid of the specified process name, 0 on error.
DWORD get_proc(const std::wstring& name) {
    PROCESSENTRY32 proc;
    std::memset(&proc, 0, sizeof proc);
    proc.dwSize = sizeof proc;

    DWORD pid = 0;
    auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snap, &proc)) {
        do {
            if (proc.szExeFile == name) {
                pid = proc.th32ProcessID;
                break;
            }
        } while (Process32Next(snap, &proc));
    }
    CloseHandle(snap);
    return pid;
}

// Returns the thread ID of a thread in the specified process and priority, 0 on error.
DWORD get_pri_thread(DWORD pid, LONG pri) {
    THREADENTRY32 thr;
    std::memset(&thr, 0, sizeof thr);
    thr.dwSize = sizeof thr;

    DWORD thid = 0;
    auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (Thread32First(snap, &thr)) {
        do {
            if (thr.th32OwnerProcessID == pid && thr.tpBasePri == pri) {
                thid = thr.th32ThreadID;
                break;
            }
        } while (Thread32Next(snap, &thr));
    }
    CloseHandle(snap);
    return thid;
}

// Kills the specified thread.
bool kill_thread(DWORD thid) {
    auto th = OpenThread(THREAD_TERMINATE, false, thid);
    if (th == NULL)
        return false;

    auto ret = TerminateThread(th, 0);
    CloseHandle(th);
    return ret;
}

// Returns the game process pid and whether if the game is running on DX12. 0 pid means error.
inline std::pair<DWORD, bool> get_game_proc() {
    auto pid = get_proc(SE4_DX11);
    return (pid == 0) ? std::pair{get_proc(SE4_DX12), true} : std::pair{pid, false};
}

// Entry point.
int main() {
    auto [pid, dx12] = get_game_proc();
    if (pid == 0) {
        std::cerr << "ERROR: Game process not found\n";
        return 1;
    }
    std::cout << "INFO: Game process found (" << pid << ")\n";

    auto thid = get_pri_thread(pid, dx12 ? SE4_THREADPRI_DX12 : SE4_THREADPRI_DX11);
    if (thid == 0) {
        std::cerr << "ERROR: Game thread not found\n";
        return 1;
    }
    std::cout << "INFO: Game thread found (" << thid << ")\n";

    std::cout << "INFO: Killing thread " << thid << "\n";
    if (!kill_thread(thid)) {
        std::cerr << "ERROR: Cannot kill thread\n";
        return 1;
    }
    std::cout << "INFO: Killed thread " << thid << "\n";
    return 0;
}
