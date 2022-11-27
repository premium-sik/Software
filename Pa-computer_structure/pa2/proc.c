/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   proc.c                                                    */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include "proc.h"
#include "mem.h"
#include "util.h"
int check = 0;


/***************************************************************/
/* System (CPU and Memory) info.                                             */
/***************************************************************/
struct MIPS32_proc_t g_processor;

// int to_binary(int n){
//     unsigned temp = 0x8000000;
//     for(int i =0; i < 32; i++){
//         if((temp & n) == temp){
            
//         }
//     }
// }
/***************************************************************/
/* Fetch an instruction indicated by PC                        */
/***************************************************************/
int fetch(uint32_t pc)
{
    return mem_read_32(pc);
}

/***************************************************************/
/* TODO: Decode the given encoded 32bit data (word)            */
/***************************************************************/
struct inst_t decode(int word)
{
    // if(g_processor.num_insts == g_processor.input_insts){
    //     g_processor.running = FALSE;
    // }
    struct inst_t inst;
    //printf("\ncheck word: %x\n", word);

    
    inst.opcode = (unsigned)word >> 26;
    //printf("\n op: %d\n", inst.opcode);
    
    if(inst.opcode == 0){ //R-type
        inst.r_t.r_i.rs = (unsigned)(word << 6) >> 27;
        //printf("\n rs: %d\n", inst.r_t.r_i.rs);
        inst.r_t.r_i.rt = (unsigned)word << 11 >> 27;
        //printf("\n rt: %d\n", inst.r_t.r_i.rt);
        inst.r_t.r_i.r_i.r.rd = (unsigned)word << 16 >> 27;
        //printf("\n rd: %d\n", inst.r_t.r_i.r_i.r.rd); 
        inst.r_t.r_i.r_i.r.shamt =(unsigned) word <<21 >> 27;
        //printf("\n shamt: %d\n", inst.r_t.r_i.r_i.r.shamt);
        inst.func_code = (unsigned int)word << 26 >> 26;
    }


    else if(inst.opcode == 2 || inst.opcode == 3){//J-format
        inst.r_t.target = word << 6 >> 6;
    }

    else{
        inst.r_t.r_i.rs = (unsigned)(word << 6) >> 27;
        //printf("\n rs: %d\n", inst.r_t.r_i.rs);
        inst.r_t.r_i.rt = (unsigned)word << 11 >> 27;
        //printf("\n rt: %d\n", inst.r_t.r_i.rt);
        inst.r_t.r_i.r_i.imm = word << 16 >> 16;
        //printf("\n imme: %d\n",  inst.r_t.r_i.r_i.imm);
    }
    
    return inst;
}

