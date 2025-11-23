#include "tokenizer/tokenizer.hpp"
#include "interpreter/interpreter.hpp"
#include "include.hpp"

#include <fstream>

using std::cerr;
using std::string;
using std::ifstream;
using std::vector;

#define _STRINGFY_HELPER(s) #s
#define STRINGFY(s) _STRINGFY_HELPER(s)

#define MajorVersion 1
#define MinorVersion 0
#define PatchVersion 0 

#define Version STRINGFY(MajorVersion) "." \
    STRINGFY(MinorVersion) "." \
    STRINGFY(PatchVersion)

// version and help functions 

void version_info()
{
    puts(string("num version " + string(Version)).c_str());
    exit(0);
}

void help()
{
    string help = R"(
usage : num [options]

default behaivour : num will take code from stdin unless the usage of flag '-f'/'--file'.
                    Eg for linux : (if you only want a single line of code) `echo "1.65*431.10*43" | num`; output : "A" (with a newline)

options :
-h, --help                  Show usage detials (now you used it btw)
-v, --version               Display num installed version 
-f, --file                  Take the code from the given filename after this flag (order matters)
-d, --debug-tokenizer       Will show all token detials for debuging purposes.
)";

    puts(help.c_str());
    exit(0);
}

// a helper for managing args 
void _args_advance(int &argc, int &ip, string &flag)
{
    if (ip + 1 < argc)
        ip++;
    else 
    {
        cerr << "num : Expected an arg after a flag. ( "  << flag << " )" << (char) 0xA;
        exit(1);
    }
}

int main(int argc, const char **argv)
{
    // args controls 
    bool show_version = false;
    bool show_help = false;

    bool debug_tokenizer = false;
    
    bool get_code_from_file = false;
    string filename = "";

    // the code 
    string code;

    // managing args 
    int arg_ip = 1;
    while (arg_ip < argc)
    {
        string flag = argv[arg_ip];

        if (flag.front() == '-')
        {
            
            // -h || --help for help 
            if (flag == "-h" || flag == "--help") 
                show_help = true;
            
            // -v || --version for showing the version
            else if (flag == "-v" || flag == "--version")
                show_version = true;
            
            // -d || --debug-tokenizer for showing all token detials 
            else if (flag == "-d" || flag == "--debug-tokenizer")
                debug_tokenizer = true;
            
            // -f || --file for getting the code from file insted of stdin
            else if (flag == "-f" || flag == "--file")
            {
                _args_advance(argc, arg_ip, flag);

                get_code_from_file = true;
                filename = argv[arg_ip];
            }

            else 
            {
                cerr << "num : Invalid flag : " << flag << (char) 0xA;
                return 1;
            }
        }

        arg_ip++;
    }

    // operations according to args 
    if (show_version) version_info();
    if (show_help) help();

    if (get_code_from_file)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "num : Cannot open file " << filename << (char) 0xA;
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        code = buffer.str();
    }

    else 
    {
        // taking code from stdin 
        string line;
        while (std::getline(std::cin, line))
            code += line + (char) 0xA;
    }
        
    code.push_back(0xA);
    code.push_back('3');

    Tokenizer tokenizer(code, debug_tokenizer);
    vector<Token> tokens = tokenizer.tokenize();

    NumInterpreter interpreter(tokens);
    interpreter.interpret();

    return 0;
}
