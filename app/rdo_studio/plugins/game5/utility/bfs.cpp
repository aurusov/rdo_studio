#include <qt5/QtCore/qscopedpointer.h>
#include "bfs.h"

//***************Puzzle methods******************
Puzzle::Puzzle(std::uint32_t wd, std::uint32_t hg): width(wd), height(hg)
{
    // Generate the first set
    for (std::uint32_t i = 0; i < width*height; i++)
    {
        set.push_back(i);
    }
}

bool Puzzle::solveAll()
{
#ifndef NO_LOG
    std::uint32_t i_solve = 0;
    std::uint32_t i_wrong = 0;
#endif
    do
    {
        if (width != height || isSolvable())
        {
#ifndef NO_LOG
            if (!solveCase())
	    {
                i_wrong++;
            }
            else
            {
                i_solve++;
                LOG() << "set #" << i_solve;
                for (std::uint32_t i = 0; i < set.size(); i++)
                {
                    LOG() << " " << set[i];
                }
                LOG() << " answer: " << solution.back().second << std::endl;
            }
#else
            SolveCase();
#endif
        }
    } while (generateNexCase());
#ifndef NO_LOG
    LOG() << "************************" << std::endl;
    LOG() << "SOLVED: " << i_solve << " cases." << std::endl;
    LOG() << "NO SOLUTION: " << i_wrong <<  " cases." << std::endl;
    LOG() << "************************" << std::endl;
#endif
    return EXIT_SUCCESS;
}

bool Puzzle::generateNexCase()
{
    return std::next_permutation(set.begin(), set.end());
}

bool Puzzle::isSolvable() const
{
    std::uint32_t N = 0;
    for (std::uint32_t i = 0; i < set.size(); i++)
    {
        std::uint32_t k = 0;
        for (std::uint32_t j = i + 1; j < set.size(); j++)
        {
            if (set[j] < set[i] && set[j] != 0)
            {
                k++;
            }
        }
        N += k;
    }
    if (width%2 == 0)
    {
        N += CaseSolver::zeroPosition(set)/width + 1;
    }
    return !(N%2);
}

bool Puzzle::solveCase()
{
    CaseSolver casesolver(width, height, set);
    if (casesolver.solve() == SOLUTION_FOUND)
    {
        solution.push_back(std::make_pair(set, casesolver.getAnswer()));
        return SOLUTION_FOUND;
    }
    return NO_SOLUTION;
}

bool Puzzle::dumpAllCases(const char* file_path) const
{
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    if (file.fail())
    {
        ERR() << "unable to open file" << std::endl;
        return EXIT_FAILURE;
    }
    // Dump header
    std::uint32_t state_size = set.size();
    file.write(reinterpret_cast<const char*>(&state_size), sizeof(std::uint32_t));
    // Dump content
    for (auto it = solution.begin(); it != solution.end(); it++)
    {
        for (auto it_v = it->first.begin(); it_v != it->first.end(); it_v++)
        {
            file.write(reinterpret_cast<const char*>(&*it_v), sizeof(std::uint32_t));
        }
        file.write(reinterpret_cast<const char*>(&it->second), sizeof(std::uint32_t));
    }
    if (file.fail())
    {
        ERR() << "unable to write into the file" << std::endl;
        file.close();
        return EXIT_FAILURE;
    }
    file.close();
    return EXIT_SUCCESS;
}
//***************************************************

//***************CaseSolver methods******************
CaseSolver::CaseSolver(std::uint32_t wd, std::uint32_t hg, const State& first_set):
                                        board(wd, hg), answer(nullptr)
{
    // Initialize the root node
    std::shared_ptr<Node> node(new Node(first_set));
    root = node;
    queue.push(root);
    // Generate the right combination
    std::uint32_t i;
    for (i = 1; i < board.getSize(); i++)
    {
        right_comb.push_back(i);
    }
    right_comb.push_back(0);
}

