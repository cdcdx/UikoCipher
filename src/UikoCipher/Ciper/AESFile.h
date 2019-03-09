#define NID_aes_128_cbc		419
//============================================================================
// Name        : test4.cpp
// Author      : DingKaimeng
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// EncFile2.cpp : Defines the entry point for the console application.
//

#define NID_aes_128_cbc		419
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
//#include "EncryptFile.h"

#pragma comment( lib, ".\\lib\\libeay32.lib")
#pragma warning( disable : 4996) 

BOOL AES_Encrypt_File(char PlainFilePath[], char CipherFilePath[], int n_ID, unsigned char* password) 
{
	unsigned char key[EVP_MAX_KEY_LENGTH]; //保存密钥的数组
	unsigned char iv[EVP_MAX_KEY_LENGTH]; ////保存初始化向量的数组
	EVP_CIPHER_CTX ctx; //EVP加密上下文环境
	unsigned char out[1024]; //保存密文的缓冲区
	int outl;
	unsigned char in[1024]; //保存原文的缓冲区
	int inl;
	const EVP_CIPHER * cipher; //加密算法
	int rv;
	FILE *fpIn; //输入文件句柄
	FILE *fpOut; //输出文件句柄
	char enchead[128] = { 0 }; //保存密文文件头的数组

	//根据算法ID获得EVP_CIPHER算法
	//cipher = EVP_get_cipherbynid(n_ID);
	cipher = EVP_aes_128_ecb();
	if (cipher == NULL) {
		return FALSE;
	}
	//打开待加密的原文文件
	fpIn = fopen(PlainFilePath, "rb");
	if (fpIn == NULL) {
		return FALSE;
	}
	//打开保存密文的文件
	fpOut = fopen(CipherFilePath, "wb");
	if (fpOut == NULL) {
		fclose(fpIn);
		return FALSE;
	}
	//	strPlainFilePath.ReleaseBuffer();
	//	strCipherFilePath.ReleaseBuffer();
	//文件头，保存算法信息
	sprintf(enchead, "ALGID:%d\n", n_ID);
	fwrite(enchead, 1, 128, fpOut);
	//根据口令、密码算法生成key和iv
	EVP_BytesToKey(cipher, EVP_md5(), NULL, (const unsigned char *) password,
					sizeof(password), 1, key, iv);
	//初始化ctx
	EVP_CIPHER_CTX_init(&ctx);
	//设置密码算法、key和iv
	rv = EVP_EncryptInit_ex(&ctx, cipher, NULL, key, iv);
	if (rv != 1) {
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	//以1K为单位，循环读取原文，加密后后保存到密文文件。
	for (;;) {
		inl = fread(in, 1, 1024, fpIn);
		if (inl <= 0)//读取原文结束
			break;
		rv = EVP_EncryptUpdate(&ctx, out, &outl, in, inl);//加密
		if (rv != 1) {
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return FALSE;
		}
		fwrite(out, 1, outl, fpOut);//保存密文到文件
	}
	//加密结束
	rv = EVP_EncryptFinal_ex(&ctx, out, &outl);
	if (rv != 1) {
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	fwrite(out, 1, outl, fpOut);//保密密文到文件
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);//清除EVP加密上下文环境
	return TRUE;
}

BOOL AES_Decrypt_File(char CipherFilePath[], char PlainFilePath[], unsigned char* password) 
{
	unsigned char key[EVP_MAX_KEY_LENGTH]; //保存密钥的数组
	unsigned char iv[EVP_MAX_KEY_LENGTH]; //保存初始化向量的数组
	EVP_CIPHER_CTX ctx; //EVP加密上下文环境
	unsigned char out[1024 + EVP_MAX_KEY_LENGTH]; //保存解密后明文的缓冲区数组
	int outl;
	unsigned char in[1024]; //保存密文数据的数组
	int inl;
	const EVP_CIPHER * cipher; //加密算法
	int rv;
	FILE *fpIn; //输入文件句柄
	FILE *fpOut; //输出文件句柄
	char enchead[128] = { 0 }; //保存密文文件头的数组
	int n_ID = 0; //加密算法

	//打开待解密的密文文件
	fpIn = fopen(CipherFilePath, "rb");
	if (fpIn == NULL) {
		return FALSE;
	}
	//打开保存明文的文件
	fpOut = fopen(PlainFilePath, "wb");
	if (fpOut == NULL) {
		fclose(fpIn);
		return FALSE;
	}
	//	strPlainFilePath.ReleaseBuffer();
	//	strCipherFilePath.ReleaseBuffer();
	//读取密文文件头，获取加密算法
	fread(enchead, 1, 128, fpIn);
	sscanf(enchead, "ALGID:%d\n", &n_ID);
	if (n_ID == 0) {
		return FALSE;
	}
	//根据算法ID获得EVP_CIPHER算法
	//cipher = EVP_get_cipherbynid(n_ID);
	cipher = EVP_aes_128_ecb();
	if (cipher == NULL) {
		return FALSE;
	}
	//根据口令、密码算法生成key和iv
	EVP_BytesToKey(cipher, EVP_md5(), NULL, (const unsigned char *) password,
			sizeof(password), 1, key, iv);
	//初始化ctx
	EVP_CIPHER_CTX_init(&ctx);
	//设置解密的算法、key和iv
	rv = EVP_DecryptInit_ex(&ctx, cipher, NULL, key, iv);
	if (rv != 1) {
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	//以1K为单位，循环读取原文，解密后后保存到明文文件。
	for (;;) {
		inl = fread(in, 1, 1024, fpIn);
		if (inl <= 0)
			break;
		rv = EVP_DecryptUpdate(&ctx, out, &outl, in, inl);//解密
		if (rv != 1) {
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return FALSE;
		}
		fwrite(out, 1, outl, fpOut);//保存明文到文件
	}
	//解密结束
	rv = EVP_DecryptFinal_ex(&ctx, out, &outl);
	if (rv != 1) {
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	fwrite(out, 1, outl, fpOut);//保存明文到文件
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);//清除EVP加密上下文环境
	return TRUE;
}

