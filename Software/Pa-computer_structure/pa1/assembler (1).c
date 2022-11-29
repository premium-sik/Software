#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

/*
 * For debug option. If you want to debug, set 1.
 * If not, set 0.
 */
#define DEBUG 0

#define MAX_SYMBOL_TABLE_SIZE   1024
#define MEM_TEXT_START          0x00400000
#define MEM_DATA_START          0x10000000
#define BYTES_PER_WORD          4
#define INST_LIST_LEN           22

/******************************************************
 * Structure Declaration
 *******************************************************/

typedef struct inst_struct {
    char *name;
    char *op;
    char type;
    char *funct;
} inst_t;

typedef struct symbol_struct {
    char name[32];
    uint32_t address; // 32비트(4바이트) 크기의 부호 없는 정수형 변수 선언
} symbol_t;

enum section {
    DATA = 0,
    TEXT,
    MAX_SIZE
};

/******************************************************
 * Global Variable Declaration
 *******************************************************/

inst_t inst_list[INST_LIST_LEN] = {       //  idx
    {"add",     "000000", 'R', "100000"}, //    0
    {"sub",     "000000", 'R', "100010"}, //    1
    {"addiu",   "001001", 'I', ""},       //    2
    {"addu",    "000000", 'R', "100001"}, //    3
    {"and",     "000000", 'R', "100100"}, //    4
    {"andi",    "001100", 'I', ""},       //    5
    {"beq",     "000100", 'I', ""},       //    6
    {"bne",     "000101", 'I', ""},       //    7
    {"j",       "000010", 'J', ""},       //    8
    {"jal",     "000011", 'J', ""},       //    9
    {"jr",      "000000", 'R', "001000"}, //   10
    {"lui",     "001111", 'I', ""},       //   11
    {"lw",      "100011", 'I', ""},       //   12
    {"nor",     "000000", 'R', "100111"}, //   13
    {"or",      "000000", 'R', "100101"}, //   14
    {"ori",     "001101", 'I', ""},       //   15
    {"sltiu",   "001011", 'I', ""},       //   16
    {"sltu",    "000000", 'R', "101011"}, //   17
    {"sll",     "000000", 'R', "000000"}, //   18
    {"srl",     "000000", 'R', "000010"}, //   19
    {"sw",      "101011", 'I', ""},       //   20
    {"subu",    "000000", 'R', "100011"}  //   21
};

symbol_t SYMBOL_TABLE[MAX_SYMBOL_TABLE_SIZE]; // Global Symbol Table

uint32_t symbol_table_cur_index = 0; // For indexing of symbol table

/* Temporary file stream pointers */
FILE *data_seg;
FILE *text_seg;

/* Size of each section */
uint32_t data_section_size = 0;
uint32_t text_section_size = 0;

/******************************************************
 * Function Declaration
 *******************************************************/

/* Change file extension from ".s" to ".o" */
char* change_file_ext(char *str) {
    char *dot = strrchr(str, '.'); // str에서 .이 마지막으로 등장하는 위치 반환

    if (!dot || dot == str || (strcmp(dot, ".s") != 0))
        return NULL; // .이 없거나, 처음에 나오거나, dot이 .s와 같거나.

    str[strlen(str) - 1] = 'o';
    return "";
}

/* Add symbol to global symbol table */
void symbol_table_add_entry(symbol_t symbol)
{
    SYMBOL_TABLE[symbol_table_cur_index++] = symbol;
#if DEBUG
    printf("%s: 0x%08x\n", symbol.name, symbol.address);
#endif
}

/* Convert integer number to binary string */
char* num_to_bits(unsigned int num, int len)
{
    char* bits = (char *) malloc(len+1);
    int idx = len-1, i;
    while (num > 0 && idx >= 0) {
        if (num % 2 == 1) {
            bits[idx--] = '1';
        } else {
            bits[idx--] = '0';
        }
        num /= 2;
    }
    for (i = idx; i >= 0; i--){
        bits[i] = '0';
    }
    bits[len] = '\0';
    return bits;
}

