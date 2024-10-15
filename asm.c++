#include <bits/stdc++.h>

#define blank_string "~"
#define blank_int -32

#define opc_len 2
#define adr_len 6
#define full_len 8


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
        std::vector<std::string> emp_chars = {" ", "\t", ","};

        auto v = std::string(1 ,str.front()); 
        if(lstrip)
        {
            for(auto emp_char: emp_chars)
            {
                while(v == emp_char)
                {
                    str.erase(str.begin());
                    v = std::string(1 ,str.front());
                }
            }
        }
        v = std::string(1 , str.back()); 
        if(rstrip)
        {
            for(auto emp_char: emp_chars)
            {
                while(v == emp_char)
                {
                    str.erase(str.end() - 1 );
                    v = std::string(1 , str.back());
                }
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

bool isValidString(const std::string& str) {
    if (str.empty()) {
        return false;  // Return false if string is empty
    }

    // Check if the first character is an alphabet
    if (!std::isalpha(str[0])) {
        return false;
    }

    bool foundDigit = false;

    // Loop through the string starting from the second character
    for (size_t i = 1; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            foundDigit = true;  // Mark that a digit was found
        } else if (!std::isalpha(str[i])) {
            return false;  // If any special character is found, return false
        }

        // If we find a digit in between alphabets, return false
        if (foundDigit && std::isalpha(str[i])) {
            return false;
        }
    }

    return true;
}

/////// NEEDS works
std::string hex_formatter(const std::string &str, int dig = -1)
{
    if(dig != -1)
    {
        return str;
    }

    return str;
}

int isproper_num(std::string& str_val)
{
    try
    {
        int val = stoi(str_val);
        return val;
    }
    catch(const std::exception& e)
    {
        return 0;
    }
}

class error_msgs
{
private:
    typedef struct error_msg
    {
        bool critial;
        int line_no;
        int msg_type;
        std::string line;

        error_msg(int ln_no = 0, std::string ln = blank_string, int msg_t = blank_int, bool crt = false)
        {
            line_no = ln_no;
            line = ln;
            msg_type = msg_t;
            critial = crt;
        }
    }error_msg;

    std::vector<error_msg> prog_errors;
    bool asm_failed = false;

    std::string err_codes(int e)
    {
        switch (e)
        {
        case 0:
            return "\n:\tinvalid label name\n";
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
        case 8:
            return "\n:\tlabel used before declare\n";
        case 9:
            return "\n:\tunused label\n";
        case 10:
            return "\n:\tinvalid use of SET/data\n";
        default:
            return "\n::unknown cause\n";
        }
    }

    std::string print_err(error_msg &err)
    {
        if(err.critial)
        {
            return "Assembly halted due to -> " + std::to_string(err.line_no) + " [" + err.line  + "]" + err_codes(err.msg_type);
        }
        else
        {
            return "Warning " + std::to_string(err.line_no) + " [" + err.line  + "]" + err_codes(err.msg_type);
        }
    }

public:
    void add_errmsg(int pc, std::string line, int msg, bool crt)
    {
        prog_errors.emplace_back(pc, line, msg, crt);
        if(!asm_failed)
        {
            asm_failed = crt;
        }
    }

    bool check_asm_failed()
    {
        return asm_failed;
    }

    void dump_errmsg(std::string &fp_name, bool lg_file = false)
    {
        std::ofstream error_fp;
        if(lg_file)
        {
            error_fp.open(fp_name+ "-err.log");
        }

        if(error_fp.is_open())
        {
            error_fp << "Error logs: \n";
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
                error_fp << print_err(err) << '\n';
            }
            std::cout << print_err(err) << '\n';
        }

        if(prog_errors.size() == 0)
        {
            std::cout << "clean" << '\n';
            if(lg_file)
            {
                error_fp << "Compiled with 0 errors." << '\n';
            }
        }

        if(lg_file)
        {
            error_fp.close();
        }
    }

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

public:
    std::unordered_map<std::string, label_data> symbol_t;
    std::unordered_map<std::string, std::string> all_label_refs;

    bool add_symbol(std::string &label_name, std::string label_val, bool ltrl = false)
    {
        if(symbol_t.find(label_name) == symbol_t.end())
        {
            symbol_t[label_name] = label_data(label_val, ltrl);
            return true;
        }

        return false;
    }

    bool check_undeclared(std::string label_name)
    {
        if(symbol_t.find(label_name) == symbol_t.end())
        {
            return true;
        }
        return false;
    }

    bool check_forw_refs(std::string &label_name)
    {
        if(all_label_refs.find(label_name) == all_label_refs.end())
        {
            return false;
        }

        symbol_t[label_name].used_lbl = true;
        return true;
    }

    std::string prty_print(bool val)
    {
        if(val)
        {
            return " stores constant";
        }
        return " stores address";
    }

    /*void dump_table(std::string &fp_name, bool lg_file = false)
    {
        std::ofstream fp_file;
        if(lg_file)
        {
            fp_file.open(fp_name);
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
        std::cout << "\n\n";
    }*/
};

