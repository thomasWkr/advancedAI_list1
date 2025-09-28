#include "header.hpp"
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Algoritmo não especificado!" << endl;
        return 1;
    }

    string alg = argv[1];

    // If there are command-line puzzle arguments, parse them
    if (argc > 2)
    {
        string root_state;
        int j = 0;
        for (int i = 2; i < argc; i++)
        {
            if (strchr(argv[i], ',') || i == argc - 1)
            {
                for (int k = 0; argv[i][k] != '\0'; ++k)
                {
                    if (argv[i][k] != ',')
                        root_state += argv[i][k];
                }
                if (j != 8 && j != 15)
                {
                    cout << "Número invalido de tiles" << endl;
                    root_state.clear();
                    j = 0;
                    continue;
                }
                resolve_puzzle(root_state, alg);
                root_state.clear();
                j = 0;
            }
            else
            {
                root_state += argv[i];
                j++;
            }
        }
    }
    else
    {
        string line;
        while (getline(cin, line))
        {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            if (line.empty())
                continue;
            if (line.size() == 8 || line.size() == 9 || line.size() == 15 || line.size() == 16)
            {
                resolve_puzzle(line, alg);
            }
            else
            {
                cout << "Número invalido de tiles: " << line << endl;
            }
        }
    }

    return 0;
}