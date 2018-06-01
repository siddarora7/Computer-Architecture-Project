#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.



// Creating user defined functions to extract 5 bits , 6 bits and for sign extension 

bitset<32> signextend(bitset<16> Imm_bits)  //Function that accepts a bitset of 16 bits and outputs a 32 bit sign extend bitset.
{
  bitset<32> bit_ex;
  for(int i = 0; i<16; i++)
  {
    bit_ex[i] = Imm_bits[i];
    bit_ex[i+16] = Imm_bits[15];
  }

  return bit_ex;
}

bitset<6> six_bit_funct(bitset<32> Ins, int s , int e)
{
  bitset<6> temp;
  int i, j=5;
  for(i = s; i<= e ; i--)
  {
    temp[j] = Ins[i];
    j--;
  }
return temp;
}

bitset<5> five_bit_funct(bitset<32> Ins, int s , int e)
{
  bitset<5> t;
  int i, j=4;
  for(i = s; i<= e ; i--)
  {
    t[j] = Ins[i];
    j--;
  }
return t;
}


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.     
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];         // reading the data from the registers

            if(WrtEnable.to_ulong() == 1)
            {
              Registers[WrtReg.to_ulong()] = WrtData;
            }


         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you. 

              if(ALUOP.to_ulong() == ADDU)
                  ALUresult = oprand1.to_ulong() + oprand2.to_ulong();        //Add
             
              else if(ALUOP.to_ulong() == SUBU)
                  ALUresult = oprand1.to_ulong() - oprand2.to_ulong();        //Sub
             
              else if(ALUOP.to_ulong() == AND)
                  ALUresult = oprand1 & oprand2;                              //AND
             
              else if(ALUOP.to_ulong() == OR)
                  ALUresult = oprand1|oprand2;                              //OR
              
              else if(ALUOP.to_ulong() == NOR)
                  ALUresult = ~(oprand1|oprand2);                           //NOR
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               // implement by you. (Read the byte at the ReadAddress and the following three byte).

                string inst_temp;
                for(int i =0 ; i < 4; i++)
                {
                  bitset<8> t = IMem[(ReadAddress.to_ulong()) + i];
                  inst_temp = inst_temp + t.to_string();
                }
                Instruction = bitset<32>(inst_temp); 
               return Instruction;     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               // implement by you.
            string temp_data;
            if(readmem == 1)
            {
              for(int j = 0; j<4; j++)
              {
                bitset<8> temp = DMem[Address.to_ulong() + 1];
                temp_data = temp_data + temp.to_string();
              }
              readdata = bitset<32>(temp_data);
            }
            else if(writemem == 1)
            {
              int in = 31;        //Pointing to the MSB
              for(int i = 0; i<4; i++)        //Counter to write 4 bytes of data 
              {
                for(int j = 7; j>=0; j--)       //To write MSB of each byte first
                {
                  DMem[Address.to_ulong()+i][j] = WriteData[in];
                  in--;
                }
              }
            }
               

               return readdata;     
            
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;
                  
                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {
                  
                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
                     
                  }
                  else cout<<"Unable to open file";
                  fileout.close();

}

   
int main()
{   
    int check =0;
    bitset<32> pc=0;
    bitset<32> Current_Ins;
    bitset<6> opcode;
    bitset<5> rs,rt,rd;
    bitset<6> funct_op; 
    bitset<3> Alu_op;
    bitset<32> result;
    bitset<32> offset;
    bitset<32> jump_add;
    bitset<32> r_data;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    while (1) //each loop body corresponds to one clock cycle.
	{
        

	
        // Executing in order. Will reverse to simulate pipeline

        //*************************************** STAGE 1 **************************************************************
        // Fetchng Instructions

          Current_Ins = myInsMem.ReadMemory(pc.to_ulong());             //Current Instruction

          if(Current_Ins == 4294967295)
           {
             break;                                                        //Break because reached Halt instruction
           }

	     //Getting opcode from the function
  
         opcode = six_bit_funct(Current_Ins,31,26);

 //*************************************** STAGE 2 **************************************************************
        // Decoding instructions
 
       if (opcode == 0x00)                    //R type instructions
          {

            rs = five_bit_funct(Current_Ins,25,20);
            rt = five_bit_funct(Current_Ins,20,16);
            rd = five_bit_funct(Current_Ins,15,11);

            // No need to decode shamt for R type.
            funct_op = six_bit_funct(Current_Ins,5,0);

            // getting the first 3 bits from the function as only first 3 bits are needed (LSB's)

            for (int i = 0 ; i< 3; i++)
            {
              Alu_op[i]=  funct_op[i];
            }

            myRF.ReadWrite(rs,rt,rd,0,0);     // 
          }
      else if(opcode == 0x09|| opcode == 0x23 || opcode == 0x2B || opcode == 0x04)            // I TYPE instructions
          {
            rs = five_bit_funct(Current_Ins,25,20);
            rt = five_bit_funct(Current_Ins,20,16);
          
            //Getting sign extended value
            bitset<16> imm;
            for(int i = 0; i< 16; i++)  
            {
              imm[i] = Current_Ins[i];
            }
            offset = signextend(imm);
            myRF.ReadWrite(rs, rt, rt, 0, 0);
          }
      else if(opcode == 0x02)            //J type value 
          {
             bitset<32> pc_temp = pc.to_ulong() + 4;
             
             for ( int i = 2 ; i< 28; i++)
             {
              jump_add[i] = Current_Ins[i-2];         // Getting 26 bits 
             }
              
             for ( int i = 28 ; i< 32; i++)
             {
              jump_add[i] = pc_temp[i];             //from pc + 4 , get 
             }

             for ( int i = 0 ; i<2; i++)
             {
              jump_add[i] = 0;
             }
            

          }

      //Executing

      if(opcode == 0x00)                //R type execution 
          {
            result = myALU.ALUOperation(Alu_op, myRF.ReadData1, myRF.ReadData2);
          }
      else if (opcode == 0x09|| opcode == 0x23 || opcode == 0x2B)             //I type - addiu , lw and sw
          {
            Alu_op = ADDU;              //All offset calculations handled by ALU
            result = myALU.ALUOperation(Alu_op, myRF.ReadData1, offset);
          }
      else if ( opcode == 0x04)           //I type - beq instruction 
          {
            if(myRF.ReadData1 == myRF.ReadData2)
            {
              check =1;         // Branch 
            }
          }


//*************************************** STAGE 3 **************************************************************
        // WB to RF and Mem

      //Memory Read and Write 
      if(opcode == 0x23)                   //Load word
          {
            r_data = myDataMem.MemoryAccess(result, myRF.ReadData2, 1, 0);
          }
      else if (opcode == 0x2B)            //Store Word
          {
            myDataMem.MemoryAccess(result, myRF.ReadData2, 0, 1);
          }

      // Snippets interracting with Register File

      if(opcode != 0x04 && opcode != 0x2b && opcode != 0x02)
      {
            if(opcode == 0x00)                              //Writing back R type
            {
              myRF.ReadWrite(rs,rt,rd,result,1);
            }
            else
            {
              myRF.ReadWrite(rs, rt, rt, r_data, 1);          // Writing back addiu and lw
            }

     }

      cout<<"PC "<<pc.to_ulong();
          if(opcode == 0x02)
          {
            pc = jump_add;
          }
      else if(opcode == 0x04 && check =='1')
          {
            offset = offset << 2;                               //Left shift by 2 . as we need 2 zereos at the end. Multiples of 4
            pc = pc.to_ulong() + 4 + offset.to_ulong();
          }
      else 
          pc = pc.to_ulong() + 4;
	
	
    // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
    // The first dumped pc value should be 0.	
  if(opcode == 0x00)
  {  
  dumpResults(pc,rd ,result , 0, 0, 0,0 );
  }
  else if(opcode == 0x09 || opcode == 23)   
  {
    dumpResults(pc,rt,r_data, 1, 0, 0,0 );
  }   
  else if(opcode == 0x2b)
  {
    dumpResults(pc,0 ,0 , 0, result, myRF.ReadData2,1 );

  }
  else
  {
        dumpResults(pc,0 ,0 , 0, 0, 0,1);

  }
    }
	          myRF.OutputRF(); // dump RF; 

          myDataMem.OutputDataMem(); // dump data mem
      
          return 0;
  }

