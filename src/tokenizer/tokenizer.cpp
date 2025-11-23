#include "tokenizer.hpp"
#include "../error/error_types.hpp"

using std::vector;
using std::string;

std::string Tokenizer::get_line()
{
    return full_lines[line - 1];
}

vector<Token> Tokenizer::tokenize()
{
    while (ip < code.size())
    {
        if (ip >= code.size()) 
            break;
        
        bool tokens_done = 
            tokenize_comment()  ||
            tokenize_number()   ||
            tokenize_dot()      || 
            tokenize_star()     || 
            tokenize_lbra()     || 
            tokenize_rbra()     || 
            tokenize_dollar()   || 
            tokenize_and()      || 
            tokenize_add()      || 
            tokenize_dash()     || 
            tokenize_bslash()   || 
            tokenize_lsqb()     || 
            tokenize_rsqb() 
            ;

        if (!tokens_done && ip < 0)  
        {  
            std::cerr  
                << "num : Tokenizer Error.\n\n"  
                << "Invalid token at " << line << ':' << col << '\n'
                << "\n   " << line << "  |    " <<  get_line() << '\n'
                << "\n (value first byte) : \"" << code[ip] << "\"\n"
                << "ip : " << ip << std::endl;  
            exit(TOKENIZER_ERROR);  
        }

        advance();
    }  
    
    if (debug)
        debug_optput();

    return tokens;
}

void Tokenizer::debug_optput()
{
    for (Token t : tokens)
        std::cout 
            << "\n-----NUM TOKENIZER DEBUG OUTPUT-----\n" 
            << "line : " << t.line << '\n'
            << "col : " << t.col << '\n'
            << "value : " << t.val << '\n'
            << "Token Type : " << (int) t.type << '\n'
            << "condent(full line) : " << t.full_line << '\n'
            << std::endl;
}

void Tokenizer::advance()
{
    while (ip < code.size()) 
    {
        ip++;
        col++;

        if (code[ip] == '#')
        {
            tokenize_comment();
            continue;
        }

        else if (code[ip] == '\n')
        {
            line++;
            col = 0;
            continue;
        }

        else if (code[ip] == ' ') 
            continue;

        break;
    }
}

bool Tokenizer::tokenize_comment()
{
    if (code[ip] != '#')
        return false;

    in_comment = true;
    while (in_comment)
    {
        if (ip < code.size()) ip++;

        if (code[ip] == '\n')
        {
            in_comment = false;

            line++;
            col = 0;
        }
    }

    return true;
}

bool Tokenizer::tokenize_number()
{
    if (!isdigit(code[ip]) || in_comment)
        return false;

    string number_str;

    while (std::isdigit(code[ip]))
    {
        number_str.push_back(code[ip]);

        if (ip < code.size())
            ip++;
        else
            return false;
    }
    
    ip--;   

    tokens.push_back({TokenType::Number, line, col, get_line(), number_str});
    return true;
}

bool Tokenizer::tokenize_dot()
{
    if (code[ip] == '.')
    {
        tokens.push_back({TokenType::Dot, line, col, get_line(), "."});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_star()
{
    if (code[ip] == '*')
    {
        tokens.push_back({TokenType::Star, line, col, get_line(), "*"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_lbra()
{
    if (code[ip] == '(')
    {
        tokens.push_back({TokenType::Lbra, line, col, get_line(), "("});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_rbra()
{
    if (code[ip] == ')')
    {
        tokens.push_back({TokenType::Rbra, line, col, get_line(), ")"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_dollar()
{
    if (code[ip] == '$')
    {
        tokens.push_back({TokenType::Dollar, line, col, get_line(), "$"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_and()
{
    if (code[ip] == '&')
    {
        tokens.push_back({TokenType::And, line, col, get_line(), "&"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_add()
{
    if (code[ip] == '+')
    {
        tokens.push_back({TokenType::Add, line, col, get_line(), "+"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_dash()
{
    if (code[ip] == '-')
    {
        tokens.push_back({TokenType::Dash, line, col, get_line(), "-"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_bslash()
{
    if (code[ip] == '/')
    {
        tokens.push_back({TokenType::Bslash, line, col, get_line(), "/"});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_lsqb()
{
    if (code[ip] == '[')
    {
        tokens.push_back({TokenType::Lsqb, line, col, get_line(), "["});
        return true;
    }

    return false;
}

bool Tokenizer::tokenize_rsqb()
{
    if (code[ip] == ']')
    {
        tokens.push_back({TokenType::Rsqb, line, col, get_line(), "]"});
        return true;
    }

    return false;
}
