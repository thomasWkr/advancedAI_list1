#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#define eight_puzzle_goal "012345678"
#define fifteen_puzzle_goal "012345678ABCDE"

using namespace std;
struct search_node
{
    search_node *parent = NULL;
    short path_cost = 0;
    short tiebreaker = 0;
    short f_value = 0;
    string state;
};

vector<search_node *> nodes_border;
unordered_map<string, int> visited_states;
int expanded_nodes;
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

bool is_state_visited(string state)
{
    return visited_states.find(state) != visited_states.end();
}

void mark_state_visited(string state, int distance)
{
    visited_states[state] = distance;
}

int state_visited_distance(string state)
{
    return visited_states[state];
}

void min_heap_insert(search_node *node)
{
    nodes_border.push_back(node);
    int new_index = nodes_border.size() - 1;
    while (new_index)
    {
        int father_index = floor((new_index - 1) / 2);
        if (nodes_border[new_index]->f_value == nodes_border[father_index]->f_value)
        {
            if (nodes_border[new_index]->tiebreaker <= nodes_border[father_index]->tiebreaker)
            {
                swap(nodes_border[father_index], nodes_border[new_index]);
                new_index = father_index;
            }
            else
            {
                break;
            }
        }
        else if (nodes_border[new_index]->f_value < nodes_border[father_index]->f_value)
        {
            swap(nodes_border[father_index], nodes_border[new_index]);
            new_index = father_index;
        }
        else
            break;
    }
}

search_node *min_heap_pop_up()
{
    search_node *root_node = nodes_border[0];
    swap(nodes_border[0], nodes_border[nodes_border.size() - 1]);
    nodes_border.pop_back();

    if (nodes_border.empty())
        return root_node;

    int adjusted_index = 0;

    while (adjusted_index * 2 + 1 <= nodes_border.size() - 1)
    {
        int left_son_index = adjusted_index * 2 + 1;
        if (left_son_index == nodes_border.size() - 1)
        {
            if (nodes_border[adjusted_index]->f_value > nodes_border[left_son_index]->f_value ||
                (nodes_border[adjusted_index]->f_value == nodes_border[left_son_index]->f_value &&
                 nodes_border[adjusted_index]->tiebreaker >= nodes_border[left_son_index]->tiebreaker))
                swap(nodes_border[adjusted_index], nodes_border[left_son_index]);
            break;
        }
        else
        {
            int right_son_index = adjusted_index * 2 + 2;
            if (nodes_border[left_son_index]->f_value == nodes_border[right_son_index]->f_value)
            {
                if (nodes_border[left_son_index]->tiebreaker < nodes_border[right_son_index]->tiebreaker)
                {
                    if (nodes_border[adjusted_index]->f_value > nodes_border[left_son_index]->f_value ||
                        (nodes_border[adjusted_index]->f_value == nodes_border[left_son_index]->f_value &&
                         nodes_border[adjusted_index]->tiebreaker >= nodes_border[left_son_index]->tiebreaker))
                    {
                        swap(nodes_border[adjusted_index], nodes_border[left_son_index]);
                        adjusted_index = left_son_index;
                    }
                    else
                        break;
                }
                else
                {
                    if (nodes_border[adjusted_index]->f_value > nodes_border[right_son_index]->f_value ||
                        (nodes_border[adjusted_index]->f_value == nodes_border[right_son_index]->f_value &&
                         nodes_border[adjusted_index]->tiebreaker >= nodes_border[right_son_index]->tiebreaker))
                    {
                        swap(nodes_border[adjusted_index], nodes_border[right_son_index]);
                        adjusted_index = right_son_index;
                    }
                    else
                        break;
                }
            }
            else if (nodes_border[left_son_index]->f_value < nodes_border[right_son_index]->f_value)
            {
                if (nodes_border[adjusted_index]->f_value > nodes_border[left_son_index]->f_value ||
                    (nodes_border[adjusted_index]->f_value == nodes_border[left_son_index]->f_value &&
                     nodes_border[adjusted_index]->tiebreaker >= nodes_border[left_son_index]->tiebreaker))
                {
                    swap(nodes_border[adjusted_index], nodes_border[left_son_index]);
                    adjusted_index = left_son_index;
                }
                else
                    break;
            }
            else
            {
                if (nodes_border[adjusted_index]->f_value > nodes_border[right_son_index]->f_value ||
                    (nodes_border[adjusted_index]->f_value == nodes_border[right_son_index]->f_value &&
                     nodes_border[adjusted_index]->tiebreaker >= nodes_border[right_son_index]->tiebreaker))
                {
                    swap(nodes_border[adjusted_index], nodes_border[right_son_index]);
                    adjusted_index = right_son_index;
                }
                else
                    break;
            }
        }
    }

    return root_node;
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