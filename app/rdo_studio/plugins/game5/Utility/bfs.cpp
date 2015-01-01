#include "bfs.h"

//***************Puzzle methods******************
Puzzle::Puzzle(uint32_t wd, uint32_t hg): width(wd), height(hg)
{
    // Generate the first set
    for (uint32_t i = 0; i < width*height; i++)
    {
        set.state.push_back(i);
    }
}

bool Puzzle::SolveAll()
{
#ifndef NO_LOG
    uint32_t i_solve = 0;
    uint32_t i_wrong = 0;
#endif
    do
    {
        if (width != height || IsSolvable())
        {
#ifndef NO_LOG
            if (!SolveCase())
	    {
                i_wrong++;
            }
            else
            {
                i_solve++;
                LOG << "set #" << i_solve;
                for (uint32_t i = 0; i < set.state.size(); i++)
		{
                    LOG << " " << set.state[i];
                }
                LOG << " answer: " << solution.back().second << std::endl;
            }
#else
            SolveCase();
#endif
        }
    } while (GenerateNexCase());
#ifndef NO_LOG
    LOG << "************************" << std::endl;
    LOG << "SOLVED: " << i_solve << " cases." << std::endl;
    LOG << "NO SOLUTION: " << i_wrong <<  " cases." << std::endl;
    LOG << "************************" << std::endl;
#endif
    return EXIT_SUCCESS;
}

bool Puzzle::GenerateNexCase()
{
    return std::next_permutation(set.state.begin(), set.state.end());
}

bool Puzzle::IsSolvable() const
{
    uint32_t N = 0;
    for (uint32_t i = 0; i < set.state.size(); i++)
    {
        uint32_t k = 0;
        for (uint32_t j = i + 1; j < set.state.size(); j++)
	{
            if (set.state[j] < set.state[i] && set.state[j] != 0)
	    {
                k++;
            }
        }
        N += k;
    }
    if (width%2 == 0)
    {
        N += CaseSolver::ZeroPosition(set)/width + 1;
    }
    return !(N%2);
}

bool Puzzle::SolveCase()
{
    CaseSolver casesolver(width, height, set);
    if (casesolver.Solve() == SOLUTION_FOUND)
    {
        solution.push_back(std::make_pair(set, casesolver.GetAnswer()));
        return SOLUTION_FOUND;
    }
    return NO_SOLUTION;
}

bool Puzzle::DumpAllCases(const char* file_path) const
{
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    if (file.fail())
    {
        ERR << "unable to open file" << std::endl;
        return EXIT_FAILURE;
    }
    // Dump header
    uint32_t state_size = set.state.size();
    file.write(reinterpret_cast<const char*>(&state_size), sizeof(uint32_t));
    // Dump content
    for (auto it = solution.begin(); it != solution.end(); it++)
    {
        for (auto it_v = it->first.state.begin(); it_v != it->first.state.end(); it_v++)
	{
            file.write(reinterpret_cast<const char*>(&*it_v), sizeof(uint32_t));
        }
        file.write(reinterpret_cast<const char*>(&it->second), sizeof(uint32_t));
    }
    if (file.fail())
    {
        ERR << "unable to write into the file" << std::endl;
        file.close();
        return EXIT_FAILURE;
    }
    file.close();
    return EXIT_SUCCESS;
}
//***************************************************

//***************CaseSolver methods******************
CaseSolver::CaseSolver(uint32_t wd, uint32_t hg, const State& first_set):
                                        board(wd, hg), answer(NULL)
{
    // Initialize the root node
    Node* node = new Node(first_set, NULL);
    root = node;
    queue.push(root);
    // Generate the right combination
    uint32_t i;
    for (i = 1; i < board.GetSize(); i++)
    {
        right_comb.state.push_back(i);
    }
    right_comb.state.push_back(0);
}

