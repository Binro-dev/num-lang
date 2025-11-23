#pragma once 

#include "../include.hpp"
#include "../tokenizer/tokenizer.hpp"

// structure stack for stack usage like loops and all (this is only needed, no full lib is needed!)
template <typename T = int64_t>
struct stack 
{
public :
    
    // push a new value
    void push(T value)
    {
        arr.push_back(value);
    }
    
    // pop the value from the top
    void pop()
    {
        arr.pop_back();
    }
    
    // returns the value from the top
    T top()
    {
        if (arr.empty()) 
        {
            return T();
        }
        return arr[arr.size() - 1];
    }
    
    // returns true if the stack is empty
    bool empty()
    {
        return arr.empty();
    }

private : 
    std::vector<T> arr;
};

// a loop address struct 
struct loop_t 
{
    size_t ip;
    
    int call_target = -1;
    int called = 0;

    bool infinite_loop = true;
};

// struct for holding numbers from the code between '<.*'
struct num_value
{
    int64_t number = 1;
    bool is_there_num = true; // for cases like ERROR or no NUMBER !
};

// interpreter class 
class NumInterpreter
{
public :
    NumInterpreter(std::vector<Token> &tks) : tokens(tks) {}
    void interpret(); // will interpret the code  
    
    void Error(std::string content, Token token);

private : // variables 
    std::vector<Token> tokens;

    std::vector<int64_t> cells = {0};
    int64_t ip = 0;

    int64_t current_cell = 0;
    int current_line = 1;

    stack<loop_t> loop_stack;

    int64_t num_v = 0; // this is the global 'num'. You can do addition, sub, mul and div in it (only)

private : // functions 
    void advance();
    Token next();
    Token peek();

    void resize_cell(int size = -1);
    void get_number(num_value &buf, bool must_need_a_num = false, bool must_positive = false);

    void cell_value_change(int iod);
    void cell_value_clear();

    void display_char();
    void read_char();

    void loop_start(int64_t call_target = -1);
    void loop_end();
    void loop_start_with_counter();

    void change_cell(int iod = 1, bool must_need_value = false);

    void oper_num(char oper);
    void clear_num(); 
};
