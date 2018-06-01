#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <utility>

using namespace std;

#define T 1 //Taken
#define NT 0 //Not taken

//Defining integer values for 2_bit counter states

#define ST 3 // Strongly taken - '11'
#define WT 2 // Weakly taken - '10'
#define WNT 1 // Weakly Not taken - '01'
#define STN 0 // Strongly not taken - '00'

int main(int argc, char const *argv[])
{
	
	int size=0,k=0;				// Size and k store the values of m nd k respectively 
	//char bhr[1];
	int predict = 0;
	long int miss_count = 0;

	std::ifstream config_params;

	config_params.open("config.txt");
	config_params>>size>>k;

	cout<<"\nSize of m is : "<<size;
	cout<<"\nSize of K is : "<<k<<endl;

	int long counter_indices = 0;
	int k_indices;

	counter_indices = pow(2,size);
	k_indices = pow(2,k);			//Finding the number of indices 


	//Creating the saturating counter based on m and k value  
	vector< vector < int > > sat_counter( counter_indices, vector<int>(k_indices, ST));	

	// creating our branch history register which is equal to k bits and initialised as taken
	vector< bool > bhr(k,T);
	int long int_bhr;
	
	
	ifstream traces;
	ofstream tracesout;
	string outname = "traces.txt.out";  //Name of traces output file


	traces.open("trace.txt");

	tracesout.open(outname.c_str());


	string line;
	string branch_status; // variable to read the actual branch status of the current branch  
	string xaddr; // Branch address in hexadecimal 
	unsigned int addr;   //address of the branch in unsigned int format 
	bitset<32> accessaddr; // Address of the branch in bitset 
	bitset<32> mbitaddr;  // storing 'm' lsb bits


	  

	if(traces.is_open())
	{
		while (getline(traces,line))
		{
			long int mbit_int;
			istringstream iss(line);
			if(!(iss>>xaddr>>branch_status)) {break;}
			stringstream saddr(xaddr);
			
			saddr >> std::hex >> (addr);
			
			accessaddr = bitset<32> (addr);
			
			// Saving m lsb's to a new mbitaddr bitset 

			int p=0;
			for ( int k = 0 ;k<size;k++)
			{
				mbitaddr[p] = accessaddr[k];
				p++;
			}

			mbit_int = (mbitaddr.to_ulong());
			// cout<<"\n m ki value in bitset : "<<mbitaddr;
			 //cout<<"\n integer value of m lsb's is : "<<mbit_int;
			//Integer value of Mbitaddr:


			//Will map to 2_bit counter array using integer value of BHR 
			// Cannot use to_ulong function as it only works with bitset int_bhr = (bhr.to_ulong());

			 bitset<32> temp;

			 for( int i = 0;i<k; i++)
			 {
			 	temp[i] = bhr[i];
			 }

			 int_bhr = (temp.to_ulong());
			//cout<<"\n bitset bhr ki value hai yeo : "<<temp; 
			//cout<<"\n Integer bhr ki value hai yeo : "<<int_bhr;
			//cout<<"\n Actual value of branch -- : "<<branch_status;
			if(k>0)			//Unstructured IF condition to solve k=0
				{
							if(branch_status == "1")				//Branch is taken			
							{

								if(sat_counter[mbit_int][int_bhr] == ST)
								{
									predict = 1;
									//cout<<"A";
								}
								else if(sat_counter[mbit_int][int_bhr] == WT)
								{
									predict = 1;
									sat_counter[mbit_int][int_bhr] = ST;
									//cout<<"B";
								}
								else if(sat_counter[mbit_int][int_bhr] == WNT)
								{
									predict = 0;
									sat_counter[mbit_int][int_bhr] = ST;
									//cout<<"C";
								}
								else if(sat_counter[mbit_int][int_bhr] == STN)
								{
									predict = 0;
									sat_counter[mbit_int][int_bhr] = WNT;
									//cout<<"D";
								}

								//Shifting BHR in regards to the actual status of the branch 
								for(int i=0;i<k-1;i++)
								{
									bhr[i] = bhr[i+1];
								}

								bhr[k-1] = 1;    //Since the latest branch is Taken

								
							}

							else if(branch_status == "0")			//Branch is not taken 
							{
								if(sat_counter[mbit_int][int_bhr] == ST)
								{
									predict = 1;
									sat_counter[mbit_int][int_bhr] = WT;
									//cout<<"E";
								}
								else if(sat_counter[mbit_int][int_bhr] == WT)
								{
									predict = 1;
									sat_counter[mbit_int][int_bhr] = STN;
									//cout<<"F";
								}
								else if(sat_counter[mbit_int][int_bhr] == WNT)
								{
									predict = 0;
									sat_counter[mbit_int][int_bhr] = STN;
									//cout<<"G";
								}
								else if(sat_counter[mbit_int][int_bhr] == STN)
								{
									predict = 0;
									//cout<<"H";
								}

								//Shifting BHR in regards to the actual status of the branch 
								
								for(int i=0;i<k-1;i++)
								{
									bhr[i] = bhr[i+1];
								}

								bhr[k-1] = 0;  //Since the latest branch is Taken
							}

				}
				else if(k==0)
				{

					if(branch_status == "1")				//Branch is taken			
							{

								if(sat_counter[mbit_int][0] == ST)
								{
									predict = 1;
									//cout<<"A";
								}
								else if(sat_counter[mbit_int][0] == WT)
								{
									predict = 1;
									sat_counter[mbit_int][0] = ST;
									//cout<<"B";
								}
								else if(sat_counter[mbit_int][0] == WNT)
								{
									predict = 0;
									sat_counter[mbit_int][0] = ST;
									//cout<<"C";
								}
								else if(sat_counter[mbit_int][0] == STN)
								{
									predict = 0;
									sat_counter[mbit_int][0] = WNT;
									//cout<<"D";
								}

							

								
							}

							else if(branch_status == "0")			//Branch is not taken 
							{
								if(sat_counter[mbit_int][0] == ST)
								{
									predict = 1;
									sat_counter[mbit_int][0] = WT;
									//cout<<"E";
								}
								else if(sat_counter[mbit_int][0] == WT)
								{
									predict = 1;
									sat_counter[mbit_int][0] = STN;
									//cout<<"F";
								}
								else if(sat_counter[mbit_int][0] == WNT)
								{
									predict = 0;
									sat_counter[mbit_int][0] = STN;
									//cout<<"G";
								}
								else if(sat_counter[mbit_int][0] == STN)
								{
									predict = 0;
									//cout<<"H";
								}

								
							}				

				}			
			tracesout<<predict<<endl;

			if(predict != atoi(branch_status.c_str()))			//To count no of miss-predictions
				miss_count++;
			

		}
		traces.close();
		tracesout.close();
	}
	else
		{
			cout<<"\n Unable to open file";
		}






	config_params.close();

	//sat_counter[0][2] = 0;
	// counter1[0][1] = 0;

	//cout<<"\n0, 0 waali value"<<sat_counter[0][0];
	//cout<<"\n 0 1 waaali value"<<sat_counter[0][0];
	//cout<<"\n 0 2 waaali value"<<sat_counter[0][0];

	//cout<<"\n CHECKING BHR ";

	//cout<<"\n pehli value "<<bhr[0];
	//cout<<"\n dooosri value "<<bhr[1];

	//cout<<"\nno of misses "<<miss_count;

	return 0;
}