bool CaseSolver::Solve()
{
    Node* current;
    while (!queue.empty() && answer == NULL)
    {
        current = queue.front();
        queue.pop();
        if (current->CompareState(right_comb))
        {
            answer = current;
        }
	else if (!isVisited(current->GetState()))
        {
            board.GenerateNexSteps(current, queue);
        }
        visited.push_back(current);
    }
    if (answer == NULL)
    {
        return NO_SOLUTION;
    }
    return SOLUTION_FOUND;
}

uint32_t CaseSolver::GetAnswer() const
{
    uint32_t result = 0;
    const Node* current = answer;
    while (current->GetParrent() != NULL)
    {
        current = current->GetParrent();
        result++;
    }
    return result;
}

bool CaseSolver::isVisited(const State& state_) const
{
    if (visited.size() == 0)
    {
        return false;
    }
    bool result = false;
    for (uint32_t i = 0; i < visited.size() && !result; i++)
    {
        if (visited[i]->CompareState(state_))
	{
            result = true;
        }
    }
    return result;
}

uint32_t CaseSolver::ZeroPosition(const State& set_)
{
    uint32_t i;
    for(i = 0; set_.state[i] != 0; i++);
    return i;
}
//***************************************************

//*******************Node methods********************
CaseSolver::Node::Node(const State& state_, Node* parrent_):
                        parrent(parrent_), state(state_)
{
    if (parrent_ != NULL)
    {
        parrent_->AddChild(this);
    }
}

const State& CaseSolver::Node::GetState() const
{
    return state;
}

void CaseSolver::Node::SwapState(uint32_t s_idx, uint32_t d_idx)
{
    state.Swap(s_idx, d_idx);
}

void CaseSolver::Node::AddChild(const Node* child_)
{
    child.push_back(child_);
}

bool CaseSolver::Node::CompareState(const State& state_) const
{
    bool result = true;
    for (uint32_t i = 0; i < state_.state.size() && result; i++)
    {
        if (state.state[i] != state_.state[i])
	{
            result = false;
        }
    }
    return result;
}

const CaseSolver::Node* CaseSolver::Node::GetParrent() const
{
    return parrent;
}
//***************************************************

//*******************State methods********************
State::State(const State& state_)
{
    state = state_.state;
}

void State::Swap(uint32_t s_idx, uint32_t d_idx)
{
    uint32_t mid = state[s_idx];
    state[s_idx] = state[d_idx];
    state[d_idx] = mid;
}
//***************************************************

//*******************GameBoard methods********************
uint32_t CaseSolver::GameBoard::GetPos(uint32_t row, uint32_t col) const
{
    return row*width + col;
}

uint32_t CaseSolver::GameBoard::GetRow(uint32_t pos) const
{
    return pos/width;
}

uint32_t CaseSolver::GameBoard::GetCol(uint32_t pos) const
{
    return pos%width;
}

void CaseSolver::GameBoard::GenerateNexSteps(Node* parrent,  std::queue<Node*>& queue_) const
{
    uint32_t hole_pos = ZeroPosition(parrent->GetState());
    uint32_t hole_row = GetRow(hole_pos);
    uint32_t hole_col = GetCol(hole_pos);
    if (hole_row != 0)
    {
        Node* new_node = new Node(parrent->GetState(), parrent);
        new_node->SwapState(hole_pos, GetPos(hole_row - 1, hole_col));
        queue_.push(new_node);
    }
    if (hole_row != height - 1)
    {
        Node* new_node = new Node(parrent->GetState(), parrent);
        new_node->SwapState(hole_pos, GetPos(hole_row + 1, hole_col));
        queue_.push(new_node);
    }
    if (hole_col != 0)
    {
        Node* new_node = new Node(parrent->GetState(), parrent);
        new_node->SwapState(hole_pos, GetPos(hole_row, hole_col - 1));
        queue_.push(new_node);
    }
    if (hole_col != width - 1)
    {
        Node* new_node = new Node(parrent->GetState(), parrent);
        new_node->SwapState(hole_pos, GetPos(hole_row, hole_col + 1));
        queue_.push(new_node);
    }
}