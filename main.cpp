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
            cout << "Manhattan: " << manhattan_distance_8(root_state) << endl;
            j = 0;
        }
        else
        {
            root_state[j] = static_cast<short>(atoi(argv[i]));
            j++;
        }
    }

    // // Create a std::vector of integers
    // vector<int> dynamicVector;

    // // Preencher o vector de forma dinâmica
    // for (int i = 0; i < 10; ++i)
    // {
    //     dynamicVector.push_back(i * 10);
    // }

    // // Print the vector elements
    // cout << "Vector elements: ";
    // for (int i = 0; i < 10; ++i)
    // {
    //     cout << dynamicVector[i] << " ";
    // }
    // cout << endl;

    // std::vector automatically handles memory deallocation when it goes out of scope

    return 0;
}