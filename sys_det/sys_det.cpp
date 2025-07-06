#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <winreg.h>
#include<string>
#include<vector>
using namespace std;


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

}



int main()
{   
    getUsername();
    system("pause");

    return 0;

}   

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
