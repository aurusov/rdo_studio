#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <queue>
#include <vector>

#define SOLUTION_FOUND true
#define NO_SOLUTION false

#define LOG std::cout
#define ERR std::cout << "Error: "

class State
{
public:
    State(const State& state_);
    State() {}
    void Swap(uint32_t s_idx, uint32_t d_idx);
    std::vector<uint32_t> state;
};

class CaseSolver
{
public:
    CaseSolver(uint32_t wd, uint32_t hg, const State& first_set);
    bool Solve();
    uint32_t GetAnswer() const;
    static uint32_t ZeroPosition(const State& set_);
    ~CaseSolver()
    {
        delete root;
    }
private:

    class Node
    {
    public:
        Node(const State& state_, Node* parrent_);
        const State& GetState() const;
        void SwapState(uint32_t s_idx, uint32_t d_idx);
        void AddChild(const Node* child_);
        bool CompareState(const State& state_) const;
        const Node* GetParrent() const;
        ~Node()
        {
            for(auto it = child.begin(); it != child.end(); it++)
            {
                delete *it;
            }
        }
    private:
        const Node* parrent;
        std::vector<const Node*> child;
        State state;
    };

    class GameBoard
    {
    public:
        GameBoard(uint32_t width_, uint32_t height_):
                                      width(width_), height(height_)
        {}
        uint32_t GetRow(uint32_t pos) const;
        uint32_t GetCol(uint32_t pos) const;
        uint32_t GetPos(uint32_t row, uint32_t col) const;
        void GenerateNexSteps(Node* parrent,  std::queue<Node*>& queue_) const;
        uint32_t GetSize() const
        {
            return width*height;
        }
    private:
        uint32_t width;
        uint32_t height;
    };

private:
    bool isVisited(const State& state_) const;
    GameBoard board;
    const Node* answer;
    std::vector<const Node*> visited;
    std::queue<Node*> queue;
    State right_comb;
    Node* root;
};

class Puzzle
{
public:
    Puzzle(uint32_t wd, uint32_t hg);
    bool SolveAll();
    bool DumpAllCases(const char* file_path) const;
private:
    bool GenerateNexCase();
    bool IsSolvable() const;
    bool SolveCase();
    State set;
    uint32_t width;
    uint32_t height;
    std::vector<std::pair<State, uint32_t> > solution;
};