/***************************************************************/
/* TODO: Execute the decoded instruction                       */
/***************************************************************/
void execute(struct inst_t inst)
{
    // int count = 0;
    // printf("\n count: %d\n", count);
    // count++;
    int temp_pc;
    
    if(inst.opcode == 0 && inst.func_code == 0x20){ //add
        //printf("\nin\n");
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = g_processor.regs[inst.r_t.r_i.rs] + g_processor.regs[inst.r_t.r_i.rt];
        //printf("\n add rd: %d\n", inst.r_t.r_i.r_i.r.rd); 
    }

    else if(inst.opcode == 0 && inst.func_code == 0x21){ //addu
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = (unsigned int)g_processor.regs[inst.r_t.r_i.rs] + (unsigned int)g_processor.regs[inst.r_t.r_i.rt];
    }

    else if(inst.opcode == 0 && inst.func_code == 0x24){ //and
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = g_processor.regs[inst.r_t.r_i.rs] & g_processor.regs[inst.r_t.r_i.rt];
    }

    else if(inst.opcode == 0 && inst.func_code == 0x8){ //jr
        //printf("\nin\n");
        g_processor.pc = g_processor.regs[inst.r_t.r_i.rs];
    }

    else if(inst.opcode == 0 && inst.func_code == 0x27){//nor
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = ~(g_processor.regs[inst.r_t.r_i.rs] | g_processor.regs[inst.r_t.r_i.rt]);
    }

   else if(inst.opcode == 0 && inst.func_code == 0x25){//or
        //printf("\nin\n");
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = (g_processor.regs[inst.r_t.r_i.rs] | g_processor.regs[inst.r_t.r_i.rt]);
    }

    else if(inst.opcode == 0 && inst.func_code == 0x2b){//sltu
        if(g_processor.regs[inst.r_t.r_i.rs] < g_processor.regs[inst.r_t.r_i.rt]){
            g_processor.regs[inst.r_t.r_i.r_i.r.rd] = 1;
        }
        else{
            g_processor.regs[inst.r_t.r_i.r_i.r.rd] = 0;
        }
    }

    else if(inst.opcode == 0 && inst.func_code == 0x00 && inst.r_t.r_i.r_i.r.shamt != 0){//sll -> rt가 base, rd가 result.
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = g_processor.regs[inst.r_t.r_i.rt] << inst.r_t.r_i.r_i.r.shamt;
    }

    else if(inst.opcode == 0 && inst.func_code == 0x02){//srl
       //printf("\n in\n");
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = g_processor.regs[inst.r_t.r_i.rt] >> inst.r_t.r_i.r_i.r.shamt;
    }

    else if(inst.opcode == 0 && inst.func_code == 0x22){ //sub
    //printf("\nininni\n");
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = g_processor.regs[inst.r_t.r_i.rs] - g_processor.regs[inst.r_t.r_i.rt];
    }

    else if(inst.opcode == 0 && inst.func_code == 0x23){ //subu
        g_processor.regs[inst.r_t.r_i.r_i.r.rd] = (unsigned int)(g_processor.regs[inst.r_t.r_i.rs] - g_processor.regs[inst.r_t.r_i.rt]);
    }
    
    /////////////////////////////////////////////////R-format end!

    else if(inst.opcode ==  0x09){//addiu
        g_processor.regs[inst.r_t.r_i.rt] = (unsigned)(g_processor.regs[inst.r_t.r_i.rs] + (unsigned)inst.r_t.r_i.r_i.imm);
    }

    else if(inst.opcode ==  0x0c){//andi
        g_processor.regs[inst.r_t.r_i.rt] = g_processor.regs[inst.r_t.r_i.rs] & inst.r_t.r_i.r_i.imm;
    }

    else if(inst.opcode ==  0x04){ //beq
        //printf("\n beq rt: %d\n", g_processor.regs[inst.r_t.r_i.rt]);
        //printf("\n beq rs: %d\n", g_processor.regs[inst.r_t.r_i.rs]) ;
        if(g_processor.regs[inst.r_t.r_i.rt] == g_processor.regs[inst.r_t.r_i.rs]){
            //printf("\n beq in?\n");
            g_processor.pc = g_processor.pc + 4 * inst.r_t.r_i.r_i.imm;
        }
    }

    else if(inst.opcode ==  0x05){//bne
        //printf("\nbne in\n");
        if(g_processor.regs[inst.r_t.r_i.rt] != g_processor.regs[inst.r_t.r_i.rs]){
            g_processor.pc = g_processor.pc + 4* inst.r_t.r_i.r_i.imm;
        }
    }

    else if(inst.opcode ==  0x0d){ //ori
        g_processor.regs[inst.r_t.r_i.rt] = g_processor.regs[inst.r_t.r_i.rs] | inst.r_t.r_i.r_i.imm;
    }

    else if(inst.opcode ==  0x0f){ //lui

        //printf("\nin\n");
        //printf("\n imm: %d\n", (int)inst.r_t.r_i.r_i.imm);
        //printf("\n rt: %d\n", inst.r_t.r_i.rt);
        g_processor.regs[inst.r_t.r_i.rt] = (inst.r_t.r_i.r_i.imm << 16);
    }

    else if(inst.opcode ==  0x0b){ //sltiu 근데 unsigned 처리 안 해도 되겠징?
        //printf("\n sltiu rs: %d\n", g_processor.regs[inst.r_t.r_i.rs]);
        //printf("\n sltiu imm: %d\n", inst.r_t.r_i.r_i.imm);
        if(g_processor.regs[inst.r_t.r_i.rs] < (unsigned)inst.r_t.r_i.r_i.imm){
            g_processor.regs[inst.r_t.r_i.rt] = 1;
            //printf("\n sltiu rt: %d\n", g_processor.regs[inst.r_t.r_i.rt]);
        }
        else{
            g_processor.regs[inst.r_t.r_i.rt] = 0;
        }
    }


    else if(inst.opcode ==  0x23){ //lw
        g_processor.regs[inst.r_t.r_i.rt] = mem_read_32(g_processor.regs[inst.r_t.r_i.rs] + (inst.r_t.r_i.r_i.imm));
        //printf("\n lw in\n");

    }

    else if(inst.opcode == 0x2b){ //sw
        //printf("\n sw rt: %d\n", inst.r_t.r_i.rt);
        //printf("\n sw  re_rt: %d\n", g_processor.regs[inst.r_t.r_i.rt]);
        mem_write_32(g_processor.regs[inst.r_t.r_i.rs] + inst.r_t.r_i.r_i.imm, g_processor.regs[inst.r_t.r_i.rt]);
        //printf("\n sw  re_rt: %d\n", g_processor.regs[inst.r_t.r_i.rt]);
    }

    ///////////////////////////////////////////////////// I-format 끝!
    
    else if(inst.opcode == 0x02){//j
        //printf("\nin\n");
        //inst.r_t.target = inst.r_t.target<<2;
        //temp_pc = g_processor.pc >> 28 << 28;
        //g_processor.pc = temp_pc + g_processor.pc;
        inst.r_t.target = inst.r_t.target<<2;
        temp_pc = g_processor.pc >> 28 << 28;
        g_processor.pc = temp_pc + inst.r_t.target;

    }

    else if(inst.opcode == 0x03){//jal
        //printf("\n jal in\n");
        // printf("\n pc: 0x%x\n",g_processor.pc);
        g_processor.regs[31] = g_processor.pc + 4;
        //inst.r_t.target = inst.r_t.target<<2;
        // temp_pc = g_processor.pc >> 28 << 28;
        // g_processor.pc = g_processor.pc + temp_pc;
        //printf("\n pc: 0x%x\n",g_processor.pc);
        inst.r_t.target = inst.r_t.target<<2;
        temp_pc = g_processor.pc >> 28 << 28;
        g_processor.pc = temp_pc + inst.r_t.target;   
    }

    else{
         g_processor.pc -= 4;
         g_processor.running = FALSE;
        //  if(g_processor.regs[4] == 0xffffffff){
        //     g_processor.regs[4] = 0xffffffec;
        //     g_processor.pc = 0x00400014;
        //  }
     }

}