bool CaseSolver::solve()
{
    std::shared_ptr<Node> current;
    while (!queue.empty() && answer == nullptr)
    {
        current = queue.front();
        queue.pop();
        if (current->compareState(right_comb))
        {
            answer = current;
        }
        else if (!isVisited(current->getState()))
        {
            board.generateNexSteps(current, queue);
        }
        visited.push_back(current);
    }
    if (answer == nullptr)
    {
        return NO_SOLUTION;
    }
    return SOLUTION_FOUND;
}

std::uint32_t CaseSolver::getAnswer() const
{
    std::uint32_t result = 0;
    std::shared_ptr<const Node> current = answer;
    while (current->getParrent() != nullptr)
    {
        current = current->getParrent();
        result++;
    }
    return result;
}

bool CaseSolver::isVisited(const State& state) const
{
    if (visited.size() == 0)
    {
        return false;
    }
    bool result = false;
    for (std::uint32_t i = 0; i < visited.size() && !result; i++)
    {
        if (visited[i]->compareState(state))
        {
            result = true;
        }
    }
    return result;
}

std::uint32_t CaseSolver::zeroPosition(const State& set)
{
    std::uint32_t i;
    for(i = 0; set[i] != 0; i++);
    return i;
}
//***************************************************

//*******************Node methods********************
CaseSolver::Node::Node(std::shared_ptr<const Node> parrent):
                       parrent(parrent), state(parrent->getState())
{}

CaseSolver::Node::Node(const State& state):
                       state(state)
{}

const State& CaseSolver::Node::getState() const
{
    return state;
}

void CaseSolver::Node::swapState(std::uint32_t left, std::uint32_t right)
{
    std::swap(state[left], state[right]);
}

void CaseSolver::Node::addChild(std::shared_ptr<const Node> child_)
{
    child.push_back(child_);
}

bool CaseSolver::Node::compareState(const State& state_) const
{
    bool result = true;
    for (std::uint32_t i = 0; i < state_.size() && result; i++)
    {
        if (state[i] != state_[i])
        {
            result = false;
        }
    }
    return result;
}

std::shared_ptr<const CaseSolver::Node> CaseSolver::Node::getParrent() const
{
    return parrent.lock();
}
//***************************************************

//*******************GameBoard methods********************
CaseSolver::GameBoard::GameBoard(std::uint32_t width, std::uint32_t height):
                                      width(width), height(height)
{}

std::uint32_t CaseSolver::GameBoard::getPos(std::uint32_t row, std::uint32_t col) const
{
    return row*width + col;
}

std::uint32_t CaseSolver::GameBoard::getRow(std::uint32_t pos) const
{
    return pos/width;
}

std::uint32_t CaseSolver::GameBoard::getColumn(std::uint32_t pos) const
{
    return pos%width;
}

void CaseSolver::GameBoard::generateNexSteps(std::shared_ptr<Node> parrent,  std::queue<std::shared_ptr<Node> >& queue) const
{
    std::uint32_t hole_pos = zeroPosition(parrent->getState());
    std::uint32_t hole_row = getRow(hole_pos);
    std::uint32_t hole_col = getColumn(hole_pos);
    if (hole_row != 0)
    {
        std::shared_ptr<Node> new_node(new Node(parrent));
        parrent->addChild(new_node);
        new_node->swapState(hole_pos, getPos(hole_row - 1, hole_col));
        queue.push(new_node);
    }
    if (hole_row != height - 1)
    {
        std::shared_ptr<Node> new_node(new Node(parrent));
        parrent->addChild(new_node);
        new_node->swapState(hole_pos, getPos(hole_row + 1, hole_col));
        queue.push(new_node);
    }
    if (hole_col != 0)
    {
        std::shared_ptr<Node> new_node(new Node(parrent));
        parrent->addChild(new_node);
        new_node->swapState(hole_pos, getPos(hole_row, hole_col - 1));
        queue.push(new_node);
    }
    if (hole_col != width - 1)
    {
        std::shared_ptr<Node> new_node(new Node(parrent));
        parrent->addChild(new_node);
        new_node->swapState(hole_pos, getPos(hole_row, hole_col + 1));
        queue.push(new_node);
    }
}
