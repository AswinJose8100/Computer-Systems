// convert Hack Assembly Language into an abstract syntax tree
#include "iobuffer.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to make out programs a bit neater
using namespace std ;

using namespace Hack_Assembler ;

// grammer to be parsed:
// a_program ::= instruction* eoi
// instruction ::= a_label | a_instr_name | a_instruction | c_instr_dest | c_instr_reg | c_instr_alu_op
// a_label ::= label
// a_instr_name ::= name
// a_instruction ::= number
// c_instr_reg ::= register (equals alu_op)? (semi jump)?
// c_instr_dest ::= dest equals alu_op (semi jump)?
// c_instr_alu_op ::= alu_op (semi jump)?
// Tokens: label, name, number, null, dest, register, alu_op, jump

// forward declare parsing functions - one per rule
ast parse_program() ;
ast parse_instruction() ;
ast parse_a_label() ;
ast parse_a_instr_name() ;
ast parse_a_instruction() ;
ast parse_c_instr_dest() ;
ast parse_c_instr_reg() ;
ast parse_c_instr_alu_op() ;

// Note: always read one token after the one recognised

// a_program ::= instruction* eoi
ast parse_program()
{
    vector<ast> instructions ;

    // we stop when we see the EOI token
    // this may be end of input or an error
    // we cannot tell so treat both as end of input
    // every new assembler instruction we find is appended to the_program
    while ( have(tk_instruction) )
    {
        instructions.push_back(parse_instruction()) ;
    }
    mustbe(tk_eoi) ;

    return create_program(instructions) ;
}

/*****************   REPLACE THE FOLLOWING CODE  ******************/

// a_program ::= instruction* eoi
ast parse_instruction()
{
    // inspect the token to see what it might start
    if ( have(tk_label) ) return parse_a_label() ;
    if ( have(tk_name) ) return parse_a_instr_name() ;
    if ( have(tk_number) ) return parse_a_instruction() ;
    if ( have(tk_register) ) return parse_c_instr_reg() ;
    if ( have(tk_dest) ) return parse_c_instr_dest() ;
    if ( have(tk_alu_op) ) return parse_c_instr_alu_op() ;
    mustbe(tk_instruction) ;

    return -1 ;
}

// tk_label is the token for '(' label ')'
ast parse_a_label()
{
    string label = token_spelling();

    next_token() ;
              // replace this with code to parse a label

    return create_label(label) ;
}

// tk_address is the token for '@' name
ast parse_a_instr_name()
{
    string name =token_spelling();

    next_token() ;              // replace this with code to parse '@'name

    return create_a_name(name) ;
}

// tk_number is the token for '@' number
ast parse_a_instruction()
{
    int ivalue =token_ivalue();

    next_token() ;              // replace this with code to parse '@'number

    return create_a_instruction(ivalue) ;
}

// c_instruction ::= tk_register (tk_assign tk_alu_op)? (tk_semi tk_jump)?
ast parse_c_instr_reg()
{
    string dest =token_spelling();
    string alu_op = "" ;
    bool val=false;
    string jump = "" ;
   

    next_token() ;              // replace this with code to parse a C instruction that starts with a register
    if(token_kind()==tk_assign)
    {
        val=true;
        next_token();
        alu_op=token_spelling();
        next_token();
        
    }
    else
        alu_op="NULL";
    


    if(token_kind()==tk_semi)
    {
        if(val==false)
        {
            alu_op=dest;
            dest="NULL";
        }
        next_token();
        jump=token_spelling();
        next_token();
        
    }
    else
        jump="NULL";



    return create_c_instruction(dest,alu_op,jump) ;
}

// c_instruction ::= tk_dest tk_assign tk_alu_op (tk_semi tk_jump)?
ast parse_c_instr_dest()
{
    string dest =token_spelling();
    string alu_op = "" ;
    string jump = "" ;

    next_token();              
    next_token();
    alu_op=token_spelling();
    next_token();


    

    if(token_kind()==tk_semi)
    {
        next_token();
        jump=token_spelling();
        next_token();
        
    }
    else
        jump="NULL";

    return create_c_instruction(dest,alu_op,jump) ;
}

// c_instruction ::= tk_alu_op (tk_semi tk_jump)?
ast parse_c_instr_alu_op()
{
    string dest = "NULL" ;
    string alu_op=token_spelling();
    string jump = "" ;

    next_token() ;              // replace this with code to parse a C instruction that starts with an alu_op
    if(token_kind()==tk_semi)
    {
        next_token();
        jump=token_spelling();
        next_token();
        
    }
    else
        jump="NULL";

    return create_c_instruction(dest,alu_op,jump) ;
}

/*****************        DOWN TO HERE         ******************/


// main program
int main(int argc,char **argv)
{
    // hide error messages
    //config_errors(iob_buffer) ;

    // initialise the tokeniser by reading the first token
    next_token() ;

    // parse a class and print the abstract syntax tree as XML
    ast_print_as_xml(parse_program(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

