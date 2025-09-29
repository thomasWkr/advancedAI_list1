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
            string number = "";
            if (strchr(argv[i], ',') || i == argc - 1)
            {
                for (int k = 0; argv[i][k] != '\0'; ++k)
                {
                    if (argv[i][k] != ',')
                        number += argv[i][k];
                }

                if (stoi(number) < 10)
                    root_state += stoi(number) + '0';
                else
                    root_state += stoi(number) + '7';

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
                number += argv[i];
                if (stoi(number) < 10)
                    root_state += stoi(number) + '0';
                else
                    root_state += stoi(number) + '7';
                j++;
            }
        }
    }
    else
    {
        string line;
        while (getline(cin, line))
        {
            if (line.empty())
                continue;
            if (line.size() == 15)
            {
                line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
                resolve_puzzle(line, alg);
            }
            else if (line.size() == 37)
            {
                string root_state = "";
                string number = "";
                for (int i = 0; i <= line.length(); i++)
                {
                    if (line[i] != ' ' && line[i] != '\0')
                        number += line[i];
                    else
                    {
                        if (stoi(number) < 10)
                            root_state += stoi(number) + '0';
                        else
                            root_state += stoi(number) + '7';
                        number = "";
                    }
                }
                resolve_puzzle(root_state, alg);
            }
            else
            {
                cout << "Número invalido de tiles: " << line << endl;
            }
        }
    }

    return 0;
}