#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main()
{
    string str("The:quick: brown :fox : jumped");

    regex reg("\\:");

    sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    sregex_token_iterator end;

    vector<string> vec(iter, end);

    for (auto a : vec)
    {
        cout << "|" << a << "|" << endl;
    }
}
