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
	unsigned char key[EVP_MAX_KEY_LENGTH]; //������Կ������
	unsigned char iv[EVP_MAX_KEY_LENGTH]; ////�����ʼ������������
	EVP_CIPHER_CTX ctx; //EVP���������Ļ���
	unsigned char out[1024]; //�������ĵĻ�����
	int outl;
	unsigned char in[1024]; //����ԭ�ĵĻ�����
	int inl;
	const EVP_CIPHER * cipher; //�����㷨
	int rv;
	FILE *fpIn; //�����ļ����
	FILE *fpOut; //����ļ����
	char enchead[128] = { 0 }; //���������ļ�ͷ������

	//�����㷨ID���EVP_CIPHER�㷨
	//cipher = EVP_get_cipherbynid(n_ID);
	cipher = EVP_aes_128_ecb();
	if (cipher == NULL) {
		return FALSE;
	}
	//�򿪴����ܵ�ԭ���ļ�
	fpIn = fopen(PlainFilePath, "rb");
	if (fpIn == NULL) {
		return FALSE;
	}
	//�򿪱������ĵ��ļ�
	fpOut = fopen(CipherFilePath, "wb");
	if (fpOut == NULL) {
		fclose(fpIn);
		return FALSE;
	}
	//	strPlainFilePath.ReleaseBuffer();
	//	strCipherFilePath.ReleaseBuffer();
	//�ļ�ͷ�������㷨��Ϣ
	sprintf(enchead, "ALGID:%d\n", n_ID);
	fwrite(enchead, 1, 128, fpOut);
	//���ݿ�������㷨����key��iv
	EVP_BytesToKey(cipher, EVP_md5(), NULL, (const unsigned char *) password,
					sizeof(password), 1, key, iv);
	//��ʼ��ctx
	EVP_CIPHER_CTX_init(&ctx);
	//���������㷨��key��iv
	rv = EVP_EncryptInit_ex(&ctx, cipher, NULL, key, iv);
	if (rv != 1) {
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	//��1KΪ��λ��ѭ����ȡԭ�ģ����ܺ�󱣴浽�����ļ���
	for (;;) {
		inl = fread(in, 1, 1024, fpIn);
		if (inl <= 0)//��ȡԭ�Ľ���
			break;
		rv = EVP_EncryptUpdate(&ctx, out, &outl, in, inl);//����
		if (rv != 1) {
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return FALSE;
		}
		fwrite(out, 1, outl, fpOut);//�������ĵ��ļ�
	}
	//���ܽ���
	rv = EVP_EncryptFinal_ex(&ctx, out, &outl);
	if (rv != 1) {
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	fwrite(out, 1, outl, fpOut);//�������ĵ��ļ�
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);//���EVP���������Ļ���
	return TRUE;
}

BOOL AES_Decrypt_File(char CipherFilePath[], char PlainFilePath[], unsigned char* password) 
{
	unsigned char key[EVP_MAX_KEY_LENGTH]; //������Կ������
	unsigned char iv[EVP_MAX_KEY_LENGTH]; //�����ʼ������������
	EVP_CIPHER_CTX ctx; //EVP���������Ļ���
	unsigned char out[1024 + EVP_MAX_KEY_LENGTH]; //������ܺ����ĵĻ���������
	int outl;
	unsigned char in[1024]; //�����������ݵ�����
	int inl;
	const EVP_CIPHER * cipher; //�����㷨
	int rv;
	FILE *fpIn; //�����ļ����
	FILE *fpOut; //����ļ����
	char enchead[128] = { 0 }; //���������ļ�ͷ������
	int n_ID = 0; //�����㷨

	//�򿪴����ܵ������ļ�
	fpIn = fopen(CipherFilePath, "rb");
	if (fpIn == NULL) {
		return FALSE;
	}
	//�򿪱������ĵ��ļ�
	fpOut = fopen(PlainFilePath, "wb");
	if (fpOut == NULL) {
		fclose(fpIn);
		return FALSE;
	}
	//	strPlainFilePath.ReleaseBuffer();
	//	strCipherFilePath.ReleaseBuffer();
	//��ȡ�����ļ�ͷ����ȡ�����㷨
	fread(enchead, 1, 128, fpIn);
	sscanf(enchead, "ALGID:%d\n", &n_ID);
	if (n_ID == 0) {
		return FALSE;
	}
	//�����㷨ID���EVP_CIPHER�㷨
	//cipher = EVP_get_cipherbynid(n_ID);
	cipher = EVP_aes_128_ecb();
	if (cipher == NULL) {
		return FALSE;
	}
	//���ݿ�������㷨����key��iv
	EVP_BytesToKey(cipher, EVP_md5(), NULL, (const unsigned char *) password,
			sizeof(password), 1, key, iv);
	//��ʼ��ctx
	EVP_CIPHER_CTX_init(&ctx);
	//���ý��ܵ��㷨��key��iv
	rv = EVP_DecryptInit_ex(&ctx, cipher, NULL, key, iv);
	if (rv != 1) {
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	//��1KΪ��λ��ѭ����ȡԭ�ģ����ܺ�󱣴浽�����ļ���
	for (;;) {
		inl = fread(in, 1, 1024, fpIn);
		if (inl <= 0)
			break;
		rv = EVP_DecryptUpdate(&ctx, out, &outl, in, inl);//����
		if (rv != 1) {
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return FALSE;
		}
		fwrite(out, 1, outl, fpOut);//�������ĵ��ļ�
	}
	//���ܽ���
	rv = EVP_DecryptFinal_ex(&ctx, out, &outl);
	if (rv != 1) {
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return FALSE;
	}
	fwrite(out, 1, outl, fpOut);//�������ĵ��ļ�
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);//���EVP���������Ļ���
	return TRUE;
}

