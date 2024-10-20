#include <bits/stdc++.h>

#define blank_string "~"
#define blank_int -32

#define opc_len 2
#define oprnd_len 6
#define full_len 8

#define max_mem 1000

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

void readHex(std::ifstream& file, std::vector<std::string> &hex_chunk) {
    if (!file.is_open()) {
        std::cerr << "Error: File is not open!" << std::endl;
        return;
    }

    char buffer[33];  // Buffer to hold data

    while (file.read(buffer, sizeof(buffer) - 1)) {
        // Process the file
        buffer[32] = '\0';

        // Convert binary string to a uint32_t (unsigned 32-bit integer)
        uint32_t value = std::bitset<32>(buffer).to_ulong();

        // Convert the uint32_t value to an 8-digit hexadecimal string
        std::stringstream hexStream;
        hexStream << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << value;

        hex_chunk.push_back(hexStream.str());
    }
}


class emultor
{
public:
    typedef struct instruction_t
    {
        bool oprnd_ofs = false;
        std::string ins_name;
        bool needs_val;

        instruction_t(std::string ins_nm = blank_string, bool nds_val = false)
        {
            ins_name = ins_nm;
            needs_val = nds_val;
        }
    }instruction;

private:
    std::unordered_map<int, instruction> IS;

    void parse_line(std::string ins_line)
    {
        std::vector<std::string> tokens = tokenizer(ins_line);
        instruction temp_ins;
        
        int opcode = std::stoi(tokens[1]);
        if(opcode >= 0)
        {
            temp_ins.ins_name = tokens[0];

            if(tokens[2] != blank_string)
            {
                if(tokens[2] == "offset")
                {
                    temp_ins.oprnd_ofs = true;
                }
                temp_ins.needs_val = true;
            } 

            IS[opcode] = temp_ins;
        }
    }

    void dump_instr_set()
    {
        for(auto is: IS)
        {
            int num_inputs = is.second.needs_val + 1;
            std::cout << std::to_string(is.first) + "-> (" + is.second.ins_name + "," + std::to_string(num_inputs) + ")\n";
        }
    }

public:
    int A, B, PC, SP;
    int count_ins;

    int mem[max_mem];
    std::vector<std::string> all_ins;

    emultor(bool dbg = false)
    {
        A = B  = PC = SP = 0;
        count_ins = 0;

        for (int m = 0; m <  max_mem; m++)
        {
            mem[m] = 0;
        }

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

    int ins_table(int opc, int val)
    {
        switch (opc)
        {
        case 0:
            B = A;
            A = val;
            break;
        case 1:
            A = A + val;
            break;
        case 2:
            B = A;
            A = mem[SP + val];
            break;
        case 3:
            mem[SP + val] = A;
            A = B;
            break;
        case 4:
            A = mem[A + val];
            break;
        case 5:
            mem[A + val] = B;
            break;
        case 6:
            A = B + A;
            break;
        case 7:
            A = B - A;
            break;
        case 8:
            A = B << A; 
            break;
        case 9:
            A = B >> A; 
            break;
        case 10:
            SP = SP + val;
            break;
        case 11:
            SP = A;
            A = B;
            break;
        case 12:
            B = A;
            A = SP;
            break;
        case 13:
            B = A;
            A = PC;
            PC = PC + val;
            break;
        case 14:
            PC = A;
            A = B;
            break;
        case 15:
            if(A == 0)
            {
                PC = PC + val;
            }
            break;
        case 16:
            if(A < 0)
            {
                PC = PC + val;
            }
            break;
        case 17:
            PC = PC + val;
            break;
        case 18:
            return 1;
        default:
            count_ins--;
            break;
        }

        return 0;
    }

    std::string get_op_name(int opc)
    {
        if(IS.find(opc) != IS.end())
        {
            return IS[opc].ins_name;
        }

        return " ";
    }

    std::string dump_val(int opc)
    {
        return "PC = " + std::to_string(PC) + " SP = " + std::to_string(SP) + " A = " + std::to_string(A) + " B = " + std::to_string(B);
    }

    // extra param for -t -l -pd -dp
    bool extra_param[4] = {false, false, false, false};
};

void print_usage()
{
std::cout << "Usage: ./emu.exe file_name.o [optional]\n";
    std::cout << "[optional]: -t -l -v -d\n";

    std::cout << "(trace): -t\n";
    std::cout << "(instruction file): -l\n";
    std::cout << "(variable dump): -v\n";
    std::cout << "(dump memory): -d\n";

    std::cout << "\n----    ----    ----\nfor standalone exe: open the emu.exe\n----    ----    ----\n\n";
}

void EMULATE(emultor &Ag_emulator, std::string file_fp)
{
    int ins_size = Ag_emulator.all_ins.size();

    std::string file_nm = "./" + file_fp.substr(0, file_fp.size() - 2) + ".log";
    std::cout << file_nm << "\n";

    std::ofstream fp_if;

    if(Ag_emulator.extra_param[1] || Ag_emulator.extra_param[3])
    {
        fp_if.open(file_nm);
        if(!fp_if.is_open())
        {
            std::cout << "Unknown cause of error\n";
            exit_codes(1);
        }
    }

    for(Ag_emulator.PC = 0; Ag_emulator.PC < ins_size; Ag_emulator.PC++)
    {
        std::string ins_ln = Ag_emulator.all_ins[Ag_emulator.PC];

        std::cout << ins_ln << '\n';
        int opc = std::stoi(ins_ln.substr(oprnd_len, opc_len), (std::size_t *)0, 16);
        int val = std::stoi(ins_ln.substr(0, full_len - opc_len), (std::size_t *)0, 16);


        if(Ag_emulator.extra_param[0])
        {
            std::cout << ins_ln + " -> " + Ag_emulator.get_op_name(opc) + " " + std::to_string(val) << '\n';
        }

        Ag_emulator.ins_table(opc, val);

        if(Ag_emulator.extra_param[2])
        {
            std::cout << Ag_emulator.dump_val(opc) + "\n";
        }

        if(Ag_emulator.extra_param[1])
        {
            fp_if << ins_ln + " -> " + Ag_emulator.get_op_name(opc) + " " + std::to_string(val) << '\n';
            fp_if << Ag_emulator.dump_val(opc) + "\n";            
        }
    }

    if(Ag_emulator.extra_param[1] || Ag_emulator.extra_param[3])
    {
        fp_if << "\n\nMemory dump:\n";
        for (int m = 0; m <  max_mem; m++)
        {
            fp_if << Ag_emulator.mem[m] << " ";
            if(m > 0 && m % 8 == 0)
            {
                fp_if << "\n";
            }
        }
    }

    fp_if.close();
    exit_codes(0);
}

int main(int argc, char* argv[])
{
    emultor Ag_emulator;
    std::cout << "====== ========== ======\n";
    std::cout << "Welcome to emulator emu\n";

    std::string fp_name;

    if(argc == 1)
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
                else if(comd_arg == "-v")
                {
                    Ag_emulator.extra_param[2] = true;                    
                }
                else if(comd_arg == "-d")
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
    std::string fp_copy = fp_name;

    std::ifstream emu_file("./" + fp_name, std::ios::binary);
    if(emu_file.is_open())
    {
        std::cout << "File located: processing file.\n\n";
        readHex(emu_file, Ag_emulator.all_ins);

        // Close the file
        emu_file.close();
    }
    else
    {
        std::cout << "No such file found in dir.\n";
        exit_codes(1);
    }

    EMULATE(Ag_emulator, fp_name);
    return 0;
}