/***************************************************************/
/* Advance a cycle                                             */
/***************************************************************/
void cycle()
{
    int inst_reg;
    struct inst_t inst;

    // 1. fetch
    inst_reg = fetch(g_processor.pc);
    //printf("")
    g_processor.pc += BYTES_PER_WORD;

    // 2. decode
    inst = decode(inst_reg);

    // 3. execute
    execute(inst);

    // 4. update stats
    g_processor.num_insts++;
    g_processor.ticks++;
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump() {
    int k;

    printf("\n[INFO] Current register values :\n");
    printf("-------------------------------------\n");
    printf("PC: 0x%08x\n", g_processor.pc);
    printf("Registers:\n");
    for (k = 0; k < MIPS_REGS; k++)
        printf("R%d: 0x%08x\n", k, g_processor.regs[k]);
}



/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate MIPS for n cycles                      */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {
    int i;
    
    if (g_processor.running == FALSE) {
        printf("[ERROR] Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("[INFO] Simulating for %d cycles...\n", num_cycles);
    for (i = 0; i < num_cycles; i++) {
        if (g_processor.running == FALSE) {
            printf("[INFO] Simulator halted\n");
            break;
        }
        cycle();
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate MIPS until HALTed                      */
/*                                                             */
/***************************************************************/
void go() {
    if (g_processor.running == FALSE) {
        printf("[ERROR] Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("[INFO] Simulating...\n");
    while (g_processor.running)
        cycle();
    printf("[INFO] Simulator halted\n");
} 