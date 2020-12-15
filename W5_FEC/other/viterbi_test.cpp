#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int u32;
#define TRANSMIT_BIT	(122)
#define CLEAN_BIT		(6)
#define BITN			(TRANSMIT_BIT + CLEAN_BIT)

// function prototype
void bit_generator(vector<u32> &bit);
void convolution(vector<u32> &bit, vector<u32> &encode_bit);
void deconvolution(vector<u32> &rec_bit, vector<u32> &bit);

void init_table(int table[][BITN]);
void output_calculator(u32 state, u32 in_bit, vector<u32> &output_bit);
int hamming_distance(vector<u32> &bit1, vector<u32> &bit2);
// function realization

void init_table(int table[][BITN])
{
	// 全部初始化为-1
	int i = 0, j = 0;
	for (i = 0; i < 64; i++)
	{
		for (j = 0; j < (BITN); j++)
		{
			table[i][j] = 65536;
		}
	}
}

void output_calculator(u32 state, u32 in_bit, vector<u32> &out_bit)
{
	/*
	 * state 	-> present state of decoder
	 * in_bit 	-> 0/1
	 * out_bit 	-> output bit of decoder
	 */
	u32 pre_bit[6];
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		pre_bit[i] = state % 2;
		state = state >> 1;
	}
	out_bit[0] = in_bit ^ pre_bit[1] ^ pre_bit[2] ^ pre_bit[4] ^ pre_bit[5];
	out_bit[1] = in_bit ^ pre_bit[0] ^ pre_bit[1] ^ pre_bit[2] ^ pre_bit[5];
}

int hamming_distance(vector<u32> &bit1, vector<u32> &bit2)
{
	int distance = 0;

	for (int i = 0; i < 2; i++)
	{
		if (bit1[i] != bit2[i])
			distance++;
	}

	return distance;
}

void bit_generator(vector<u32> &bit)
{
	int n;

	for(n = 0; n < TRANSMIT_BIT; n++)
	{
		bit[n] = rand() % 2;
	}
	for (n = TRANSMIT_BIT; n < BITN; n++)
	{
		// 清洗比特，使译码器状态归零
		bit[n] = 0;
	}
}

void deconvolution(vector<u32> &rec_bit, vector<u32> &bit)
{
	// define varible
	int path[64][BITN], metric[64][BITN];	// path用于记录前一个状态，metric用于记录之前的汉明距离
	init_table(path);
	init_table(metric);
	int b_pointer = 0; // bit pointer
	int s_pointer = 0;
	vector<u32> out_bit(2);
	vector<u32> temp_bit(2);
	// calculate table
	for (b_pointer = 0; b_pointer < (BITN); b_pointer++)	// bit pointer
	{
		if (b_pointer == 0)
		{	// 初始化比较特殊只有两个
			for (int z = 0; z < 2; z++)
			{
				output_calculator(0, z, out_bit);
				temp_bit[0] = rec_bit[b_pointer * 2];
				temp_bit[1] = rec_bit[b_pointer * 2 + 1];
				metric[z][b_pointer] = hamming_distance(out_bit, temp_bit);
				path[z][b_pointer] = 0;
			}
		}
		else
		{
			for (s_pointer = 0; s_pointer < 64; s_pointer++)	// 每一个状态遍历一遍 state pointer
			{
				if (metric[s_pointer][b_pointer - 1] != 65536)		// 如果之前这个状态已经有路径了
				{
					// 0
					for (int z = 0; z < 2; z++)
					{
						output_calculator(s_pointer, z, out_bit);
						temp_bit[0] = rec_bit[b_pointer * 2];
						temp_bit[1] = rec_bit[b_pointer * 2 + 1];
						if ((hamming_distance(out_bit, temp_bit) + metric[s_pointer][b_pointer - 1]) 
								< metric[((s_pointer << 1) & (0b111111)) + z][b_pointer])
						{
							metric[((s_pointer << 1) & (0b111111)) + z][b_pointer] = hamming_distance(out_bit, temp_bit)
																	+ metric[s_pointer][b_pointer - 1];
							// 记录转移过来的状态
							path[((s_pointer << 1) & (0b111111)) + z][b_pointer] = s_pointer;
						}
					}
				}
			}
		}
	}
	// 回溯
	int state = 0; // 末尾编码器状态一定是0，有清洗比特
	for (b_pointer = (BITN - 1); b_pointer >= 0; b_pointer--)
	{
		bit[b_pointer] = (state & 0b0001);
		state = path[state][b_pointer];
	}
}

void convolution(vector<u32> &bit, vector<u32> &encode_bit)
{
	u32 t_1 = 0, t_2 = 0, t_3 = 0, t_4 = 0, t_5 = 0, t_6 = 0;
	int count = 0;
	
	for (count = 0; count < BITN; count++)
	{
		encode_bit[count * 2] = bit[count] ^ t_2 ^ t_3 ^ t_5 ^ t_6;
		encode_bit[count * 2 + 1] = bit[count] ^ t_1 ^ t_2 ^ t_3 ^ t_6;
		// refresh t_1, t_2
		t_6 = t_5;
		t_5 = t_4;
		t_4 = t_3;
		t_3 = t_2;
		t_2 = t_1;
		t_1 = bit[count];
	}
}

int main()
{
	vector<u32> original_bit(BITN);
	vector<u32> encoded_bit(BITN * 2);
	vector<u32> decoded_bit(BITN);
	// generate bit
	bit_generator(original_bit);
	// encode
	convolution(original_bit, encoded_bit);
	// decode
	deconvolution(encoded_bit, decoded_bit);
	int error = 0;
	for (int i = 0; i < BITN; i++)
	{
		if (original_bit[i] != decoded_bit[i])
			error++;
	}
	cout << "error = " << error << endl;
}