class asmbler
{
public:
    typedef struct machine_code
    {
        std::string prog_line;
        std::string ins_line;

        machine_code(std::string prg_ln, std::string ins_ln)
        {
            prog_line = prg_ln;
            ins_line = ins_ln;
        }
    }machline;

    typedef struct asm_code
    {
        std::string label;
        std::string ins_name;
        std::string value;

        asm_code(std::string ins_nm = blank_string, std::string val = blank_string, std::string lbl = "")
        {
            label = lbl;
            ins_name = ins_nm;
            value = val;
        }
    }asmbline;

private:
    typedef struct instruction_t
    {
        int op_code;
        bool needs_val;

        instruction_t(int opc = blank_int, bool nds_val = false)
        {
            op_code = opc;
            needs_val = nds_val;
        }
    }instruction;

    std::vector<asmbline> ProgC;
    std::unordered_map<std::string, instruction> IS;

    std::string prty_print(std::string &val)
    {
        return (val == blank_string)? "" : val;
    }

    void parse_line(std::string ins_line)
    {
        std::vector<std::string> tokens = tokenizer(ins_line);

        instruction temp_ins;
        if(tokens[1] != blank_string)
        {
            temp_ins.op_code = stoi(tokens[1]);
        }
        else
        {
            temp_ins.op_code = 0;
        }

        if(tokens[2] != blank_string)
        {
            temp_ins.needs_val = true;
        } 

        IS[tokens[0]] = temp_ins;
    }

    void dump_instr_set()
    {
        for(auto is: IS)
        {
            int num_inputs = is.second.needs_val + 1;
            std::cout << is.first << "-> (" + std::to_string(is.second.op_code) + "," + std::to_string(num_inputs) + ")\n";
        }
    }

public:
    asmbler(bool dbg = false)
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

            if(dbg){ dump_instr_set(); }
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

    // params for: -t -l -d -s
    bool extra_param[4] = {false, false, false, false};

    int get_pc()
    {
        return ProgC.size();
    }

    bool invalid_ins(std::string &ins_nm)
    {
        if(IS.find(ins_nm) == IS.end())
        {
            return true;
        }
        return false;
    }

