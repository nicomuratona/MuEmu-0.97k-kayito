#pragma once

#define MAX_MAIN_PACKET_SIZE 2048

struct ENCDEC_HEADER
{
#pragma pack(1)
	WORD header;
	DWORD size;
#pragma pack()
};

struct ENCDEC_DATA
{
	DWORD Modulus[4];
	DWORD Key[4];
	DWORD Xor[4];
};

class CPacketManager
{
public:

	CPacketManager();

	virtual ~CPacketManager();

	void Init();

	bool LoadEncryptionKey(char* name);

	bool LoadDecryptionKey(char* name);

	bool LoadKey(char* name, WORD header, bool type);

	int Encrypt(BYTE* lpTarget, BYTE* lpSource, int size);

	int Decrypt(BYTE* lpTarget, BYTE* lpSource, int size);

	int EncryptBlock(BYTE* lpTarget, BYTE* lpSource, int size);

	int DecryptBlock(BYTE* lpTarget, BYTE* lpSource);

	int AddBits(BYTE* lpTarget, int TargetBitPos, BYTE* lpSource, int SourceBitPos, int size);

	int GetByteOfBit(int value);

	void Shift(BYTE* lpBuff, int size, int ShiftSize);

	bool AddData(BYTE* lpBuff, int size);

	bool ExtractPacket(BYTE* lpBuff);

	void XorData(int start, int end);

private:

	ENCDEC_DATA m_Encryption;

	ENCDEC_DATA m_Decryption;

	DWORD m_SaveLoadXor[4];

	BYTE m_buff[MAX_MAIN_PACKET_SIZE];

	DWORD m_size;

	BYTE m_XorFilter[32];
};

extern CPacketManager gPacketManager;