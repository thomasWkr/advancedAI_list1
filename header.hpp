#include "structs.hpp"

search_node *bfs()
{
    heuristic_value_calls = 1;
    visited_states.clear();

    if (nodes_border[0]->state == eight_puzzle_goal)
        return nodes_border[0];

    while (!nodes_border.empty())
    {
        search_node *current_node = nodes_border[0];
        nodes_border.erase(nodes_border.begin());

        if (is_state_visited(current_node->state))
        {
            delete current_node;
            continue;
        }

        expanded_nodes++;
        mark_state_visited(current_node->state, 0);

        int pos_zero = current_node->state.find('0');
        string actions = get_actions(pos_zero);

        int movement = -3;
        for (int i = 0; i < 4; i++)
        {
            if (actions[i] - '0')
            {
                search_node *new_node = create_node(current_node->state, current_node);
                int aux = new_node->state[pos_zero + movement];
                new_node->state[pos_zero + movement] = '0';
                new_node->state[pos_zero] = aux;
                if (new_node->state == eight_puzzle_goal)
                    return new_node;
                else if (!is_state_visited(new_node->state))
                    nodes_border.push_back(new_node);
                else
                    delete new_node;
            }
            movement += 2;
        }
    }
    return NULL;
}

search_node *limited_dfs(search_node *node, int depth_limit)
{
    if (node->state == eight_puzzle_goal)
        return node;
    if (depth_limit)
    {
        expanded_nodes++;

        int pos_zero = node->state.find('0');
        string actions = get_actions(pos_zero);

        int movement = -3;
        for (int i = 0; i < 4; i++)
        {
            if (actions[i] - '0')
            {
                search_node *new_node = create_node(node->state, node);
                int aux = new_node->state[pos_zero + movement];
                new_node->state[pos_zero + movement] = '0';
                new_node->state[pos_zero] = aux;
                if (!node->parent || !(new_node->state == node->parent->state))
                {
                    search_node *solution = limited_dfs(new_node, depth_limit - 1);
                    if (solution)
                        return solution;
                    else
                        delete solution;
                }
                else
                    delete new_node;
            }
            movement += 2;
        }
    }
    return NULL;
}

search_node *idfs(search_node *root_node)
{
    heuristic_value_calls = 1;
    for (int i = 0; i >= 0; i++)
    {
        search_node *solution = limited_dfs(root_node, i);
        if (solution)
            return solution;
        else
            delete solution;
    }
    return NULL;
}

search_node *gbfs()
{
    heuristic_value_calls = 0;
    visited_states.clear();

    if (nodes_border[0]->state == eight_puzzle_goal)
    {
        heuristic_value_calls++;
        return nodes_border[0];
    }

    while (!nodes_border.empty())
    {
        search_node *current_node;
        current_node = min_heap_pop_up_gbfs();

        if (is_state_visited(current_node->state))
        {
            delete current_node;
            continue;
        }

        mark_state_visited(current_node->state, current_node->path_cost);

        if (current_node->state == eight_puzzle_goal)
            return current_node;

        expanded_nodes++;
        int pos_zero = current_node->state.find('0');
        string actions = get_actions(pos_zero);

        int movement = -3;
        for (int i = 0; i < 4; i++)
        {
            if (actions[i] - '0')
            {
                search_node *new_node = create_node(current_node->state, current_node);
                int aux = new_node->state[pos_zero + movement];
                new_node->state[pos_zero + movement] = '0';
                new_node->state[pos_zero] = aux;
                new_node->path_cost = current_node->path_cost + 1;
                if (!current_node->parent || !(new_node->state == current_node->parent->state))
                {
                    min_heap_insert_gbfs(new_node);
                }
                else
                    delete new_node;
            }
            movement += 2;
        }
    }
    return NULL;
}

search_node *astar()
{
    heuristic_value_calls = 0;
    visited_states.clear();

    if (nodes_border[0]->state == eight_puzzle_goal)
    {
        heuristic_value_calls++;
        return nodes_border[0];
    }

    while (!nodes_border.empty())
    {
        search_node *current_node;
        current_node = min_heap_pop_up_astar();

        if (is_state_visited(current_node->state))
        {
            delete current_node;
            continue;
        }

        mark_state_visited(current_node->state, current_node->path_cost);

        if (current_node->state == eight_puzzle_goal)
            return current_node;

        expanded_nodes++;

        int pos_zero = current_node->state.find('0');
        string actions = get_actions(pos_zero);

        int movement = -3;
        for (int i = 0; i < 4; i++)
        {
            if (actions[i] - '0')
            {
                search_node *new_node = create_node(current_node->state, current_node);
                int aux = new_node->state[pos_zero + movement];
                new_node->state[pos_zero + movement] = '0';
                new_node->state[pos_zero] = aux;
                new_node->path_cost = current_node->path_cost + 1;
                if (!current_node->parent || !(new_node->state == current_node->parent->state))
                {
                    min_heap_insert_astar(new_node);
                }
                else
                    delete new_node;
            }
            movement += 2;
        }
    }
    return NULL;
}

void resolve_puzzle(string root_state, string alg)
{
    nodes_border.clear();
    expanded_nodes = 0;
    heuristic_value_sum = 0;

    auto start = chrono::high_resolution_clock::now();

    search_node *root_node = create_node(root_state, NULL);
    nodes_border.push_back(root_node);
    search_node *final_node;

    if (alg == "-bfs")
        final_node = bfs();
    else if (alg == "-idfs")
        final_node = idfs(root_node);
    else if (alg == "-gbfs")
        final_node = gbfs();
    else if (alg == "-astar")
        final_node = astar();

    if (final_node)
    {
        auto end = chrono::high_resolution_clock::now();

        auto duration = end - start;
        auto seconds = chrono::duration_cast<std::chrono::duration<double>>(duration).count();

        double heuristic_medium = heuristic_value_sum / heuristic_value_calls;

        cout << expanded_nodes << "," << solution_length(final_node) - 1 << "," << seconds << "," << heuristic_medium << "," << manhattan_distance(root_state) << endl;
        // print_path(final_node);
    }
}