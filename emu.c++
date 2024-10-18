#include <bits/stdc++.h>

#define blank_string "~"
#define blank_int -32

std::string str_splice(std::string &str, std::string del = " ", bool rev = false, std::string deflt = blank_string)
{
    int pos = str.find(del);
    int str_size = str.size();

    // Check if the delimiter was found
    if (pos != std::string::npos) {

        if(rev)
        {
            // Get the substring from the pos to end
            return str.substr(pos + 1, str_size - 1);
        }
        else
        {
            // Get the substring from the beginning to the position of the delimiter
            return str.substr(0, pos);
        }

    } else {
        if(deflt == blank_string)
        {
            return str;
        }
        else
        {
            return deflt;
        }
    }
}

void lrstrip(std::string &str, bool lstrip = true, bool rstrip = true)
{
    if  (str.length() != 0)
    {
        std::string w = " ";
        std::string n = "\n" ;
        std::string r = "\r" ;
        std::string t = "\t" ;

        auto v = std::string(1 ,str.front()); 
        if(lstrip)
        {
            while((v == w) || (v==t) || (v==r) || (v==n))
            {
                str.erase(str.begin());
                v = std::string(1 ,str.front());
            }
        }
        v = std::string(1 , str.back()); 
        if(rstrip)
        {
            while((v ==w) || (v==t) || (v==r) || (v==n))
            {
                str.erase(str.end() - 1 );
                v = std::string(1 , str.back());
            }
        }
    }
}

void clean_targetstr(std::vector<std::string> &str, std::string deflt = "")
{
    // Step 1: Use std::remove to move all empty strings to the back
    auto new_end = std::remove(str.begin(), str.end(), deflt);

    // Step 2: Use pop_back() to remove the extra empty strings at the back
    while (str.end() != new_end) {
        str.pop_back();
    }
}

std::vector<std::string> tokenizer(std::string &str, std::string del = " ")
{
    std::vector<std::string> result;

    int del_size = del.size();
    int start, end = -1 * del_size;
    do {
        start = end + del_size;
        end = str.find(del, start);

        std::string temp_str = str.substr(start, end - start);
        lrstrip(temp_str);

        result.push_back(temp_str);
    } while (end != -1);

    clean_targetstr(result);
    return result;
}

void print_tokens(std::vector<std::string> all_tokens)
{
    for(auto token: all_tokens)
    {
        std::cout << token << "\n";
    }
}

void exit_codes(int e)
{
    std::cout << "\n\n";
    if(e)
    {
        std::cout << "Error exit.\n";
    }
    else
    {
        std::cout << "Program terminated successfully.\n";
    }
    std::cout << "==== Press enter to close ====\n";
    
    std::cin.get();
    exit(0);
}

class emultor
{
private:


    void ins_table(int opc, int &pc, int &a, int &b, int &sp)
    {


    }

public:
    bool extra_param[4] = {false, false, false, false};
};

void print_usage()
{
std::cout << "Usage: ./emu.exe file_name.asm [optional]\n";
    std::cout << "[optional]: -t -d -s -l\n\n";

    std::cout << "(trace): -t\n";
    std::cout << "(dump): -d\n";
    std::cout << "(symbol): -s\n";
    std::cout << "(err logfile): -l\n";

    std::cout << "\n----    ----    ----\nfor standalone exe: open the emu.exe\n----    ----    ----\n\n";
}

void EMULATE()
{

}

int main(int argc, char* argv[])
{
    //emultor Ag_emulator;
    std::cout << "====== ========== ======\n";
    std::cout << "Welcome to emulator emu\n";

    int A, B, PC, SP;
    A = B  = PC = SP = 0;

    std::string fp_name;

    int a = std::stoi("-0xFF", (std::size_t *)0 ,16);
    std::cout << a <<"\n";

    int b = std::stoi("+17", (std::size_t *)0 ,8);
    std::cout << b <<"\n";

    int c = std::stoi("-011", (std::size_t *)0 ,2);
    std::cout << c <<"\n";

    std::string v = "1234";
    std::cout << v.substr(1, v.size() - 1) << '\n';

    /*if(argc == 1)
    {
        std::cout << "enter the file name (with .o):\n";
        std::cin >> fp_name;
    }
    else
    {
        fp_name = argv[1];
        if(argc > 2)
        {
            for(int ep = 2; ep < argc; ep++)
            {
                std::string comd_arg = argv[ep];
                if(comd_arg == "-t")
                {
                    Ag_emulator.extra_param[0] = true;
                }
                else if(comd_arg == "-l")
                {
                    Ag_emulator.extra_param[1] = true;
                }
                else if(comd_arg == "-d")
                {
                    Ag_emulator.extra_param[2] = true;                    
                }
                else if(comd_arg == "-s")
                {
                    Ag_emulator.extra_param[3] = true;                    
                }
                else
                {
                    print_usage();
                    break;
                }
            }
        }
    }

    std::cout << "\nSearching: " + fp_name << "\n";

    std::ifstream emu_file("./" + fp_name);
    if(emu_file.is_open())
    {
        std::cout << "File located: processing file.\n\n";

        std::string store_line, pC, val, OPc;

        // Read each line from the file and store it
        while (getline(emu_file, store_line)) 
        {
            lrstrip(store_line);
            //pC = str_splice(store_line, " ");

            //val = str_splice(store_line, " ", true);
        }

        // Close the file
        emu_file.close();
    }
    else
    {
        std::cout << "No such file found in dir.\n";
        exit_codes(1);
    }*/

    //EMULATE(Ag_emulator, symb_tab, ISA, err_tab, fp_name);

    exit_codes(0);
    return 0;
}

