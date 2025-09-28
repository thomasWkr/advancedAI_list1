#include "header.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string alg = argv[1];
    string root_state;

    int j = 0;
    for (int i = 2; i < argc; ++i)
    {
        if (argv[i][1] == ',' || argv[i][2] == ',' || i == argc - 1)
        {
            root_state += argv[i][0];
            if (j != 8 && j != 15)
            {
                cout << "Número invalido de tiles" << endl;
                break;
            }
            resolve_puzzle(root_state, alg);
            root_state = "";
            j = 0;
        }
        else
        {
            root_state += argv[i];
            j++;
        }
    }

    return 0;
}