// convert an abstract syntax tree for Hack Assembly language into machine code
#include "iobuffer.h"
#include "symbols.h"
#include <bitset>
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to simplify the code
using namespace CS_Symbol_Tables ;
using namespace Hack_Assembler ;
using namespace std ;

int val=16,find_a_val;
string str_1,str_2,str_3,str_4;
int val_of_a_reg;
symbols inst;


// This function is called when the switch case is ast_a_name 
string a_Name(ast instruct)
{
    str_1=get_a_name_unresolved(instruct);
    find_a_val=lookup_ints(inst,str_1);
    int value;
    if(find_a_val==-1)
    {
        insert_ints(inst,str_1,val);
        find_a_val=val;
        val=val+1;
    }
    // for padding to make 16
    return bitset<16>(find_a_val).to_string()+"\n";


}
// This is the Symbol Table for the jump instruction
symbols table_symbol_ju_mp()
{

    symbols ju_mp=create_strings();
    insert_strings(ju_mp, "NULL", "000");insert_strings(ju_mp, "JGT", "001");
    insert_strings(ju_mp, "JEQ", "010");
    insert_strings(ju_mp, "JGE", "011");insert_strings(ju_mp, "JLT", "100");
    insert_strings(ju_mp, "JNE", "101");
    insert_strings(ju_mp, "JLE", "110");insert_strings(ju_mp, "JMP", "111");
    //returning the created symbol table
    return ju_mp;
}
// This function is called when the switch case is ast_a_instruction
string a_instruct(ast instruct)
{
    val_of_a_reg=get_a_instruction_value(instruct);
    return bitset<16>(val_of_a_reg).to_string()+"\n";

}
// This function creates the symbol table for the destination instructions
symbols table_destin_ation_symbol() 
{

    symbols des_tination=create_strings();
    insert_strings(des_tination,"NULL","000");insert_strings(des_tination,"M","001");
    insert_strings(des_tination,"D","010");insert_strings(des_tination,"MD","011");
    insert_strings(des_tination,"A","100");insert_strings(des_tination,"AM","101");
    insert_strings(des_tination,"AD","110");insert_strings(des_tination,"AMD","111");
    return des_tination;
}

// This function creats the symbol table special instructions
symbols table_inst_symbol()
{
    symbols inst=create_ints();
    insert_ints(inst,"SP",0);
    insert_ints(inst,"LCL",1);
    insert_ints(inst,"ARG",2);insert_ints(inst,"THIS",3);
    insert_ints(inst,"THAT",4);insert_ints(inst,"R0",0);
    insert_ints(inst,"R1",1);
    insert_ints(inst,"R2",2);
    insert_ints(inst,"R3",3);
    insert_ints(inst,"R4",4);insert_ints(inst,"R5",5);
    insert_ints(inst,"R6",6);insert_ints(inst,"R7",7);
    insert_ints(inst,"R8",8);
    insert_ints(inst,"R9",9);insert_ints(inst,"R10",10);
    insert_ints(inst,"R11",11);insert_ints(inst,"R12",12);
    insert_ints(inst,"R13",13);
    insert_ints(inst,"R14",14);insert_ints(inst,"R15",15);
    insert_ints(inst,"SCREEN",16384);
    insert_ints(inst,"KBD",24576);
    return inst;
}

// This function creates the symbol table for the alu or comp instructions
symbols table_alu_symbol() 
{

    symbols alu=create_strings();
    insert_strings(alu, "0", "0101010");insert_strings(alu, "1", "0111111");
    insert_strings(alu, "-1", "0111010");
    insert_strings(alu, "D", "0001100");
    insert_strings(alu, "A", "0110000");insert_strings(alu, "M", "1110000");
    insert_strings(alu, "!D", "0001101");
    insert_strings(alu, "!A", "0110001");
    insert_strings(alu, "!M", "1110001");insert_strings(alu, "-D", "0001111");
    insert_strings(alu, "-A", "0110011");
    insert_strings(alu, "-M", "1110011");insert_strings(alu, "D+1", "0011111");
    insert_strings(alu, "A+1", "0110111");
    insert_strings(alu, "M+1", "1110111");
    insert_strings(alu, "D-1", "0001110");insert_strings(alu, "A-1", "0110010");
    insert_strings(alu, "M-1", "1110010");
    insert_strings(alu, "D+A", "0000010");
    insert_strings(alu, "D+M", "1000010");insert_strings(alu, "D-A", "0010011");
    insert_strings(alu, "D-M", "1010011");insert_strings(alu, "A-D", "0000111");
    insert_strings(alu, "M-D", "1000111");
    insert_strings(alu, "D&A", "0000000");
    insert_strings(alu, "D&M", "1000000");
    insert_strings(alu, "D|A", "0010101");insert_strings(alu, "D|M", "1010101");
    return alu;
}



// this function provides an example of how to walk an abstract syntax tree constructed by ast_parse_xml()
void walk_program(ast the_program,symbols inst)
{
    // the_program contains a vector of instructions
    int final_count=size_of_program(the_program);
    int count;
    int j;
    j=0;
    count=0;
    while(j<final_count)
    {
        ast instruction=get_program(the_program,j) ;
        switch(ast_node_kind(instruction))
        {
        case ast_c_instruction: 
        case ast_a_instruction:
        case ast_a_name:
            count=count+1;
            break;

        case ast_label:
            if(insert_ints(inst,get_label_name(instruction),count))
            {
                break;
            }
            fatal_error(0,"");    

            break ;
        
        default:
            fatal_error(0,"// bad node - expected ast_label,ast_str_1,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }

        j=j+1;
    }
}

// This function is the second pass and ignores the labels as the lables case is done
// in the function above called walk_program.
void func_tion(symbols des_tination,symbols ju_mp,symbols inst,ast the_program,symbols alu)
{
    int k;
    k=0;
    string str_3;
    int num;
    string str_4;
    int final_num;
    string str_2;
    final_num = size_of_program(the_program);

    while(k<final_num)
    {
        ast instruct = get_program(the_program,k);
        switch(ast_node_kind(instruct))
        {
            case ast_c_instruction:
                str_2=lookup_strings(des_tination,get_c_instruction_dest(instruct));
                str_3=lookup_strings(alu,get_c_instruction_alu(instruct));
                str_4=lookup_strings(ju_mp,get_c_instruction_jump(instruct));
                write_to_output("111"+str_3+str_2+str_4+"\n");
                break;
            case ast_label:
                break;
            
            case ast_a_instruction:
                write_to_output(a_instruct(instruct));
                break;
            case ast_a_name:
                write_to_output(a_Name(instruct));
                break;    
            
            default:
                fatal_error(0, "// bad node - expected ast_label,ast_str_1,ast_a_instruction or ast_c_instruction\n");
                break;
        }
        k=k+1;
    }
}


// translate an abstract syntax tree representation of Hack Assembly language into Hack machine code
// In my code this actual works as my main function and does the first and second passes.
static void asm_translator(ast the_program)
{
    
    symbols inst=table_inst_symbol();
    symbols des_tination=table_destin_ation_symbol();
    symbols alu=table_alu_symbol();
    symbols ju_mp=table_symbol_ju_mp();
    walk_program(the_program,inst);
    func_tion(des_tination,ju_mp,inst,the_program,alu);

}

// main progra
int main(int argc,char **argv)
{
    // error messages
    config_errors(iob_buffer) ;

    // parse abstract syntax tree and pass to the translator
    asm_translator(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}
