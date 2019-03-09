#ifndef _CRC32_H_
#define _CRC32_H_

/* CRCУ�� */
unsigned int CRC16(const unsigned char* buf, unsigned int len);

/* CRCУ�飬��Դ��MySQL��CRC���ɺ��� */
unsigned int CRC32(const unsigned char* buf, unsigned int len);

class ISO_13818_CRC32
{
public:
    ISO_13818_CRC32();
    void update(const unsigned char* data, long length);
    unsigned long get_value() const;
private:
    void process_byte(unsigned char b);
    unsigned char pump_bit(unsigned char b, unsigned int bit_pos) const;
    void decode_crc(unsigned char bit);
private:
    unsigned long crc_reg;
    unsigned long poly;
};

#endif //_CRC32_H_
