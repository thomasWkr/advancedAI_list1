#include "header.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string alg = argv[1];
    short root_state[16];

    int j = 0;
    for (int i = 2; i < argc; ++i)
    {
        if (argv[i][1] == ',' || argv[i][2] == ',' || i == argc - 1)
        {
            root_state[j] = static_cast<short>(argv[i][0] - '0');
            if (j != 8 && j != 15)
            {
                cout << "Número invalido de tiles" << endl;
                break;
            }
            resolve_puzzle(root_state, alg);
            j = 0;
        }
        else
        {
            root_state[j] = static_cast<short>(atoi(argv[i]));
            j++;
        }
    }

    return 0;
}