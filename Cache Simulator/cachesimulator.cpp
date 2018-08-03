/*
 Cache Simulator
 Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
 The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
 s = log2(#sets)   b = log2(block size)  t=32-s-b
 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
    int L1blocksize;
    int L1setsize;
    int L1size;
    int L2blocksize;
    int L2setsize;
    int L2size;
};

/* you can define the cache class here, or design your own data structure for L1 and L2 cache
 class cache {
 
 }
 */



int main(int argc, char* argv[])
{
    
    
    int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
    int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open("cacheconfig.txt");
    while(!cache_params.eof())  // read config file
    {
        
        cache_params>>dummyLine;
        cache_params>>cacheconfig.L1blocksize;
        cache_params>>cacheconfig.L1setsize;
        cout<<" L1 set size is ----------------"<<cacheconfig.L1setsize;
        cache_params>>cacheconfig.L1size;
        cache_params>>dummyLine;
        cache_params>>cacheconfig.L2blocksize;
        cache_params>>cacheconfig.L2setsize;
        cout<<" L2 set size is ----------------"<<cacheconfig.L2setsize;
        cache_params>>cacheconfig.L2size;
        
    }
    
    
    
    // Implement by you:
    // initialize the hirearch cache system with those configs
    // probably you may define a Cache class for L1 and L2, or any data structure you like
    
    
    //For L1 cache
    
    int L1_blocks, L1_index_bits, L1_offset_bits, L1_tag_bits;
    int L2_blocks, L2_index_bits, L2_offset_bits, L2_tag_bits;
    long int way1,way2;
    
    if (cacheconfig.L1setsize!=0){
        L1_blocks=  ((cacheconfig.L1size) * 1024) / ( (cacheconfig.L1blocksize) * (cacheconfig.L1setsize));
        cout<<"no of blocks L1 "<<L1_blocks;
        L1_index_bits= log2(L1_blocks);
        cout<<"no of index bits L1 "<<L1_index_bits;
        L1_offset_bits = log2(cacheconfig.L1blocksize);
        L1_tag_bits = 32 - L1_index_bits- L1_offset_bits;
        cout<<"no of tag bits L1 "<<L1_tag_bits;
        way1 = cacheconfig.L1setsize;
    }
    else {
        L1_blocks=1;
        cout<<"no of blocks L1 "<<L1_blocks;
        L1_index_bits= 0;
        cout<<"no of index bits L1 "<<L1_index_bits;
        L1_offset_bits = log2(cacheconfig.L1blocksize);
        L1_tag_bits = 32 - L1_index_bits- L1_offset_bits;
        cout<<"no of tag bits L1 "<<L1_tag_bits;
        way1= ((cacheconfig.L1size) * 1024) / ( (cacheconfig.L1blocksize));
        
    }
    
    
    
    // For L2 cache
    
    
    if (cacheconfig.L2setsize!=0){
        L2_blocks=  ((cacheconfig.L2size) * 1024) / ( (cacheconfig.L2blocksize) * (cacheconfig.L2setsize));
        cout<<"no of blocks L2 "<<L2_blocks;
        L2_index_bits= log2(L2_blocks);
        cout<<"no of index bits L2 "<<L2_index_bits;
        L2_offset_bits = log2(cacheconfig.L2blocksize);
        L2_tag_bits = 32 - L2_index_bits- L2_offset_bits;
        cout<<"no of tag bits L2 "<<L2_tag_bits;
        way2 = cacheconfig.L2setsize;
    }
    else {
        L2_blocks=1;
        cout<<"no of blocks L2 "<<L2_blocks;
        L2_index_bits= 0;
        cout<<"no of index bits L2 "<<L2_index_bits;
        L2_offset_bits = log2(cacheconfig.L2blocksize);
        L2_tag_bits = 32 - L2_index_bits- L2_offset_bits;
        cout<<"no of tag bits L2 "<<L2_tag_bits;
        way2= ((cacheconfig.L2size) * 1024) / ( (cacheconfig.L2blocksize));
    }
    cout<<"L1 ways"<<way1<<" L1 ways"<<way2;
    
    
    
    /***** Create Cache and Valid bit array *****/
    
    
    long int L1arraysize=L1_blocks*way1; //how many indexes in total cache array
    long int L2arraysize=L2_blocks*way2; //how many indexes in total cache array
    long int L1RR[L1_blocks];
    long int L2RR[L2_blocks];
    for( int k =0 ; k<L1_blocks; k++)
        L1RR[k] = 0;
    
    for( int k =0 ; k<L2_blocks; k++)
        L2RR[k] = 0;
    
    
    //Create Caches L1 and L2:
    
    bitset<32> *L1= new bitset<32>[L1arraysize];
    bitset<32> *L2= new bitset<32>[L2arraysize];
    
    //Create valid bit array for L1 and L2:
    bitset<1> *L1valid= new bitset<1>[L1arraysize];
    bitset<1> *L2valid= new bitset<1>[L2arraysize];
    
    /***** Cache and Valid bit arrays initalization completed *****/
    
    
    
    
    
    
    
    bitset<32> tag_L1 , index_L1, tag_L2 , index_L2;
    unsigned long int tag_L1_i, index_L1_i, tag_L2_i, index_L2_i;
    unsigned long int flag1,flag2;
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = "trace.out";
    
    traces.open("trace.txt");
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    int count=0;
    if (traces.is_open()&&tracesout.is_open())
    {
        while (getline (traces,line))
        {   // read mem access file and access Cache
            istringstream iss(line);
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
            
            
            
            for ( int j = 0; j <L1_index_bits; j++)
            {
                index_L1[j] = accessaddr[j+L1_offset_bits];
            }
            index_L1_i= int(index_L1.to_ulong());
            
            for ( int m = 0; m <=L1_tag_bits; m++)
            {
                tag_L1[m] = accessaddr[m+L1_index_bits+ L1_offset_bits];
            }
            tag_L1_i=int(tag_L1.to_ulong());
            
            cout<<"For L1 cache  " <<endl;
            cout<<"bitset index "<<index_L1<<endl;
            cout<<"index integer "<<index_L1_i<<endl;
            cout<<" bitset tag " <<tag_L1<<endl;
            cout<<" tag integer " <<tag_L1_i<<endl;
            
            
            for ( int j = 0; j <L2_index_bits; j++)
            {
                index_L2[j] = accessaddr[j + L2_offset_bits];
            }
            index_L2_i= int(index_L2.to_ulong());
            
            for ( int m = 0; m <=L2_tag_bits; m++)
            {
                tag_L2[m] = accessaddr[m+L2_index_bits + L2_offset_bits];
            }
            tag_L2_i=int(tag_L2.to_ulong());
            cout<<"For L2 cache  " <<endl;
            cout<<"bitset index "<<index_L2<<endl;
            cout<<"index integer "<<index_L2_i<<endl;
            cout<<" bitset tag " <<tag_L2<<endl;
            cout<<" tag integer " <<tag_L2_i<<endl;
            
            long unsigned int flag1= int(L1[index_L1_i].to_ulong());
            long unsigned int flag2= int(L2[index_L2_i].to_ulong());
            
            // access the L1 and L2 Cache according to the trace;
            if (accesstype.compare("R")==0)
            {
                if(way1 == 1 && way2 == 1)            // When L1 and L2 are direct mapped
                {
                    
                    
                    if(flag1 == tag_L1_i && L1valid[index_L1_i] == '1')
                    {
                        L1AcceState = 1;
                        L2AcceState = 0;
                    }
                    else if( flag1 != tag_L1_i || L1valid[index_L1_i] == '0')                    //Read miss on L1 check L2
                    {
                        L1AcceState= 2;
                        if(flag2 != tag_L2_i || L2valid[index_L2_i] == '0')                     //Miss on L2 as well , copy values to both L1 and L2 and make both valid bits as 1
                        {
                            L2AcceState = 2;
                            L1valid[index_L1_i] = '1';                                          //Making valid bit for L1 as 1
                            L2valid[index_L2_i] =  '1';                                         //Making valid bit for L2 as 1
                            for(int i = 0 ; i<= L1_tag_bits;i++)
                            {
                                L1[index_L1_i][i] = tag_L1[i];                                   //copying value to L1
                            }
                            for(int m = 0; m<=L2_tag_bits ; m++)
                            {
                                L2[index_L2_i][m] = tag_L2[m];                                   //copying value to L2
                            }
                            
                        }
                        
                    }
                    else if( flag2== tag_L2_i && L2valid[index_L2_i] == '1')                  // When miss in  L1 and hit in L2
                    {
                        L2AcceState=1;
                        if(L1valid[index_L1_i] == '1')
                        {
                            bitset<32> copy,temp;
                            
                            for( int m=0; m<L1_index_bits;m++)
                            {
                                copy[m+L1_offset_bits]= L1[index_L1_i][m+L1_offset_bits];
                            }
                            for( int l= 0; l< L1_tag_bits; l++)
                            {
                                copy[l + L1_index_bits +L1_offset_bits] = L1[index_L1_i][l+ L1_index_bits +L1_offset_bits];
                                temp[l] = L1[index_L1_i][ l + L1_index_bits + L1_offset_bits];
                            }
                            
                            for( int x = 0; x < L1_index_bits; x++)
                            {
                                L1[index_L1_i][x+L1_offset_bits] = accessaddr[x +L1_index_bits +L1_offset_bits];
                            }
                            long int temp_i = int(temp.to_ulong());
                            for( int y =0; y< L2_tag_bits; y++)
                            {
                                L2[temp_i][y] = copy[32-y];
                            }
                            
                        }
                        else if ( L1valid[index_L1_i] == '0')
                        {
                            for(int i = 0 ; i< L1_tag_bits; i++)
                                L1[index_L1_i][i] = L2[index_L2_i][i];
                        }
                    }
                    
                }
                
                
                else if ( way1 == 1 && way2 !=1)           // L1 is dirct mapped L2 is set associative
                {
                    if(flag1 == tag_L1_i && L1valid[index_L1_i] == '1')
                    {
                        L1AcceState = 1;
                        L2AcceState = 0;
                        
                    }
                    else if( flag1!= tag_L2_i || L1valid[index_L1_i] == '1')
                    {
                        L1AcceState = 2;                                                           //Read miss in L1 go to L2
                        long int a;
                        int way_hit = 0;
                        for( int i =0 ; i<way2; i++)
                        {
                            a=index_L2_i + (L2_blocks * i);
                            long int b = int(L2[a].to_ulong());
                            if(tag_L2_i == b)
                            {
                                L2valid[index_L2_i] = '1';
                                L2AcceState = 1;
                                way_hit = 1;
                                break;
                                
                            }
                        }
                        if(way_hit == 0)
                        {
                            L2AcceState = 2;
                            
                            for( int i =0 ; i< L2_tag_bits ; i++)
                            {
                                L2[index_L2_i + ((L2_blocks)*L2RR[index_L2_i])][i] = tag_L2[i];
                                cout<<tag_L2[i];
                            }
                            L2valid[index_L2_i] = 1;
                            
                            if(L2RR[index_L2_i] == (way2-1))
                            {
                                //Round robin counter is full
                                //evicting from the last way
                                L2RR[index_L2_i] = 0;
                            }
                            else
                            {
                                
                                
                                L2RR[index_L2_i]= L2RR[index_L2_i] + 1;
                                
                            }
                            
                            for (int f= 0; f< tag_L1_i; f++)
                            {
                                //Write to L1
                                L1[index_L1_i][f] = tag_L1[f];
                                
                                
                            }
                            L1valid[index_L1_i] = '1';
                            
                        }
                        else if(way_hit==1){
                            for( int i =0 ; i< L1_tag_bits ; i++)
                            {
                                L1[index_L1_i][i] = tag_L1[i];
                                cout<<tag_L1[i];
                            }
                            L1valid[index_L1_i]=1;
                            
                        }
                    }
                    
                }
                else if(way1 != 1 && way2 ==1 )              // L1 is set associative and L2 is direst mapped
                {
                    long int a;
                    int way_hit = 0;
                    for ( int i = 0 ; i<way2; i++)
                    {
                        a= index_L1_i + (L1_blocks * i);
                        long int b = int(L1[a].to_ulong());
                        
                        if(tag_L1_i == b)
                        {
                            L1valid[index_L1_i] = '1' ;
                            L1AcceState = 1;                                                     //Read hit in L1 dont go to L2
                            L2AcceState = 0;
                            way_hit = 1;
                            break;
                        }
                    }
                    if(way_hit == 0)                                                             //Read miss in L1 go to L2
                    {
                        L1AcceState=2;
                        if(flag2== tag_L2_i && L2valid[index_L2_i] == '1')
                        {
                            L2AcceState=1;
                            if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '0')
                            {
                                for(int j =0 ; j<tag_L1_i; j++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][j] = tag_L1[j];
                                }
                                L1valid[index_L1_i + index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] = '1';
                            }
                            else if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '1')                            //When L1 is not empty , evict from L1 put to L2
                            {
                                
                                bitset<32> copy,temp;
                                for(int t=0;t<=L1_index_bits;t++)
                                {
                                    copy[t+L1_offset_bits]= L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][t+L1_offset_bits];
                                }
                                
                                for( int u=0 ; u<L1_tag_bits;u++)
                                {
                                    
                                    copy[u + L1_index_bits +L1_offset_bits] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][u+ L1_index_bits +L1_offset_bits];
                                    temp[u] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][ u + L1_index_bits + L1_offset_bits];
                                }
                                
                                for( int x = 0; x < L1_index_bits; x++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][x+L1_offset_bits] = accessaddr[x +L1_index_bits +L1_offset_bits];
                                }
                                long int temp_i = int(temp.to_ulong());
                                for( int y =0; y< L2_tag_bits; y++)
                                {
                                    L2[temp_i][y] = copy[32-y];
                                }
                                
                                
                            }
                        }
                        else if ( flag2!= tag_L2_i || L2valid[index_L2_i] == '0')        // Read miss in L2
                        {
                            for(int v = 0 ; v<tag_L2_i; v++)
                            {
                                L2[index_L2_i][v] = tag_L2[v];
                            }
                            if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '0')
                            {
                                for(int n =0 ; n<tag_L1_i; n++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][n] = tag_L1[n];
                                }
                                L1valid[index_L1_i + index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] = '1';
                            }
                            
                            else if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '1')                            //When L1 is not empty , evict from L1 put to L2
                            {
                                
                                bitset<32> copy,temp;
                                for(int t=0;t<=L1_index_bits;t++)
                                {
                                    copy[t+L1_offset_bits]= L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][t+L1_offset_bits];
                                }
                                
                                for( int u=0 ; u<L1_tag_bits;u++)
                                {
                                    
                                    copy[u + L1_index_bits +L1_offset_bits] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][u+ L1_index_bits +L1_offset_bits];
                                    temp[u] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][ u + L1_index_bits + L1_offset_bits];
                                }
                                
                                for( int x = 0; x < L1_index_bits; x++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][x+L1_offset_bits] = accessaddr[x +L1_index_bits +L1_offset_bits];
                                }
                                
                            }
                            
                        }
                        
                    }
                    
                    
                    
                }
                else if(way1!= 1 && way2!=1)               // L1 and L2 are both set associative
                {
                    long int a;
                    int way_hit = 0;
                    for ( int i = 0 ; i<way1; i++)
                    {
                        a= index_L1_i + (L1_blocks * i);
                        long int b = int(L1[a].to_ulong());
                        
                        if(tag_L1_i == b)
                        {
                            L1valid[index_L1_i] = '1' ;
                            L1AcceState = 1;                                                     //Read hit in L1 dont go to L2
                            L2AcceState = 0;
                            way_hit = 1;
                            break;
                        }
                    }
                    if ( way_hit == 0)
                    {
                        L1AcceState = 2;                                                    //Read miss in L1 go to L2
                        long int c;
                        int way_hit_L2 = 0;
                        for( int i =0 ; i<way2; i++)
                        {
                            c=index_L2_i + (L2_blocks * i);
                            long int v = int(L2[c].to_ulong());
                            if(tag_L2_i == v)
                            {
                                L2valid[index_L2_i] = '1';
                                L2AcceState = 1;
                                way_hit_L2 = 1;
                                break;
                                
                            }
                        }
                        if(way_hit_L2 == 0)
                        {
                            
                            L2AcceState = 2;
                            if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '0')
                            {
                                for(int j =0 ; j<tag_L1_i; j++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][j] = tag_L1[j];
                                }
                                L1valid[index_L1_i + index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] = '1';
                            }
                            else if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '1')                            //When L1 is not empty , evict from L1 put to L2
                            {
                                
                                bitset<32> copy,temp;
                                for(int t=0;t<=L1_index_bits;t++)
                                {
                                    copy[t+L1_offset_bits]= L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][t+L1_offset_bits];
                                }
                                
                                for( int u=0 ; u<L1_tag_bits;u++)
                                {
                                    
                                    copy[u + L1_index_bits +L1_offset_bits] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][u+ L1_index_bits +L1_offset_bits];
                                    temp[u] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][ u + L1_index_bits + L1_offset_bits];
                                }
                                
                                for( int x = 0; x < L1_index_bits; x++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][x+L1_offset_bits] = accessaddr[x +L1_index_bits +L1_offset_bits];
                                }
                                long int temp_i = int(temp.to_ulong());
                                for( int y =0; y< L2_tag_bits; y++)
                                {
                                    L2[temp_i][y] = copy[32-y];
                                }
                                
                                
                            }
                            
                            for(int h =0 ; h<tag_L2_i; h++)                                             //Writing to L2
                            {
                                L2[index_L2_i + ((L2_blocks)*L2RR[index_L2_i])][h] = tag_L2[h];
                            }
                            L2valid[index_L2_i + index_L2_i + ((L2_blocks)*L2RR[index_L2_i])] = '1';
                        }
                        else if(way_hit_L2==1){
                            
                            
                            
                            L2AcceState = 2;
                            if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '0')
                            {
                                for(int j =0 ; j<tag_L1_i; j++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][j] = tag_L1[j];
                                }
                                L1valid[index_L1_i + index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] = '1';
                            }
                            else if(L1valid[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])] == '1')                            //When L1 is not empty , evict from L1 put to L2
                            {
                                
                                bitset<32> copy,temp;
                                for(int t=0;t<=L1_index_bits;t++)
                                {
                                    copy[t+L1_offset_bits]= L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][t+L1_offset_bits];
                                }
                                
                                for( int u=0 ; u<L1_tag_bits;u++)
                                {
                                    
                                    copy[u + L1_index_bits +L1_offset_bits] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][u+ L1_index_bits +L1_offset_bits];
                                    temp[u] = L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][ u + L1_index_bits + L1_offset_bits];
                                }
                                
                                for( int x = 0; x < L1_index_bits; x++)
                                {
                                    L1[index_L1_i + ((L1_blocks)*L1RR[index_L1_i])][x+L1_offset_bits] = accessaddr[x +L1_index_bits +L1_offset_bits];
                                }
                                long int temp_i = int(temp.to_ulong());
                                for( int y =0; y< L2_tag_bits; y++)
                                {
                                    L2[temp_i][y] = copy[32-y];
                                }
                                
                                
                            }
                            
                        }
                        
                        
                    }
                    
                    
                }
                
            }
            
            else     //Write access
            {
                
                if(way1== 1 && way2 == 1)         // Both L1 and L2 are direct mapped
                {
                    
                    
                    if (flag1 == tag_L1_i && L1valid[index_L1_i] == '1')                                   ///Write Hit in L1 then L2 is no access
                    {
                        
                        
                        L1AcceState = 3;
                        L2AcceState = 0;
                        
                    }
                    else if(flag1 != tag_L1_i || L1valid[index_L1_i] != '1')                                     // Write miss in L1 going to L2
                    {
                        L1AcceState= 4 ;
                        if(flag2 == tag_L2_i && L2valid[index_L2_i] == 1)
                        {
                            L2AcceState=3;
                            
                        }
                        else if( flag2 != tag_L2_i || L2valid[index_L2_i] == 0)
                        {
                            L2AcceState= 4;
                        }
                    }
                    
                }
                else if ( way1 == 1 && way2!= 1 )                 // L1 is direct mapped L2 is set associative
                {
                    if(flag1 == tag_L1_i && L1valid[index_L1_i] == '1')
                    {
                        L1AcceState = 3;
                        L2AcceState = 0;
                    }
                    else if(flag1 != tag_L1_i || L1valid[index_L1_i] !='1')
                    {
                        L1AcceState = 4;
                        long int a;
                        int way_hit = 0;
                        for( int i =0 ; i<way2; i++)
                        {
                            a=index_L2_i + (L2_blocks * i);
                            long int b = int(L2[a].to_ulong());
                            if(tag_L2_i == b)
                            {
                                L2valid[index_L2_i] = '1';
                                L2AcceState = 3;
                                way_hit = 1;
                                break;
                            }
                        }
                        if(way_hit == 0)
                        {
                            L2AcceState = 4;
                        }
                        
                    }
                    
                }
                else if ( way1 != 1 && way2 ==1)                //L1 is set associative and L2 is direct mapped
                {
                    long int a;
                    int way_hit = 0;
                    for ( int i = 0 ; i<way1; i++)
                    {
                        a= index_L1_i + (L2_blocks * i);
                        long int b = int(L1[a].to_ulong());
                        
                        if(tag_L1_i == b)
                        {
                            L1valid[index_L1_i] = '1' ;
                            L1AcceState = 3;                                                     //Write hit in L1 dont go to L2
                            L2AcceState = 0;
                            way_hit = 1;
                            break;
                        }
                    }
                    if(way_hit == 0)
                    {
                        L1AcceState = 4;
                        if(flag2 == tag_L2_i && L2valid[index_L2_i] == 1)
                        {
                            L2AcceState=3;
                            
                        }
                        else if( flag2 != tag_L2_i || L2valid[index_L2_i] == 0)
                        {
                            L2AcceState= 4;
                        }
                        
                    }
                    
                }
                else if ( way1 !=1 && way2!=1)                         // L1 and L2 are both set associative
                {
                    long int a;
                    int way_hit = 0;
                    for ( int i = 0 ; i<way1; i++)
                    {
                        a= index_L1_i + (L1_blocks * i);
                        long int b = int(L1[a].to_ulong());
                        
                        if(tag_L1_i == b)
                        {
                            L1valid[index_L1_i] = '1' ;
                            L1AcceState = 3;                                                     //Write hit in L1 dont go to L2
                            L2AcceState = 0;
                            way_hit = 1;
                            break;
                        }
                    }
                    
                    if( way_hit == 0)
                        
                    {
                        L1AcceState = 4;
                        long int c;
                        int way_hit_2 = 0;
                        for( int j =0 ; j<way2; j++)
                        {
                            c=index_L2_i + (L2_blocks * j);
                            long int d = int(L2[c].to_ulong());
                            if(tag_L2_i == d)
                            {
                                L2valid[index_L2_i] = '1';
                                L2AcceState = 3;
                                way_hit_2 = 1;
                                break;
                                
                            }
                        }
                        if(way_hit_2 == 0)
                        {
                            L2AcceState = 4;
                        }
                        
                    }
                }
            }
            
            tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
            
        }
        traces.close();
        tracesout.close();
    }
    else cout<< "Unable to open trace or traceout file ";
    
    
    
    return 0;
    
    
    
    //R 0xb77af5b8
    
}

