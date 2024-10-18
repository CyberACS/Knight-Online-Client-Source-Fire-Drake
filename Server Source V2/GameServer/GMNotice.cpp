#include "stdafx.h"
#include "User.h"
#include <algorithm>

using std::string;
using namespace std;

void CUser::LoginNoticeHi()
{
	DateTime time;
	std::string Zaman;

	if (isGM())
		SendChat(ChatType::PUBLIC_CHAT, "[GM] : GM Tools event baslattiginizda diger eventi baslatmadan onceki eventi kapatiniz.");
	    SendChat(ChatType::PUBLIC_CHAT, "[GM] : Eventleri ust uste baslattiginizda event buga girecektir.Digerini kapatmadan baslatmayin.");
		SendChat(ChatType::PUBLIC_CHAT, "[GM] : GM Tools reloads tables duzenleme yapmadiginiz taktirde kullanmayiniz.");


}