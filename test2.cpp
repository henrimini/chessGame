#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <sstream>

using namespace std;
string fileToRead = "saj//saj4.txt";
vector<string> result;
int max_depth;

vector<string> split(const string &str, const string &delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos)
            pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}
main()
{
    string s = "oki_is_good";

    cout << split(s, "_")[1] << endl;
}
