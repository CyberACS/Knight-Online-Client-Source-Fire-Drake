#include "stdafx.h"
#include "DBAgent.h"

void CUser::PPCardProcess(Packet& pkt)
{
	enum PPCardOpcodes
	{
		PPCARD_FAILED = 0,
		PPCARD_SUCCES = 1
	};

	enum PPCardErrorCodes
	{
		PPCardSuccess = 1, // Iteam has inserted successfully.Please check the letter with pressing [L].
		PPCardFailed = 2, // The serial in not existed or wrong.Please insert other serial after 5 minutes
	};

	uint8 opcode;
	int Number1;
	std::string Number2;

	pkt.SByte();
	pkt >> opcode >> Number1 >> Number2;

	std::string PPCardNumber = string_format("%d%s", Number1, Number2.c_str());

	if (PPCardNumber.length() < 19 
		&& PPCardNumber.length() > 20 
		|| (UNIXTIME - m_tPPCardTimeNormal) < PPCARD_TIME)
	{
		Packet Result(WIZ_EDIT_BOX);
		Result << uint8(4) << uint8(PPCardFailed);
		g_pMain->SendHelpDescription(this, string_format("[PPCard] : Please check the characters you entered"));
		Send(&Result);
		return;
	}

	m_tPPCardTimeNormal = UNIXTIME;
	uint16 sStatus = g_DBAgent.LoadPPCard(PPCardNumber, GetName());

	switch (sStatus)
	{
	case PPCARD_FAILED:
	{
		Packet Result(WIZ_EDIT_BOX);
		Result << uint8(4) << uint8(PPCardFailed);
		g_pMain->SendHelpDescription(this, string_format("[PPCard] : Key previously used or incorrect."));
		Send(&Result);
	}
	break;
	case PPCARD_SUCCES:
	{
		Packet Result(WIZ_EDIT_BOX);
		Result << uint8(4) << uint8(PPCardSuccess);
		g_pMain->SendHelpDescription(this, string_format("[PPCard] : Your transaction was successful."));
		Send(&Result);
	}
	break;
	}
}