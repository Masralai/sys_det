#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <winreg.h>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>


using std::string;
using std::to_string;
using std::vector;
using std::max;
using std::stringstream;
using std::getline;
using std::cout;
using std::endl;

const string LOGO = R"(

)";


struct InfoPair {
    string key;
    string value;


};

InfoPair getUsername() {
    char buffer[257];
    DWORD size = sizeof(buffer);
    GetComputerNameA(buffer, &size);
    return { "Hostname",string(buffer) };
}

InfoPair getOSInfo() {
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
        return { "OS","N/A" };
    }

    char buffer[256];
    DWORD buffersize = sizeof(buffer);

    if (RegQueryValueEx(hkey, TEXT("ProductName"), NULL, NULL, (LPBYTE)buffer, &buffersize) == ERROR_SUCCESS) {
                RegCloseKey(hkey);
        return { "OS",string(buffer) };
    }

    RegCloseKey(hkey);
    return { "OS","N/A" };
}

InfoPair getCPUInfo() {
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
        return { "CPU","N/A" };
	}

	char buffer[256];
	DWORD buffersize = sizeof(buffer);
    if (RegQueryValueEx(hkey, TEXT("ProcessorNameString"), NULL, NULL, (LPBYTE)buffer, &buffersize) == ERROR_SUCCESS) {
        RegCloseKey(hkey);
        return { "CPU",string(buffer) };
	}
    RegCloseKey(hkey);
	return { "CPU","N/A" };

}
InfoPair getMemoryInfo() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    unsigned long long totalMB = statex.ullTotalPhys / (1024 * 1024);
	unsigned long long freeMB = statex.ullAvailPhys / (1024 * 1024);
	unsigned long long usedMB = totalMB - freeMB;

	string mem_str = to_string(totalMB) + " MB (Used: " + to_string(usedMB) + " MB, Free: " + to_string(freeMB) + " MB)";
	return { "Memory", mem_str };
}

InfoPair getGPUInfo() {
        HKEY hkey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000"), 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
        return { "GPU","N/A" };
    }
    char buffer[256];
    DWORD buffersize = sizeof(buffer);
    if (RegQueryValueEx(hkey, TEXT("DriverDesc"), NULL, NULL, (LPBYTE)buffer, &buffersize) == ERROR_SUCCESS) {
        RegCloseKey(hkey);
        return { "GPU",string(buffer) };
    }
    RegCloseKey(hkey);
	return { "GPU","N/A" };
}

InfoPair getDiskInfo() {
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    if (GetDiskFreeSpaceEx(NULL, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        unsigned long long totalGB = totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024);
        unsigned long long freeGB = totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024);
        unsigned long long usedGB = totalGB - freeGB;
        string disk_str = to_string(totalGB) + " GB (Used: " + to_string(usedGB) + " GB, Free: " + to_string(freeGB) + " GB)";
        return { "Disk", disk_str };
    }
    return { "Disk","N/A" };
}



InfoPair getResolutionInfo() {
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	string res_str = to_string(screenWidth) + "x" + to_string(screenHeight);
	return { "Resolution", res_str };
}

vector<string> splitString(const string& str) {
    stringstream ss(str);
    string line;
	vector<string> lines;

    if(str.rfind("\n", 0) == 0) {
        getline(ss, line);
	}
    while (getline(ss, line)) {
        lines.push_back(line);
    }
    return lines;
}

void printSystemInfo() {
    vector<InfoPair> infoList;
	infoList.push_back(getUsername());
    infoList.push_back(getOSInfo());
	infoList.push_back(getCPUInfo());
	infoList.push_back(getMemoryInfo());
	infoList.push_back(getGPUInfo());
	infoList.push_back(getDiskInfo());
	infoList.push_back(getResolutionInfo());

    vector<string> logoLines = splitString(LOGO);
	size_t max_logo_width = 0;
    for (const auto& line : logoLines) {
        max_logo_width = max(max_logo_width, line.length());
    }

    
    size_t max_lines = max(logoLines.size(), infoList.size());
    for (size_t i= 0;i < max_lines;++i) {
        if (i < logoLines.size()) {
			cout << logoLines[i]<<endl;
			cout << string(max_logo_width - logoLines[i].length(), ' ');
        }
        else {
			cout << string(max_logo_width, ' ');
        }

        cout << "  ";

        if (i < infoList.size()) {
            cout << "  " << infoList[i].key << ": " << infoList[i].value<<endl;
        }
    }
}

int main()
{   
    printSystemInfo();
}   