/* Record .text section to output file */
void record_text_section(FILE *output)
{
    uint32_t cur_addr = MEM_TEXT_START; // == 0x00400000
    char line[1024];
    /* Point to text_seg stream */
    rewind(text_seg); // 파일의 읽기쓰기 위치를 파일의 처음 위치로 초기화

    /* Print .text section */
    while (fgets(line, 1024, text_seg) != NULL) { // line에 text_seg 한 줄 씩 저장(\n 포함 저장)
        char inst[0x1000] = {0};
        char op[32] = {0};
        char label[32] = {0};
        char type = '0';
        int i, idx = 0;
        int rs, rt, rd, imm, shamt;
        int addr;

        rs = rt = rd = imm = shamt = addr = 0;
#if DEBUG
        printf("0x%08x: ", cur_addr);
#endif
        /* Find the instruction type that matches the line */
        /* blank */

        
        char* temp;
        temp=strtok(line, " \t\n"); // ex) temp == and/lui/add
        
        for(idx=0;idx<22;idx++){ // idx는 inst_list 번호
            if(!strcmp(temp, inst_list[idx].name)){ // temp == inst_list[idx].name 일 때
                type=inst_list[idx].type;
                break;
            }
        } // type, idx는 결정된 상태
        /* blank */
        switch (type) {
            case 'R': 
                /* blank */
                // op, rs, rt, rd, shamt, funct 필요
                strcpy(op, inst_list[idx].op); // op 저장
                fputs(op, output); // op 출력

                if(!strcmp(temp, "jr")){ // rs 하나만 있음
                    temp=strtok(NULL, "$\t"); // temp에 숫자만 들어오게 됨
                    rs=atoi(temp);
                    strcpy(inst, num_to_bits(rs, 5));
                    fputs(inst, output);
                    fputs("000000000000000", output);
                    fputs(inst_list[idx].funct, output);
                }
                else if(!strcmp(temp, "sll")||!strcmp(temp, "srl")){ // rs 없고 shamt 있음
                    fputs("00000", output); // rs 빈자리 채우기
                    temp=strtok(NULL, "$,\t"); // rd에서 숫자만 떼어내기
                    rd=atoi(temp);
                    temp=strtok(NULL, "$"); // rt에서 숫자만 떼어내기
                    temp=strtok(NULL, ",\t"); 
                    rt=atoi(temp);

                    strcpy(inst, num_to_bits(rt, 5));
                    strcat(inst, num_to_bits(rd, 5));
                    fputs(inst, output); // rd까지 다 출력

                    temp=strtok(NULL, " \t\n"); // shamt 가져오기
                    shamt=atoi(temp);
                    fputs(num_to_bits(shamt, 5), output); // shamt 출력
                    fputs(inst_list[idx].funct, output);
                }
                else{
                    temp=strtok(NULL, "$,\t"); // rd
                    rd=atoi(temp);
                    temp=strtok(NULL, "$"); // rs
                    temp=strtok(NULL, ",\t"); 
                    rs=atoi(temp);
                    temp=strtok(NULL, "$"); // rt
                    temp=strtok(NULL, ",\t\n"); 
                    rt=atoi(temp);
                    
                    strcpy(inst, num_to_bits(rs, 5));
                    strcat(inst, num_to_bits(rt, 5));
                    strcat(inst, num_to_bits(rd, 5));
                    fputs(inst, output);

                    fputs("00000", output); // shamt
                    fputs(inst_list[idx].funct, output);
                }
                /* blank */
#if DEBUG
                printf("op:%s rs:$%d rt:$%d rd:$%d shamt:%d funct:%s\n",
                        op, rs, rt, rd, shamt, inst_list[idx].funct);
#endif
                break;

            case 'I': // temp == op인 상태
                /* blank */
                // op, rs, rt, imm 필요
                strcpy(op, inst_list[idx].op); // op 저장
                fputs(op, output); // op 출력
                
                if(!strcmp(temp, "lui")){ // rt만 있음
                    fputs("00000", output); // rs 빈 자리 채우기
                    temp=strtok(NULL, "$,\t"); // rt
                    rt=atoi(temp);
                    fputs(num_to_bits(rt, 5), output);

                    
                    temp=strtok(NULL, " \t\n"); // imm
                    

                    if(strchr(temp, 'x')!=NULL){ // temp가 16진수일 때
                        imm=strtol(temp, NULL, 16);
                    }
                    else{ // temp가 10진수일 때
                        imm=strtol(temp, NULL, 10);
                    }
                    fputs(num_to_bits(imm, 16), output);
                }
                else if(!strcmp(temp, "lw")||!strcmp(temp, "sw")){ // 괄호 구별
                    temp=strtok(NULL, " $,\t"); // rt
                    rt=atoi(temp);
                    temp=strtok(NULL, " ,(\t"); // imm
                    imm=atoi(temp);
                    temp=strtok(NULL, " $)\n"); // rs
                    rs=atoi(temp);

                    strcpy(inst, num_to_bits(rs, 5));
                    strcat(inst, num_to_bits(rt, 5));
                    strcat(inst, num_to_bits(imm, 16));
                    fputs(inst, output);
                }
                else if(!strcmp(temp, "beq")||!strcmp(temp, "bne")){ // branchaddress(3rd값) 존재
                    temp=strtok(NULL, " ,$\t\n"); // temp == rs
                    rs=atoi(temp);
                    temp=strtok(NULL, " ,$\t\n"); // temp == rt
                    rt=atoi(temp);
                    temp=strtok(NULL, " ,\t\n"); // temp == label이름 (ex loop2, lap5)
                    
                    // label 주소 찾기
                    int label_addr;
                    for(i=0;i<symbol_table_cur_index;i++){
                        if(!strcmp(temp, SYMBOL_TABLE[i].name)){
                            label_addr=(int)(SYMBOL_TABLE[i].address); // uint32_t
                        }
                    }
                    //printf("label addr: %x\ncurrent addr: %x\n", label_addr, cur_addr);
                    imm=(label_addr-(int)cur_addr)/4-1;// 원래꺼에서 -1 지우고 +1함
                    //printf("imm: %d\n", imm);
                    
                    /*if(imm<0){
                        imm=~imm+1;
                        printf("complement imm: %d\n", imm);
                    }*/

                    fputs(num_to_bits(rs, 5), output);
                    fputs(num_to_bits(rt, 5), output);
                    fputs(num_to_bits(imm, 16), output);
                    
                }
                else{ // addiu, andi, ori에 3rd값 10진수/16진수 구별
                    // addiu, andi, ori, sltiu. 뒤에 값 세 개 옴
                    temp=strtok(NULL, " ,$\t\n"); // temp == rt
                    rt=atoi(temp);
                    temp=strtok(NULL, " ,$\t\n"); // temp == rs
                    rs=atoi(temp);
                    temp=strtok(NULL, " \t\n"); // temp == imm
                    if(strchr(temp, 'x')!=NULL){ // temp(imm)가 16진수일 때. 10진수로 변환해줌
                        imm=strtol(temp, NULL, 16);
                    }
                    else{ // temp(imm)가 10진수일 떄
                        imm=atoi(temp);
                    }
                    strcpy(inst, num_to_bits(rs, 5));
                    strcat(inst, num_to_bits(rt, 5));
                    strcat(inst, num_to_bits(imm, 16));
                    fputs(inst, output);

                }
                /* blank */
#if DEBUG
                printf("op:%s rs:$%d rt:$%d imm:0x%x\n",
                        op, rs, rt, imm);
#endif
                break;

            case 'J':
                /* blank */
                // op, addr 필요
                strcpy(op, inst_list[idx].op); // op 저장
                fputs(op, output); // op 출력
                temp=strtok(NULL, " \t\n"); 
                for(i=0;i<symbol_table_cur_index;i++){
                        //printf("%d.%s.%x\n", i, SYMBOL_TABLE[i].name, SYMBOL_TABLE[i].address);
                    if(!strcmp(temp, SYMBOL_TABLE[i].name)){
                        //printf("\nwork\n");
                        addr=(int)SYMBOL_TABLE[i].address; // target주소
                    }
                }
                addr=addr>>2;
                char* bits_temp=num_to_bits(addr, 26);
                fputs(bits_temp, output);

                /* blank */
#if DEBUG
                printf("op:%s addr:%i\n", op, addr);
#endif
                break;

            default:
                break;
        }
        fprintf(output, "\n"); // 줄 바꿔주기

        cur_addr += BYTES_PER_WORD;
    }
}

