#include "interpreter.hpp"
#include "get_char.hpp"
#include "../error/error_types.hpp"

using std::string;
using std::cerr;

void NumInterpreter::Error(string e, Token t)
{
    cerr << "\nnum : Error while interpreting.\n\n";
    
    cerr << t.line << ':' << t.col << '\n';
    cerr << t.line << " | " << t.full_line << "\n\n";

    cerr << '\n' << e << " [ " << t.val << " ]\n";

    cerr << "\nExiting with code 1.\n";
    cerr << std::flush;

    exit(INTERPRETER_ERROR);
}

void NumInterpreter::advance()
{
    if (ip < tokens.size())
        ip++;
    if (ip >= tokens.size())
        exit(0);
}

Token NumInterpreter::next()
{
    Token val;

    if (ip < tokens.size())
        val = tokens[ip + 1];

    return val;
}

Token NumInterpreter::peek()
{
    return tokens[ip];
}

void NumInterpreter::resize_cell(int size)
{
    if (size < 0)
        size = current_cell;

    if (size >= cells.size())
        cells.resize(size + 1, 0);
}

void NumInterpreter::get_number(num_value &num, bool mnn, bool mp)
{
    if (next().type == TokenType::Lsqb)
    {
        advance();
        advance();

        if (peek().type == TokenType::Number)
            num.number = std::stoi(peek().val);
        else if (peek().type == TokenType::Dollar)
            num.number = cells[current_cell];
        else if (peek().type == TokenType::And)
            num.number = num_v;
        else 
            Error("Invalid type : `" + peek().val + "`. Expected a number.", peek());

        advance();
        if (peek().type != TokenType::Rsqb)
            Error("Invalid syntax. Did you forgot a ']'?", peek());

        if (num.number < 0 && mp) 
            Error("The number must be positive in this case.", peek());
    }

    else if (mnn) 
        Error("No number given in case a number was rquired.", peek());
    else 
        num.is_there_num = false;
}

void NumInterpreter::interpret()
{
    while (ip < tokens.size())
    {
        switch (peek().type)
        {
        case (TokenType::Number) : 
        {
            int64_t num = std::stoi(peek().val);

            if (!(num <= 9 && num >= 0)) 
                Error("Invalid number for opcode.", peek());

            switch (num)
            {
            case 1 : 
                cell_value_change(1);
                break;
            case 2:
                cell_value_change(-1);
                break;
            case 3:
                cell_value_clear();
                break;
            case 4:
                display_char();
                break;
            case 5:
                read_char();
                break;
            case 6:
                loop_start_with_counter();
                break;
            case 7:
                change_cell(1);
                break;
            case 8:
                change_cell(-1);
                break;
            case 9:
                change_cell(0, true);
                break;
            case 0 :
                clear_num(); break;
            }

            break;
        }

        case (TokenType::Lbra) :
            loop_start();
            break;
        case (TokenType::Rbra) :
            loop_end();
            break;

        case (TokenType::Add) : 
        case (TokenType::Dash) :
        case (TokenType::Star) : 
        case (TokenType::Bslash) :
            oper_num(peek().val[0]); break;

        default : Error("Invalid opcode.", peek());
        }

        advance();
    }
    
    if (!loop_stack.empty())
        Error("Unbalanced parentheses: missing ')'", peek());
}

void NumInterpreter::cell_value_change(int inc_or_dec)
{
    num_value times;
    get_number(times);

    resize_cell();

    if (inc_or_dec < 0)
        cells[current_cell] -= times.number;
    else
        cells[current_cell] += times.number;
}

void NumInterpreter::cell_value_clear()
{
    resize_cell();
    cells[current_cell] = 0;
}

void NumInterpreter::display_char()
{
    resize_cell();
    putchar(cells[current_cell]);
    fflush(stdout);
}

void NumInterpreter::read_char()
{
    resize_cell();
    cells[current_cell] = (char) get_char();
}

void NumInterpreter::loop_start(int64_t call_target)
{
    loop_t loop;
    loop.ip = ip;

    if (call_target < 0)
        loop.infinite_loop = true;
    else
    {
        loop.infinite_loop = false;
        loop.call_target = call_target;
    }

    loop_stack.push(loop);
}

void NumInterpreter::loop_end()
{
    if (loop_stack.empty())
        Error("Unbalanced parentheses: unexpected ')'", peek());

    loop_t loop = loop_stack.top();

    if (!loop.infinite_loop)
    {
        loop.called++;
        
        if (loop.called < loop.call_target)
        {
            if (cells[current_cell] >= 0)
                ip = loop.ip;
            loop_stack.pop();
            loop_stack.push(loop);
        }
        else
            loop_stack.pop();
    }
    else if (cells[current_cell] >= 0)
        ip = loop.ip;
}

void NumInterpreter::loop_start_with_counter()
{
    num_value num;

    get_number(num, true, true);
    advance();

    if (peek().type == TokenType::Lbra)
        loop_start(num.number);
    else
        Error("Expected '(' after loop counter", peek());
}

void NumInterpreter::change_cell(int iod, bool must_value)
{
    num_value n;
    get_number(n, false, true);

    if (!n.is_there_num)
    {
        if (must_value)
            Error("No number specified in a case where a value is needed.", peek());

        if (iod >= 0)
        {
            resize_cell();
            current_cell++;
        }
        else
        {
            if (current_cell > 0)
                current_cell--;
        }
    }

    else if (must_value)
    {
        resize_cell(n.number);
        current_cell = n.number;
    }

    else 
    {
        if (iod >= 0) 
        {
            resize_cell();
            current_cell += n.number;
        }

        else 
            current_cell -= n.number;
    }
}

void NumInterpreter::oper_num(char oper)
{
    num_value val;
    get_number(val);

    switch (oper)
    {
    case '+' : num_v += val.number; break;
    case '-' : num_v -= val.number; break;
    case '*' : num_v *= val.number; break;
    case '/' : num_v /= val.number; break;
    }
}

void NumInterpreter::clear_num()
{
    num_v = 0;
}
