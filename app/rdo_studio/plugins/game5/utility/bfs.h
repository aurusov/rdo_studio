#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <memory>

static const bool SOLUTION_FOUND = true;
static const bool NO_SOLUTION = false;

namespace
{
    std::ostream& LOG()
    {
        return std::cout;
    }

    std::ostream& ERR()
    {
        return std::cout << "Error: ";
    }
}

typedef std::vector<std::uint32_t> State;

class CaseSolver
{
public:
    CaseSolver(std::uint32_t wd, std::uint32_t hg, const State& first_set);
    bool solve();
    std::uint32_t getAnswer() const;
    static std::uint32_t zeroPosition(const State& set);
private:

    class Node
    {
    public:
        Node(std::shared_ptr<const Node> parrent);
        Node(const State& state);
        const State& getState() const;
        void swapState(std::uint32_t left, std::uint32_t right);
        void addChild(std::shared_ptr<const Node> child_);
        bool compareState(const State& state_) const;
        std::shared_ptr<const Node> getParrent() const;
    private:
        std::weak_ptr<const Node> parrent;
        std::vector<std::weak_ptr<const Node> > child;
        State state;
    };

    class GameBoard
    {
    public:
        GameBoard(std::uint32_t width, std::uint32_t height);
        std::uint32_t getRow(std::uint32_t pos) const;
        std::uint32_t getColumn(std::uint32_t pos) const;
        std::uint32_t getPos(std::uint32_t row, std::uint32_t col) const;
        void generateNexSteps(std::shared_ptr<Node> parrent,  std::queue<std::shared_ptr<Node> >& queue) const;
        std::uint32_t getSize() const
        {
            return width*height;
        }
    private:
        std::uint32_t width;
        std::uint32_t height;
    };

private:
    bool isVisited(const State& state) const;
    GameBoard board;
    std::shared_ptr<const Node> answer;
    std::vector<std::shared_ptr<const Node> > visited;
    std::queue<std::shared_ptr<Node> > queue;
    State right_comb;
    std::shared_ptr<Node> root;
};

class Puzzle
{
public:
    Puzzle(std::uint32_t wd, std::uint32_t hg);
    bool solveAll();
    bool dumpAllCases(const char* file_path) const;
private:
    bool generateNexCase();
    bool isSolvable() const;
    bool solveCase();
    State set;
    std::uint32_t width;
    std::uint32_t height;
    std::vector<std::pair<State, std::uint32_t> > solution;
};
