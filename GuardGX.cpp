#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <string>
#include <algorithm>

std::string ToLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool IsOperaInstaller(const std::wstring& filename) {
    int size = WideCharToMultiByte(CP_UTF8, 0, filename.c_str(), (int)filename.size(), NULL, 0, NULL, NULL);
    std::string fileStr(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, filename.c_str(), (int)filename.size(), &fileStr[0], size, NULL, NULL);
    fileStr = ToLower(fileStr);

    if (fileStr.length() < 4 || fileStr.substr(fileStr.length() - 4) != ".exe") {
        return false;
    }

    return (fileStr.find("operaairsetup") != std::string::npos ||
            fileStr.find("operagxsetup") != std::string::npos ||
            fileStr.find("operasetup") != std::string::npos);
}

int main() {
    FreeConsole();

    PWSTR path = NULL;
    std::wstring downloadsPath;

    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path))) {
        downloadsPath = path;
        CoTaskMemFree(path);
    }

    if (downloadsPath.empty()) return 1;

    HANDLE hDir = CreateFileW(downloadsPath.c_str(), FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (hDir == INVALID_HANDLE_VALUE) return 1;

    char buffer[4096];
    DWORD bytesReturned;

    while (true) {
        if (ReadDirectoryChangesW(hDir, &buffer, sizeof(buffer), FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION,
            &bytesReturned, NULL, NULL)) {

            FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)buffer;

            do {
                std::wstring filename(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));

                if ((pNotify->Action == FILE_ACTION_ADDED || 
                     pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME) &&
                    IsOperaInstaller(filename)) {

                    std::wstring fullPath = downloadsPath + L"\\" + filename;
                    Sleep(200);

                    for (int i = 0; i < 5; i++) {
                        if (DeleteFileW(fullPath.c_str())) break;
                        Sleep(100);
                    }
                }

                if (pNotify->NextEntryOffset == 0) break;
                pNotify = (FILE_NOTIFY_INFORMATION*)((BYTE*)pNotify + pNotify->NextEntryOffset);

            } while (true);
        }
    }

    CloseHandle(hDir);
    return 0;
}
