#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <chrono>
#include <tuple>

#define eight_puzzle_goal "012345678"
#define fifteen_puzzle_goal "0123456789ABCDEF"

using namespace std;
struct search_node
{
    search_node *parent = NULL;
    int path_cost = 0;
    int tiebreaker = 0;
    int f_value = 0;
    int order = 0;
    string state;
};

vector<search_node *> nodes_border;
unordered_map<string, int> visited_states;
int expanded_nodes;
int global_order;
double heuristic_value_sum;
int heuristic_value_calls;

int manhattan_distance(string state)
{
    int final_distance = 0;

    for (int i = 0; i < state.length(); i++)
    {
        int number = state[i] - '0';
        if (number)
        {
            int vertical_distance = abs(i % 3 - number % 3);
            int horizontal_distance = abs(floor(i / 3) - floor(number / 3));
            final_distance += (horizontal_distance + vertical_distance);
        }
    }

    heuristic_value_sum += final_distance;
    heuristic_value_calls++;
    return final_distance;
}

int manhattan_distance_15(string state)
{
    int final_distance = 0;

    for (int i = 0; i < state.length(); i++)
    {
        int number;
        if (state[i] - '0' < 10)
            number = state[i] - '0';
        else
            number = state[i] - '7';

        if (number)
        {
            int vertical_distance = abs(i % 4 - number % 4);
            int horizontal_distance = abs(floor(i / 4) - floor(number / 4));
            final_distance += (horizontal_distance + vertical_distance);
        }
    }

    heuristic_value_sum += final_distance;
    heuristic_value_calls++;
    return final_distance;
}

search_node *create_node(string state, search_node *parent)
{
    search_node *new_node = new search_node();
    new_node->parent = parent;
    new_node->state = state;
    return new_node;
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

string get_actions_15(int zero_position)
{
    string actions = "0000";
    if (floor(zero_position / 4) != 0)
    {
        actions[0] = '1';
    }

    if (zero_position % 4)
    {
        actions[1] = '1';
    }

    if (zero_position % 4 != 3)
    {
        actions[2] = '1';
    }

    if (floor(zero_position / 4) != 3)
    {
        actions[3] = '1';
    }
    return actions;
}

bool is_state_visited(string state)
{
    return visited_states.find(state) != visited_states.end();
}

void mark_state_visited(string state, int distance)
{
    visited_states[state] = distance;
}

void insert_in_queue(search_node *node)
{
    if (nodes_border.empty())
        nodes_border.push_back(node);
    else
    {
        int i = 0;
        while (i < nodes_border.size())
        {
            if (node->f_value < nodes_border[i]->f_value || (node->f_value == nodes_border[i]->f_value && node->tiebreaker <= nodes_border[i]->tiebreaker))
            {
                nodes_border.insert(nodes_border.begin() + i, node);
                break;
            }
            i++;
        }
        if (i == nodes_border.size())
        {
            nodes_border.push_back(node);
        }
    }
}

search_node *pop_from_queue()
{
    search_node *node = nodes_border[0];
    nodes_border.erase(nodes_border.begin());
    return node;
}

void print_state(string state)
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

// Min-heap for A* (min f, min h, LIFO)
void min_heap_insert_astar(search_node *node)
{
    nodes_border.push_back(node);
    int idx = nodes_border.size() - 1;
    while (idx > 0)
    {
        int parent = floor((idx - 1) / 2);
        if (nodes_border[idx]->f_value < nodes_border[parent]->f_value ||
            (nodes_border[idx]->f_value == nodes_border[parent]->f_value &&
             nodes_border[idx]->tiebreaker <= nodes_border[parent]->tiebreaker))
        {
            swap(nodes_border[idx], nodes_border[parent]);
            idx = parent;
        }
        else
        {
            break;
        }
    }
}

search_node *min_heap_pop_astar()
{
    if (nodes_border.empty())
        return NULL;
    search_node *top = nodes_border[0];
    nodes_border[0] = nodes_border.back();
    nodes_border.pop_back();
    int idx = 0, n = nodes_border.size();
    while (true)
    {
        int left = 2 * idx + 1, right = 2 * idx + 2, smallest = idx;
        if (left < n &&
            (nodes_border[left]->f_value < nodes_border[smallest]->f_value ||
             (nodes_border[left]->f_value == nodes_border[smallest]->f_value &&
              nodes_border[left]->tiebreaker < nodes_border[smallest]->tiebreaker) ||
             (nodes_border[left]->f_value == nodes_border[smallest]->f_value &&
              nodes_border[left]->tiebreaker == nodes_border[smallest]->tiebreaker && nodes_border[left]->order > nodes_border[smallest]->order)))
        {
            smallest = left;
        }
        if (right < n &&
            (nodes_border[right]->f_value < nodes_border[smallest]->f_value ||
             (nodes_border[right]->f_value == nodes_border[smallest]->f_value &&
              nodes_border[right]->tiebreaker < nodes_border[smallest]->tiebreaker) ||
             (nodes_border[right]->f_value == nodes_border[smallest]->f_value &&
              nodes_border[right]->tiebreaker == nodes_border[smallest]->tiebreaker && nodes_border[right]->order > nodes_border[smallest]->order)))
        {
            smallest = right;
        }
        if (smallest != idx)
        {
            swap(nodes_border[idx], nodes_border[smallest]);
            idx = smallest;
        }
        else
        {
            break;
        }
    }
    return top;
}
