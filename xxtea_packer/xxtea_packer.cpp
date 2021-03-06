// xxtea_packer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <direct.h>
#include <windows.h>
#include "xxtea.h"

using namespace std;

void show_help();
void encode_files(const char * src_dir, const char* dst_dir, const char* xxtea_key, const char* signment);
void decode_files(const char * src_dir, const char* dst_dir, const char* xxtea_key, const char* signment);

int main(int argc, char* argv[])
{
	show_help();
	while (1)
	{
		if (argc < 5) {
			printf("xxtea>>");
			std::string cmd, src, dst, key, sign;
			scanf_s("%s %s %s %s %s", &cmd, &src, &dst, &key, &sign);
			printf("\ninput: %s %s %s %s %s\n", cmd.c_str(), src.c_str(), dst.c_str(), key.c_str(), sign.c_str());
			if (cmd == "encode")
			{
				encode_files(src.c_str(), dst.c_str(), key.c_str(), sign.c_str());
				continue;
			}
			if (cmd == "decode")
			{
				decode_files(src.c_str(), dst.c_str(), key.c_str(), sign.c_str());
				continue;
			}
			show_help();
			continue;
		}
		if (strcmp(argv[1], "encode") == 0) {
			encode_files(argv[2], argv[3], argv[4], argv[5]);
			break;
		}
		if (strcmp(argv[1], "decode") == 0) {
			decode_files(argv[2], argv[3], argv[4], argv[5]);
			break;
		}
		break;
	}
}

void show_help()
{
	printf("----------------------------------------------------------------------------------\n");
	printf("xxtea packer!\n");
	printf("----------------------------------------------------------------------------------\n");
	printf("command list:\n");
	printf("1、encode : use xxtea_key encode src_dir's files, output in dst_dir!\n");
	printf("  format: encode  src_dir|string  dst_dir|string  xxtea_key|string  signment|string\n");	
	printf("2、decode : use xxtea_key decode src_dir's files, output in dst_dir!\n");
	printf("  format: decode  src_dir|string  dst_dir|string  xxtea_key|string  signment|string\n");
	printf("  example : encode ../cbin/src/ ./encode/ cogame XXTEA \n");
	printf("  example : decode . /encode  . /decode/ cogame XXTEA \n");
	printf("----------------------------------------------------------------------------------\n");
	printf("bat command : must add the exe file name! \n");
	printf("example : xxtea_packer encode ../cbin/src/ ./encode/ cogame XXTEA \n");
	printf("warning : file dir must end with '\\'!\n");
	printf("----------------------------------------------------------------------------------\n");
}

void decode_files(const char * src_dir, const char* dst_dir, const char* xxtea_key, const char* signment)
{
	char find_dir[256];
	strncpy_s(find_dir, src_dir, strlen(src_dir));
	strcat_s(find_dir, "*.*");

	HANDLE hFind;
	WIN32_FIND_DATA findData;
	hFind = FindFirstFile(find_dir, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		_mkdir(dst_dir);
		do
		{
			// 忽略"."和".."两个结果 
			if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
				continue;

			char child_src[256];
			char child_dst[256];
			strncpy_s(child_src, src_dir, strlen(src_dir));
			strncpy_s(child_dst, dst_dir, strlen(dst_dir));
			strcat_s(child_src, findData.cFileName);
			strcat_s(child_dst, findData.cFileName);
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // 是否是目录 
			{
				_mkdir(child_dst);
				strcat_s(child_dst, "\\");
				strcat_s(child_src, "\\");
				decode_files(child_src, child_dst, xxtea_key, signment);
			}
			else
			{
				std::ifstream i(child_src, ios::binary);
				std::ofstream o(child_dst, ios::binary);

				int length;
				i.seekg(0, std::ios::end);
				length = i.tellg();
				i.seekg(0, std::ios::beg);
				char* buffer = new char[length];
				i.read(buffer, length);
				i.close();

				if (strncmp(buffer, signment, strlen(signment)) == 0)
				{
					xxtea_long len = 0;
					length -= strlen(signment);
					unsigned char* text = xxtea_decrypt((unsigned char*)(buffer + strlen(signment)), length, (uint8_t*)xxtea_key, strlen(xxtea_key), &len);
					printf("decode: %s, size=%d, decrypt = %d\n", child_src, length, len);
					o.write((const char*)text, len);
				}
				else
				{
					o.write(buffer, length);
				}
				o.close();
			}
		} while (FindNextFile(hFind, &findData));
	}	
}

void encode_files(const char * src_dir, const char* dst_dir, const char* xxtea_key, const char* signment)
{
	char find_dir[256];
	strncpy_s(find_dir, src_dir, strlen(src_dir));
	strcat_s(find_dir, "*.*");

	HANDLE hFind;
	WIN32_FIND_DATA findData;
	hFind = FindFirstFile(find_dir, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		_mkdir(dst_dir);
		do
		{
			// 忽略"."和".."两个结果 
			if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
				continue;

			char child_src[256];
			char child_dst[256];
			strncpy_s(child_src, src_dir, strlen(src_dir));
			strncpy_s(child_dst, dst_dir, strlen(dst_dir));
			strcat_s(child_src, findData.cFileName);
			strcat_s(child_dst, findData.cFileName);
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // 是否是目录 
			{
				_mkdir(child_dst);
				strcat_s(child_dst, "\\");
				strcat_s(child_src, "\\");
				encode_files(child_src, child_dst, xxtea_key, signment);
			}
			else
			{
				std::ifstream i(child_src, std::ios::binary);
				std::ofstream o(child_dst, ios::binary);

				size_t length;
				i.seekg(0, ios::end);
				length = i.tellg();          
				i.seekg(0, std::ios::beg);   
				char* buffer = new char[length];
				i.read(buffer, length);       
				i.close();

				if (strncmp(buffer, signment, strlen(signment)) == 0)
				{
					o.write(buffer, length);
				}
				else
				{
					xxtea_long len = 0;
					unsigned char* text = xxtea_encrypt((unsigned char*)buffer, length, (unsigned char*)xxtea_key, strlen(xxtea_key), &len);
					printf("xxtea_encrypt: %s, size=%d, encrypt = %d\n", child_src, length, len);

					o.write(signment, strlen(signment));
					o.write((const char*)text, len);
				}				
				o.close();
			}
		} while (FindNextFile(hFind, &findData));
	}
}