/* Record .data section to output file */
void record_data_section(FILE *output)
{
    uint32_t cur_addr = MEM_DATA_START;
    char line[1024];

    /* Point to data segment stream */
    rewind(data_seg);
    
    /* Print .data section */
    while (fgets(line, 1024, data_seg) != NULL) {
        /* blank */
        char _line[1024]={0};
        strcpy(_line, line); 
        char* temp;
        unsigned int num;
        temp=strtok(_line, " \t\n"); // temp == .word
        temp=strtok(NULL, " \t\n"); // ex) temp == 1, 2, 0x100

        // 주소값문자열을 unsigned int 형태로 변환하기
        if(strchr(temp, 'x')!=NULL){ // temp에 저장된 값이 16진수일 때
            num=strtol(temp, NULL, 16);
        }
        else{ // temp에 저장된 값이 10진수일 때
            num=atoi(temp);
        }

        char* bits=num_to_bits(num, 32);
        fputs(bits, output);
        fputs("\n", output);
        /* blank */
#if DEBUG
        printf("0x%08x: ", cur_addr);
        printf("%s", line);
#endif
        cur_addr += BYTES_PER_WORD;
    }
}



/* Fill the blanks */
void make_binary_file(FILE *output)
{
#if DEBUG
    char line[1024] = {0};
    rewind(text_seg);
    /* Print line of text segment */
    while (fgets(line, 1024, text_seg) != NULL) {
        printf("%s",line);
    }
    printf("text section size: %d, data section size: %d\n",
            text_section_size, data_section_size);
#endif
    /* Print text section size and data section size */
    /* blank */
    fputs(num_to_bits(text_section_size, 32), output);
    fputs("\n", output);
    fputs(num_to_bits(data_section_size, 32), output);
    fputs("\n", output);
    /* blank */
    /* Print .text section */
    record_text_section(output);
    /* Print .data section */
    record_data_section(output);
}



