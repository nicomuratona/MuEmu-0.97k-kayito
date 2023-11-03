#include "stdafx.h"
#include "SerialCheck.h"

CSerialCheck gSerialCheck[MAX_OBJECT];

CSerialCheck::CSerialCheck()
{
	this->Init();
}

CSerialCheck::~CSerialCheck()
{

}

void CSerialCheck::Init()
{
	this->m_RecvSerial = (BYTE)-1;

	this->m_SendSerial = 0;
}

bool CSerialCheck::CheckSerial(BYTE serial)
{
	if ((BYTE)(this->m_RecvSerial + 1) == serial)
	{
		this->m_RecvSerial++;

		return 1;
	}

	return 0;
}

BYTE CSerialCheck::GetRecvSerial()
{
	return this->m_RecvSerial;
}

BYTE CSerialCheck::GetSendSerial()
{
	return this->m_SendSerial++;
}