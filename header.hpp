#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>

short eight_puzzle_goal[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
short fifteen_puzzle_goal[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

using namespace std;
struct search_node
{
    search_node *parent = NULL;
    int path_cost = 0;
    int heuristic_value = 0;
    short state[9];
};

vector<search_node *> nodes_queue;
set<vector<short>> visited_states;
int expanded_nodes;
int heuristic_value_sum;
int heuristic_value_calls;

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

    heuristic_value_sum += final_distance;
    heuristic_value_calls++;
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

search_node *create_node(short state[], search_node *parent, int heuristic_value)
{
    search_node *new_node = new search_node();
    new_node->parent = parent;
    new_node->heuristic_value = heuristic_value;
    for (int i = 0; i < 9; i++)
    {
        new_node->state[i] = state[i];
    }
    return new_node;
}

int find_zero(short state[])
{
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

bool compare_states(short state1[], short state2[])
{
    for (int i = 0; i < 9; i++)
    {
        if (state1[i] != state2[i])
        {
            return false;
        }
    }
    return true;
}

int solution_length(search_node *final_node)
{
    int length = 0;
    search_node *aux_node = final_node;
    while (aux_node)
    {
        aux_node = aux_node->parent;
        length++;
    }
    return length;
}

string get_actions(int zero_position)
{
    string actions = "0000";
    if (floor(zero_position / 3) != 0)
    {
        actions[0] = '1';
    }

    if (zero_position % 3)
    {
        actions[1] = '1';
    }

    if (zero_position % 3 != 2)
    {
        actions[2] = '1';
    }

    if (floor(zero_position / 3) != 2)
    {
        actions[3] = '1';
    }
    return actions;
}

bool is_state_visited(short state[])
{
    vector<short> state_vector(state, state + 9);
    return visited_states.find(state_vector) != visited_states.end();
}

void mark_state_visited(short state[])
{
    vector<short> state_vector(state, state + 9);
    visited_states.insert(state_vector);
}

search_node *bfs()
{
    expanded_nodes = 0;
    heuristic_value_sum = 0;
    heuristic_value_calls = 1;
    visited_states.clear();

    if (compare_states(nodes_queue[0]->state, eight_puzzle_goal))
        return nodes_queue[0];

    while (!nodes_queue.empty())
    {
        search_node *current_node = nodes_queue[0];
        nodes_queue.erase(nodes_queue.begin());

        if (is_state_visited(current_node->state))
        {
            delete current_node;
            continue;
        }

        expanded_nodes++;
        mark_state_visited(current_node->state);

        int pos_zero = find_zero(current_node->state);
        string actions = get_actions(pos_zero);

        int movement = -3;
        for (int i = 0; i < 4; i++)
        {
            if (actions[i] - '0')
            {
                search_node *new_node = create_node(current_node->state, current_node, 0);
                int aux = new_node->state[pos_zero + movement];
                new_node->state[pos_zero + movement] = 0;
                new_node->state[pos_zero] = aux;
                if (compare_states(new_node->state, eight_puzzle_goal))
                    return new_node;
                else if (!is_state_visited(new_node->state))
                    nodes_queue.push_back(new_node);
                else
                    delete new_node;
            }
            movement += 2;
        }
    }
    return NULL;
}

void print_state(short state[])
{
    cout << state[0] << state[1] << state[2] << endl;
    cout << state[3] << state[4] << state[5] << endl;
    cout << state[6] << state[7] << state[8] << endl
         << endl;
}

void print_path(search_node *node)
{
    search_node *aux_node = node;
    while (aux_node)
    {
        print_state(aux_node->state);
        aux_node = aux_node->parent;
    }
}

void resolve_puzzle(short root_state[], string alg)
{
    nodes_queue.clear();
    search_node *root_node = create_node(root_state, NULL, manhattan_distance_8(root_state));
    nodes_queue.push_back(root_node);
    search_node *final_node = bfs();

    if (final_node)
    {
        float heuristic_medium = heuristic_value_sum / heuristic_value_calls;
        cout << expanded_nodes << "," << solution_length(final_node) - 1 << "," << heuristic_medium << "," << root_node->heuristic_value << endl;
        // print_path(final_node);
    }
}