/* Fill the blanks */
void make_symbol_table(FILE *input)
{
    char line[1024] = {0};
    uint32_t address = 0;
    enum section cur_section = MAX_SIZE;

    /* Read each section and put the stream */
    while (fgets(line, 1024, input) != NULL) {
    // 개행문자 만나면 line에 읽은 문자열을 넣어서 반환
    // 한 줄씩 나오고, 마지막 줄까지 다 읽으면 NULL 리턴
        char *temp;
        char _line[1024] = {0};
        strcpy(_line, line); // _line에 line 복사
        // _line에는 input 한 줄씩 들어가있음
        temp = strtok(_line, " \t\n");

        /* Check section type */
        if (!strcmp(temp, ".data")) { // temp == ".data"
            /* blank */
            cur_section=DATA;
            address=MEM_DATA_START;
            /* blank */
            data_seg = tmpfile(); // 임시 (이진)파일 생성, 프로그램 종료 시 제거됨
            continue; // while문 다시 실행
        }
        else if (!strcmp(temp, ".text")) { // temp == ".text"
            /* blank */
            cur_section=TEXT;
            address=MEM_TEXT_START;
            /* blank */
            text_seg = tmpfile();
            continue; 
        }

        // section 사이즈 한 줄 당 4
        /* Put the line into each segment stream */
        if (cur_section == DATA) {
            /* blank */
            //data _seg에 fput으로 값 넣어주기.
            data_section_size+=4; // 한 줄 당 한 번
            // 첫 번째 섹션 저장 & symbol table 채우기
            // ex) temp == data1: or .word
            if(strchr(temp, ':')!=NULL){ // temp에 :가 있으면
                // ex) temp == data1:
                char temp_d[10]={0};
                for(int i=0;temp[i]!=':';i++){
                    temp_d[i]=temp[i];
                }
                strcpy(SYMBOL_TABLE[symbol_table_cur_index].name, temp_d);
                SYMBOL_TABLE[symbol_table_cur_index].address=address;
                symbol_table_cur_index++;
                temp=strtok(NULL, " \t\n");
            } // ex) data1: 로 시작할 경우. 처리 끝남
            
            // 이제 .word 처리할 차례
            fputs(temp, data_seg);
            fputs(" ", data_seg);
            
            // 이제 .word 뒤의 숫자값 처리할 차례
            temp=strtok(NULL, " \t\n");

            fputs(temp, data_seg);
            fputs("\n", data_seg);
        }
        else if (cur_section == TEXT) {
            /* blank */
            
            // ex) temp == main: OR temp == lui
            if(strchr(temp, ':')!=NULL){ // temp에 :가 있으면
                char temp_t[30]={0};

                for(int i=0;temp[i]!=':';i++)
                    temp_t[i]=temp[i];

                strcpy(temp, temp_t); // ex) temp == main
                strcpy(SYMBOL_TABLE[symbol_table_cur_index].name, temp);
                SYMBOL_TABLE[symbol_table_cur_index].address=address;
                symbol_table_cur_index++;
                continue;// 방금 추가
                //address -= BYTES_PER_WORD; // 방금 추가한 거
            }
            else{ // temp에 :가 없으면. ex) temp == lui
                text_section_size+=4;
                while(temp!=NULL){
                    if(!strcmp(temp, "la")){ // temp == la 일 때
                        uint32_t label;
                        uint16_t upperlabel;
                        uint16_t lowerlabel;
                        char r[10];
                        fputs("lui ", text_seg);
                        temp=strtok(NULL, " \t\n"); // ex) temp == $2,
                        strcpy(r, temp); // r에 temp(==$2)값을 미리 저장해놓음(후에 ori 쓸 때 필요)
                        fputs(temp, text_seg);
                        fputs(" ", text_seg);
                        temp=strtok(NULL, " \t\n"); // ex) temp == data1
                        
                        for(int i=0;i<symbol_table_cur_index;i++){ // data1 주소값 찾기
                            if(!strcmp(temp, SYMBOL_TABLE[i].name)){ // ex) temp(==data1) == SYMBOL_TABLE[i].name
                                label=SYMBOL_TABLE[i].address;
                            }
                        } // label == la 지시어에 오는 데이타 섹션의 주소값
                        lowerlabel=label; // label을 upper, lower로 각각 16비트주소로 나누기
                        upperlabel=label>>16;

                        fprintf(text_seg, "0x%x", upperlabel); // lui 줄은 끝남
                        
                        if(lowerlabel!=0x0000){ // ori도 써야 함
                            text_section_size+=4;
                            fputs("\nori ", text_seg);
                            fputs(r, text_seg); // ex) $2
                            fputs(" ", text_seg);
                            fputs(r, text_seg);
                            fputs(" ", text_seg);
                            fprintf(text_seg, "%d", lowerlabel);
                            address += BYTES_PER_WORD;
                        }
                        temp=strtok(NULL, " \t\n");
                        // lowerlabel == 0x0000이면 ori 안 써도 됨
                    } 
                    else{ // temp != la 일 때 // else문이 2번 더 돌아감 이거 고쳐야돼
                        fputs(temp, text_seg);
                        fputs(" ", text_seg); // 섹션 사이 띄어쓰기 추가
                        temp=strtok(NULL, " \t\n");
                    }
                }
                fputs("\n", text_seg);
            }
            

        }

        address += BYTES_PER_WORD; // 4씩 증가
    } // while문 종료
}

/******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/

int main(int argc, char* argv[])
{
    FILE *input, *output;
    char *filename;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Read the input file */
    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /* Create the output file (*.o) */
    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    // strdup: 동적할당 실행 후 문자열 복사하는 함수
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /******************************************************
     *  Let's complete the below functions!
     *
     *  make_symbol_table(FILE *input)
     *  make_binary_file(FILE *output)
     *  ├── record_text_section(FILE *output)
     *  └── record_data_section(FILE *output)
     *
     *******************************************************/
    
    make_symbol_table(input);
    make_binary_file(output);
    
    fclose(input);
    fclose(output);
    return 0;
}

