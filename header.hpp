#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

#define eight_puzzle_goal static_cast<short>[0, 1, 2, 3, 4, 5, 6, 7, 8]
#define fifteen_puzzle_goal static_cast<short>[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

using namespace std;

struct search_node
{
    search_node *parent = NULL;
    int path_cost = 0;
    int heuristic_value = 0;
    short state[];
};

search_node create_node(short state[], search_node *parent, int heuristic_value)
{
    search_node new_node;
    new_node.parent = parent;
    new_node.heuristic_value = heuristic_value;
    for (int i;)
}

search_node *expand_node_8(search_node node[])
{
    search_node expanded_nodes[4];

    return expanded_nodes;
}

int manhattan_distance_8(short state[])
{
    int final_distance = 0;

    for (int i = 0; i < 9; i++)
    {
        if (state[i])
        {
            int vertical_distance = abs(i % 3 - state[i] % 3);
            int horizontal_distance = abs(floor(i / 3) - floor(state[i] / 3));
            final_distance += (horizontal_distance + vertical_distance);
        }
    }

    return final_distance;
}

int manhattan_distance_15(short state[])
{
    int final_distance = 0;

    for (int i = 0; i < 16; i++)
    {
        if (state[i])
        {
            int vertical_distance = abs(i % 4 - state[i] % 4);
            int horizontal_distance = abs(floor(i / 4) - floor(state[i] / 4));
            final_distance += (horizontal_distance + vertical_distance);
        }
    }
    return final_distance;
}

void resolve_puzzle(short root_state[], string alg)
{
    search_node root_node = create_node(root_state, NULL, manhattan_distance_8(root_state));
}