    bool ins_needs_val(std::string &ins_nm)
    {
        return IS[ins_nm].needs_val;
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
        std::cout << "Processed internal Code: \n";
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

void ASSEMBLE(asmbler &Ag_asmbler, symbols_table &symb_tab, error_msgs &err_tab, std::string &fp_name0)
{
    std::string file_fp = "./" + fp_name0.substr(0, fp_name0.size() - 5);


    Ag_asmbler.dump_code();
    err_tab.dump_errmsg(file_fp);
}

int main(int argc, char* argv[])
{
    asmbler Ag_asmbler;
    error_msgs error_table;
    symbols_table symb_table;

    std::cout << "====== ========== ======\n";
    std::cout << "Welcome to assembler asm\n";

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

        std::string asm_line, store_line, labl;
        std::vector<std::string> tokens;

        // Read each line from the file and store it
        while (getline(asm_file, store_line)) 
        {
            store_line = str_splice(store_line, ";");
            lrstrip(store_line);

            if(store_line != "") 
            {
                labl = str_splice(store_line, ":", false, "");
                asm_line = str_splice(store_line, ":", true);

                lrstrip(labl);
                tokens = tokenizer(asm_line);
                int tk_size = tokens.size();

                asmbler::asmbline temp_asm;

                // All label processings
                if(labl != "")
                {
                    temp_asm.label = labl;
                    if(isValidString(labl))
                    {
                        if(symb_table.add_symbol(labl, std::to_string(Ag_asmbler.get_pc())))
                        {
                            if(symb_table.check_forw_refs(labl))
                            {
                                error_table.add_errmsg(Ag_asmbler.get_pc(), symb_table.all_label_refs[labl], 8, false);
                            }
                            else
                            {
                                symb_table.all_label_refs[labl] = store_line;
                            }

                            if(tk_size == 2)
                            {
                                if(labl == tokens[1])
                                {
                                    error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 4, true);                                   
                                }
                                else if(tokens[0] == "SET")
                                {
                                    symb_table.symbol_t[labl].literal_type= true;
                                }
                            }
                        }
                        else
                        {
                            error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 2, true);
                        }
                    }
                    else
                    {
                        error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 0, true);
                    }
                }

                if(tk_size >= 1)
                {
                    temp_asm.ins_name = tokens[0];
                    if(tk_size >= 2)
                    {
                        temp_asm.value = tokens[1];
                    }

                    if(Ag_asmbler.invalid_ins(tokens[0]))
                    {
                        error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 5, true);
                    }
                    else if(Ag_asmbler.ins_needs_val(tokens[0]) && tk_size == 1)
                    {
                        error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 3, true);
                    }
                    else if(Ag_asmbler.ins_needs_val(tokens[0]) && tk_size >= 2)
                    {
                        // Check for set / data
                        if((tokens[0] == "SET" || tokens[0] == "data") && isproper_num(tokens[1]))
                        {
                            symb_table.symbol_t[labl] = tokens[1];
                        }
                        else if (!(tokens[0] == "SET" || tokens[0] == "data"))
                        {
                            if(!isproper_num(tokens[1]) && !symb_table.check_forw_refs(tokens[1]))
                            {
                                if(isValidString(tokens[1]))
                                {
                                    symb_table.all_label_refs[tokens[1]] = store_line;
                                }
                                else
                                {
                                    error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 7, true);
                                }
                            }
                        }
                        else
                        {
                            error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 10, true);
                        }

                        if(tk_size > 2)
                        {
                            error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 1, false);
                        }
                    }
                    else if(!Ag_asmbler.ins_needs_val(tokens[0]) && tk_size >= 2)
                    {
                        error_table.add_errmsg(Ag_asmbler.get_pc(), store_line, 1, false);
                    }
                }

                Ag_asmbler.add_code(temp_asm);
            }
            else{ continue; }
        }

        // All unused/ not found labels
        for(auto sym_data: symb_table.all_label_refs)
        {
            if(symb_table.check_undeclared(sym_data.first))
            {
                error_table.add_errmsg(-1, sym_data.second, 6, true);
            }
            else if(!symb_table.symbol_t[sym_data.first].used_lbl)
            {
                try
                {
                    error_table.add_errmsg(stoi(symb_table.symbol_t[sym_data.first].label_data), sym_data.second, 9, false);
                }
                catch(const std::exception& e)
                {
                    error_table.add_errmsg(-1, sym_data.second, 9, false);
                }
            }
        }

        symb_table.all_label_refs.clear();
        asm_file.close();
    }
    else
    {
        std::cout << "No such file found in dir.\n";
        exit_codes(1);
    }

    ASSEMBLE(Ag_asmbler, symb_table, error_table, fp_name);

    exit_codes(0);
    return 0;
}

