#include "stdafx.h"
#include "DBAgent.h"
using namespace std;

void CUser::LoginNotice()
{
	DateTime time;
	std::string Zaman;
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice1, m_strUserID.c_str()), g_pMain->m_Baslik1);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice2, m_strUserID.c_str()), g_pMain->m_Baslik2);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice3, m_strUserID.c_str()), g_pMain->m_Baslik3);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice4, m_strUserID.c_str()), g_pMain->m_Baslik4);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice5, m_strUserID.c_str()), g_pMain->m_Baslik5);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice6, m_strUserID.c_str()), g_pMain->m_Baslik6);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice7, m_strUserID.c_str()), g_pMain->m_Baslik7);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice8, m_strUserID.c_str()), g_pMain->m_Baslik8);
	SendChatnotice(ChatType::PRIVATE_CHAT, string_format(g_pMain->m_Notice9, m_strUserID.c_str()), g_pMain->m_Baslik9);

}