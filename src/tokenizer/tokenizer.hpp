#pragma once

#include "../include.hpp"
#include <sstream>

enum class TokenType
{
    Number,

    Dot,
    Star,
    Dollar, 
    And,

    Rbra,
    Lbra,
    Rsqb, 
    Lsqb,

    Add, 
    Dash,
    Bslash
};

struct Token 
{
    TokenType type;

    int line;
    int col;

    std::string full_line;
    std::string val;
};

class Tokenizer 
{
public : // functions
    Tokenizer(std::string &CODE, bool debuG) : code(CODE), debug(debuG)
    {
        std::istringstream ss(code);
        std::string s;

        while (getline(ss, s, '\n'))
            full_lines.push_back(s);
    }

    std::vector<Token> tokenize();

private : // variables 
    std::string code;
    bool debug;

    bool in_comment = false;

    std::vector<Token> tokens;
    std::vector<std::string> full_lines;

    int line = 1;
    int col = 1;

    size_t ip = 0;

private : // functions
    void debug_optput(); // to display tokens detials 
    std::string get_line();
    
    void advance();
    
    bool tokenize_comment();
    bool tokenize_number();
    bool tokenize_dot();
    bool tokenize_star();
    bool tokenize_lbra();
    bool tokenize_rbra();
    bool tokenize_dollar();
    bool tokenize_and();
    bool tokenize_add();
    bool tokenize_dash();
    bool tokenize_bslash();
    bool tokenize_rsqb();
    bool tokenize_lsqb();
};
