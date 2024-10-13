#include <bits/stdc++.h>
#include <fstream>

std::string str_splice(std::string &str, std::string del = " ", bool rev = false)
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
        return str;
    }
}

std::vector<std::string> tokenizer(std::string &s, std::string del = " ")
{
    std::vector<std::string> result;

    int del_size = del.size();
    int start, end = -1 * del_size;
    do {
        start = end + del_size;
        end = s.find(del, start);

        result.push_back(s.substr(start, end - start));
    } while (end != -1);

    return result;
}

void print_tokens(std::vector<std::string> all_tokens)
{
    for(auto token: all_tokens)
    {
        std::cout << token << "\n";
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

class instruction_set
{
private:
    typedef struct instruction_t
    {
        std::string op_type;
        int op_code;

        instruction_t(int opc = -1, std::string oprnd = "x")
        {
            op_code = opc;
            op_type = oprnd;
        }
    }instruction;

    std::unordered_map<std::string, instruction> ISa;

    void parse_line(std::string ins_line)
    {
        std::vector<std::string> tokens = tokenizer(ins_line);
        
        lrstrip(tokens[2], false);
        instruction temp_ins(-1, tokens[2]);

        if(tokens[1] != "x")
        {
            temp_ins.op_code = stoi(tokens[1]);
            ISa[tokens[0]] = temp_ins;
        } 
        else
        {
            ISa[tokens[0]] = temp_ins;
        }
    }

public:
    instruction_set(bool dbg = false)
    {
        std::ifstream file("./data/Mnemonics.txt");

        // String to store each line of the file.
        bool heading = true;
        std::string line;

        if (file.is_open()) {
            // Read each line from the file and store it in the
            while (getline(file, line)) {
                if(heading){ heading = false; continue;}

                if(dbg)
                {
                    std::cout << line << "\n";
                }
                parse_line(line);
            }

            // Close the file stream once all lines have been
            // read.
            file.close();
        }
        else {
            // Print an error message to the standard error
            // stream if the file cannot be opened.
            std::cout << "Mnemonics.txt not found in data.\n";
            exit_codes(1);
        }
    }

    int get_opcode(std::string instrc_name)
    {
        return ISa[instrc_name].op_code;
    }

    ~instruction_set(){}
};

class err_msgs
{
private:
    typedef struct err_msg
    {
        bool critial;
        int line_no;
        int msg_type;
        std::string line;

        err_msg(int line_n = 0, std::string ln = " ", int mesg = -1, bool crt = false)
        {
            line_no = line_n;
            line = ln;
            msg_type = mesg;
            critial = crt;
        }
    }err_msg;

    std::vector<err_msg> prog_errors;

    std::string err_codes(int e)
    {
        switch (e)
        {
        case 0:
            return "invalid user input\n";
        default:
            return "unknown cause\n";
        }
    }

    std::string print_err(err_msg& err)
    {
        if(err.critial)
        {
            return "Assembly halted due to -> " + std::to_string(err.line_no) + " " + err.line + err_codes(err.msg_type);
        }
        else
        {
            return std::to_string(err.line_no) + " " + err.line + err_codes(err.msg_type);
        }
    }
public:
    void add_errmsg(int pc, std::string line, int mesg, bool crt)
    {
        prog_errors.emplace_back(pc, line, mesg, crt);
    }

    void dump_errmsg(std::string fp_name)
    {
        std::ofstream err_f(fp_name+".log.txt");

        for(auto err: prog_errors)
        {
            err_f << print_err(err) << '\n';
        }

        err_f << std::endl;
        err_f.close();
    }

    ~err_msgs(){}
};

class symbols_table
{
private:
    typedef struct label
    {
        int label_data;
        bool literal_type;

        label(int lbl_d = 0, bool ltrl = false) 
        {
            label_data = lbl_d;
            literal_type = ltrl;
        }
    }label_data;

    std::unordered_map<std::string, label_data> symbols;
public:
    void add_symbol(std::string label_name, int label_val, bool ltrl = false)
    {
        symbols[label_name] = label_data(label_val, ltrl);
    }

    label_data get_label(std::string label_name)
    {
        return symbols[label_name];
    }

    void dump_table()
    {
        for(auto symbol: symbols)
        {
            std::cout << symbol.first + "->(" << symbol.second.label_data << ") literal: " << symbol.second.literal_type << "\n";
        }
    }

    ~symbols_table(){}
};

class asmbler
{
private:
    typedef struct asm_code
    {
        std::string label;
        std::string ins_name;
        std::string value;

        asm_code(std::string ins_nm, std::string val, std::string lbl = "")
        {
            label = lbl;
            ins_name = ins_nm;
            value = val;
        }
    }asmbline;

    std::vector<asmbline> ProgC;
public:
    bool extra_param[4] = {false, false, false, false};

    int get_pc()
    {
        return ProgC.size();
    }

    ~asmbler(){}
};

void print_usage()
{
    std::cout << "Usage: ./asm.exe file_name.asm [optional]\n";
    std::cout << "[optional]: -t -d -s -l\n";



    std::cout << "\n----    ----    ----\nfor standalone exe: open the asm.exe\n";
}

void ASSEMBLE(asmbler &Ag_asmbler, symbols_table &symb_tab, instruction_set &ISA, err_msgs &err_tab)
{

}

int main(int argc, char* argv[])
{
    instruction_set ISA;
    std::cout << "====== ========== ======\n";
    std::cout << "Welcome to assembler asm\n";

    asmbler Ag_asmbler;
    symbols_table symb_tab;
    err_msgs err_tab;

    std::string fp_name;

    if(argc == 1)
    {
        std::cout << "enter the file name (with .asm):\n";
        std::cin >> fp_name;
    }
    else
    {
        fp_name = argv[1];
        if(argc > 2)
        {
            for(int ep = 3; ep < argc; ep++)
            {
                if(argv[ep] == "-t")
                {
                    Ag_asmbler.extra_param[0] = true;
                }
                else if(argv[ep] == "-l")
                {
                    Ag_asmbler.extra_param[1] = true;
                }
                else if(argv[ep] == "-d")
                {
                    Ag_asmbler.extra_param[2] = true;                    
                }
                else if(argv[ep] == "-s")
                {
                    Ag_asmbler.extra_param[3] = true;                    
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

    std::ifstream asm_file("./" + fp_name);
    if(asm_file.is_open())
    {
        std::cout << "File located: processing file.\n\n";

        std::string asm_line, temp_label;
        std::vector<std::string> tokens;

        // Read each line from the file and store it
        while (getline(asm_file, asm_line)) 
        {
            asm_line = str_splice(asm_line, ";");

            if(asm_line != "") 
            {
                temp_label = str_splice(asm_line, ":"); 
                if(temp_label == asm_line) {temp_label = "";}

                asm_line = str_splice(asm_line, ":", true);
                lrstrip(asm_line);

                tokens = tokenizer(asm_line, " ");
                print_tokens(tokens);
                if(tokens.size() == 2)
                {

                }
            }
            else{ continue; }
        }

        // Close the file
        asm_file.close();
    }
    else
    {
        std::cout << "No such file found in dir.\n";
        exit_codes(1);
    }

    ASSEMBLE(Ag_asmbler, symb_tab, ISA, err_tab);

    exit_codes(0);
    return 0;
}

