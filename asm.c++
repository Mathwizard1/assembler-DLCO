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

void clean_emptystr(std::vector<std::string> &strng)
{
    // Step 1: Use std::remove to move all empty strings to the back
    auto new_end = std::remove(strng.begin(), strng.end(), "");

    // Step 2: Use pop_back() to remove the extra empty strings at the back
    while (strng.end() != new_end) {
        strng.pop_back();
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

        std::string temp_str = s.substr(start, end - start);
        lrstrip(temp_str);

        result.push_back(temp_str);
    } while (end != -1);

    clean_emptystr(result);
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

class instruction_set
{
public:
    typedef struct instruction_t
    {
        std::string op_type;
        int op_code;

        instruction_t(int opc = blank_int, std::string oprnd = blank_string)
        {
            op_code = opc;
            op_type = oprnd;
        }
    }instruction;

private:
    std::unordered_map<std::string, instruction> ISa;

    void parse_line(std::string ins_line)
    {
        std::vector<std::string> tokens = tokenizer(ins_line);

        instruction temp_ins(-1, tokens[2]);
        if(tokens[1] != blank_string)
        {
            temp_ins.op_code = stoi(tokens[1]);
            ISa[tokens[0]] = temp_ins;
        } 
        else
        {
            ISa[tokens[0]] = temp_ins;
        }
    }

    void dump_isa()
    {
        for(auto isa: ISa)
        {
            std::cout << isa.first << "->(" + std::to_string(isa.second.op_code) + "," + isa.second.op_type + ")\n";
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

            if(dbg){ dump_isa(); }
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

    instruction get_instrcution(std::string &instrc_name)
    {
        if(ISa.find(instrc_name) != ISa.end())
        {
            return ISa[instrc_name];
        }
        else
        {
            return instruction();
        }
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

        err_msg(int line_n = 0, std::string ln = blank_string, int mesg = blank_int, bool crt = false)
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
            return "\n:\tinvalid user input\n";
        case 1:
            return "\n:\textra arguments passed\n";
        case 2:
            return "\n:\tduplicate labels found\n";
        case 3:
            return "\n:\tInsufficient arguments\n";
        case 4:
            return "\n:\tinfinite loop detected\n";
        case 5:
            return "\n:\tinvalid instruction\n";
        case 6:
            return "\n:\tlabel not found\n";
        case 7:
            return "\n:\tnot a valid number\n";
        default:
            return "\n::unknown cause\n";
        }
    }

    std::string print_err(err_msg &err)
    {
        if(err.critial)
        {
            return "Assembly halted due to -> [" + std::to_string(err.line_no) + " " + err.line  + "]" + err_codes(err.msg_type);
        }
        else
        {
            return "Warning [" + std::to_string(err.line_no) + " " + err.line  + "]" + err_codes(err.msg_type);
        }
    }

public:
    void add_errmsg(int pc, std::string line, int mesg, bool crt)
    {
        prog_errors.emplace_back(pc, line, mesg, crt);
    }

    void dump_errmsg(std::string &fp_name, bool lg_file = false)
    {
        std::ofstream err_f;
        if(lg_file)
        {
            err_f.open("./" + fp_name+ "-err.log");
        }

        if(err_f.is_open())
        {
            err_f << "Error logs: \n";
        }
        else if(lg_file)
        {
            std::cout << "Unknown cause of Exit.\n";
            exit_codes(1);
        }

        std::cout << "\nError logs: \n";
        for(auto err: prog_errors)
        {
            if(lg_file)
            {
                err_f << print_err(err) << '\n';
            }
            std::cout << print_err(err) << '\n';
        }

        if(prog_errors.size() == 0)
        {
            std::cout << "clean" << '\n';
        }

        if(lg_file)
        {
            err_f.close();
        }
    }

    ~err_msgs(){}
};

class symbols_table
{
private:
    typedef struct labl
    {
        std::string label_data;
        bool literal_type;
        bool used_lbl;

        labl(std::string lbl_d = blank_string, bool ltrl = false, bool usd_lbl = false) 
        {
            label_data = lbl_d;
            literal_type = ltrl;
            used_lbl = usd_lbl;
        }
    }label_data;

    std::unordered_map<std::string, label_data> symbol_t;

public:
    bool add_symbol(std::string label_name, std::string label_val, bool ltrl)
    {
        if(symbol_t.find(label_name) == symbol_t.end())
        {
            symbol_t[label_name] = label_data(label_val, ltrl);
            return true;
        }

        return false;
    }

    void set_label(std::string label_name, std::string label_val, bool ltrl)
    {
        symbol_t[label_name].label_data = label_val;
        symbol_t[label_name].literal_type = ltrl;
    }

    std::string prty_print(bool val)
    {
        if(val)
        {
            return " stores constant";
        }
        return " stores address";
    }

    label_data get_label(std::string label_name)
    {
        if(symbol_t.find(label_name) == symbol_t.end())
        {
            symbol_t[label_name].used_lbl = true;
            return symbol_t[label_name];
        }

        return label_data(blank_string);
    }

    void dump_table(std::string &fp_name, bool lg_file = false)
    {
        std::ofstream fp_file;
        if(lg_file)
        {
            fp_file.open("./" + fp_name);
        }

        std::cout << "Symbol table: \n";
        if(fp_file.is_open())
        {
            fp_file << "Symbol table: \n";
        }
        else if(lg_file)
        {
            std::cout << "Unknown cause of Exit.\n";
            exit_codes(1);
        }

        for(auto symbl: symbol_t)
        {
            std::cout << symbl.first + ": (" << symbl.second.label_data << ")" + prty_print(symbl.second.literal_type) << "\n";
            if(lg_file)
            {
                fp_file << symbl.first + ": (" << symbl.second.label_data << ")" + prty_print(symbl.second.literal_type) << "\n";
            }

        }

        if(lg_file)
        {
            fp_file.close();
        }
    }

    ~symbols_table(){}
};

class asmbler
{
public:
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

private:
    std::vector<asmbline> ProgC;

    std::string prty_print(std::string val)
    {
        return (val == blank_string)? "" : val;
    }

public:
    // params for: -t -l -d -s
    bool extra_param[4] = {false, false, false, false};

    int get_pc()
    {
        return ProgC.size();
    }

    void add_code(asmbline &asmdata)
    {
        ProgC.push_back(asmdata);
    }

    asmbline get_code(int i)
    {
        return ProgC[i];
    }

    std::string get_line(int i)
    {
        if(ProgC[i].label != "")
        {
            return ProgC[i].label + ": " + prty_print(ProgC[i].ins_name) + " " + prty_print(ProgC[i].value);
        }
        
        return prty_print(ProgC[i].ins_name) + " " + prty_print(ProgC[i].value);
    }

    void dump_code()
    {
        int count = 0;
        for(auto asm_line: ProgC)
        {
            if(asm_line.label != "")
            {
                std::cout << std::to_string(count) + "-> " << asm_line.label + ": " + prty_print(asm_line.ins_name) + " " + prty_print(asm_line.value) <<'\n';
            }
            else
            {
                std::cout << std::to_string(count) + "-> " << prty_print(asm_line.ins_name) + " " + prty_print(asm_line.value) <<'\n';                
            }

            count++;
        }
    }

    ~asmbler(){}
};

void print_usage()
{
    std::cout << "Usage: ./asm.exe file_name.asm [optional]\n";
    std::cout << "[optional]: -t -d -s -l\n\n";

    std::cout << "(trace): -t\n";
    std::cout << "(dump): -d\n";
    std::cout << "(symbol): -s\n";
    std::cout << "(err logfile): -l\n";

    std::cout << "\n----    ----    ----\nfor standalone exe: open the asm.exe\n----    ----    ----\n\n";
}

void ASSEMBLE(asmbler &Ag_asmbler, symbols_table &symb_tab, instruction_set &ISA, err_msgs &err_tab, std::string &fp_name0)
{
    bool flag = false;
    std::string fp_name = fp_name0.substr(0, fp_name0.size() - 5);

    for(int index = 0; index < Ag_asmbler.get_pc(); index++)
    {
        asmbler::asmbline curr_line = Ag_asmbler.get_code(index);
        instruction_set::instruction curr_ins = ISA.get_instrcution(curr_line.ins_name);

        // Needs work

        if(curr_line.ins_name != blank_string && curr_ins.op_code == blank_int)
        {
            err_tab.add_errmsg(index, Ag_asmbler.get_line(index) , 5, true);
            flag = true;
        }
        else if(curr_ins.op_type == blank_string && curr_line.value != blank_string)
        {
            err_tab.add_errmsg(index, Ag_asmbler.get_line(index) , 1, false);
        }
        else if(curr_ins.op_type != blank_string && curr_line.value == blank_string)
        {
            err_tab.add_errmsg(index, Ag_asmbler.get_line(index) , 3, true);
            flag = true;
        }
        else if(curr_ins.op_type != blank_string)
        {

        }


        if(flag)
        {
            break;
        }
    }

    if(Ag_asmbler.extra_param[3])
    {
        symb_tab.dump_table(fp_name);
    }

    if(Ag_asmbler.extra_param[0])
    {
        Ag_asmbler.dump_code();
    }

    err_tab.dump_errmsg(fp_name, Ag_asmbler.extra_param[1]);
    if(flag)
    {
        exit_codes(1);
    }

    if(Ag_asmbler.extra_param[2])
    {
        fp_name = fp_name0.substr(0, fp_name0.size() - 5) + "-data.txt";  
        symb_tab.dump_table(fp_name, Ag_asmbler.extra_param[2]);

        std::ofstream file_data;
        file_data.open("./" + fp_name, std::ios::app);

        if(file_data.is_open())
        {
            file_data << "\nAssembly code: " << '\n';

            int pc_num = Ag_asmbler.get_pc();
            for(int id = 0; id < pc_num; id++)
            {
                file_data << Ag_asmbler.get_line(id) << '\n';
            }

            file_data.close();
        }
        else
        {
            std::cout << "Unknown cause of Exit.\n";
            exit_codes(1);
        }
    }
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
            for(int ep = 2; ep < argc; ep++)
            {
                std::string comd_arg = argv[ep];
                if(comd_arg == "-t")
                {
                    Ag_asmbler.extra_param[0] = true;
                }
                else if(comd_arg == "-l")
                {
                    Ag_asmbler.extra_param[1] = true;
                }
                else if(comd_arg == "-d")
                {
                    Ag_asmbler.extra_param[2] = true;                    
                }
                else if(comd_arg == "-s")
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

        std::string asm_line, store_line;
        std::vector<std::string> tokens;

        // Read each line from the file and store it
        while (getline(asm_file, store_line)) 
        {
            store_line = str_splice(store_line, ";");
            lrstrip(store_line);

            if(store_line != "") 
            {
                asm_line = str_splice(store_line, ":", true);
                lrstrip(asm_line);

                tokens = tokenizer(asm_line, " ");
                int tk_size = tokens.size();

                asmbler::asmbline temp_asm(blank_string, blank_string, str_splice(store_line, ":", false, ""));

                if(tk_size == 2)
                {
                    temp_asm.ins_name = tokens[0];
                    temp_asm.value = tokens[1];
                }
                else if(tk_size == 1)
                {
                    temp_asm.ins_name = tokens[0];
                }
                else if(tk_size > 2)
                {
                    err_tab.add_errmsg(Ag_asmbler.get_pc(), store_line, 1, false);
                }

                lrstrip(temp_asm.label);
            
                if(temp_asm.label != "")
                {
                    if(symb_tab.add_symbol(temp_asm.label, std::to_string(Ag_asmbler.get_pc()), false))
                    {
                        if(temp_asm.ins_name == "SET")
                        {
                            if(tk_size == 2)
                            {
                                symb_tab.set_label(temp_asm.label, tokens[1], true);
                            }
                        }
                        // Needs more information
                        else if(temp_asm.ins_name == "data")
                        {

                        }
                    }
                    else
                    {
                        err_tab.add_errmsg(Ag_asmbler.get_pc(), store_line, 2, true);
                    }
                }
                
                Ag_asmbler.add_code(temp_asm);
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

    ASSEMBLE(Ag_asmbler, symb_tab, ISA, err_tab, fp_name);

    exit_codes(0);
    return 0;
}

