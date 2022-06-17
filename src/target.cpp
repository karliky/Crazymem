#include <iostream>
#include <windows.h>

int main()
{
    int32_t value = 12345;
    while (true)
    {
        std::cout << "value at: " << " " << value << " " << &value << "\n";
        Sleep(500);
    }
    return 0;
}