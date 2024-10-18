#include "stdafx.h"
#include "DBAgent.h"

#pragma region Event Manuel Opening
#pragma region CGameServerDlg::ChaosExpansionManuelOpening()
void CGameServerDlg::ChaosExpansionManuelOpening()
{
	_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::ChaosExpansion);
	if (pPlayInfo == nullptr
		|| pPlayInfo->EventSignTime <= 0
		|| pPlayInfo->EventPlayTime <= 0)
	{
		printf("Room Event Timer Table is null or sign time : 0 or play time  : 0 \n");
		return;
	}

	if (pTempleEvent.ActiveEvent != -1
		|| pKnightRoyaleEvent.ActiveEvent != -1)
	{
		printf("event is ongoing \n");
		return;
	}

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

	TempleEventReset(TEMPLE_EVENT_CHAOS);
	TrashChaosExpansionRanking();
	pTempleEvent.isAttackable = false;
	pTempleEvent.bAllowJoin = true;
	pTempleEvent.ActiveEvent = TEMPLE_EVENT_CHAOS;
	pTempleEvent.ZoneID = ZONE_CHAOS_DUNGEON;
	pTempleEvent.StartTime = (uint32)UNIXTIME;
	pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
	m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

	TempleEventStart();
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", pPlayInfo->EventName.c_str(), nHour, nMinute, nSeconds);
}
#pragma endregion

void CGameServerDlg::DeathMatchManuelOpening()
{
	_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::DeathMatchs);
	if (pPlayInfo == nullptr
		|| pPlayInfo->EventSignTime <= 0
		|| pPlayInfo->EventPlayTime <= 0)
	{
		printf("Room Event Timer Table is null or sign time : 0 or play time  : 0 \n");
		return;
	}

	
	if (pTempleEvent.isActive)
	{
		printf("event is ongoing \n");
		return;
	}

	pBeefEvent.isActive = true;

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

	TempleEventReset(DEATHMATCH);
	TrashChaosExpansionRanking();
	pTempleEvent.isAttackable = false;
	pTempleEvent.bAllowJoin = true;
	pTempleEvent.ActiveEvent = DEATHMATCH;
	pTempleEvent.ZoneID = ZONE_DEATHMATCH;
	pTempleEvent.StartTime = (uint32)UNIXTIME;
	pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
	m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

	TempleEventStart();
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", pPlayInfo->EventName.c_str(), nHour, nMinute, nSeconds);
}
#pragma endregion

#pragma region CGameServerDlg::BorderDefenceWarManuelOpening()
void CGameServerDlg::BorderDefenceWarManuelOpening()
{
	_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::BorderDefenceWar);
	if (pPlayInfo == nullptr
		|| pPlayInfo->EventSignTime <= 0
		|| pPlayInfo->EventPlayTime <= 0)
	{
		printf("Room Event Timer Table is null or sign time : 0 or play time  : 0 \n");
		return;
	}

	if (pTempleEvent.ActiveEvent != -1
		|| pKnightRoyaleEvent.ActiveEvent != -1)
	{
		printf("event is ongoing \n");
		return;
	}

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

	TempleEventReset(TEMPLE_EVENT_BORDER_DEFENCE_WAR);
	TrashBorderDefenceWarRanking();
	pTempleEvent.isAttackable = false;
	pTempleEvent.bAllowJoin = true;
	pTempleEvent.ActiveEvent = TEMPLE_EVENT_BORDER_DEFENCE_WAR;
	pTempleEvent.ZoneID = ZONE_BORDER_DEFENSE_WAR;
	pTempleEvent.StartTime = (uint32)UNIXTIME;
	pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
	m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

	TempleEventStart();
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", pPlayInfo->EventName.c_str(), nHour, nMinute, nSeconds);
}
#pragma endregion

#pragma region CGameServerDlg::JuraidMountainManuelOpening()
void CGameServerDlg::JuraidMountainManuelOpening()
{
	_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::JuraidMountain);
	if (pPlayInfo == nullptr
		|| pPlayInfo->EventSignTime <= 0
		|| pPlayInfo->EventPlayTime <= 0)
	{
		printf("Room Event Timer Table is null or sign time : 0 or play time  : 0 \n");
		return;
	}

	if (pTempleEvent.ActiveEvent != -1
		|| pKnightRoyaleEvent.ActiveEvent != -1)
	{
		printf("event is ongoing \n");
		return;
	}

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

	TempleEventReset(TEMPLE_EVENT_JURAD_MOUNTAIN);
	pTempleEvent.isAttackable = false;
	pTempleEvent.bAllowJoin = true;
	pTempleEvent.ActiveEvent = TEMPLE_EVENT_JURAD_MOUNTAIN;
	pTempleEvent.ZoneID = ZONE_JURAID_MOUNTAIN;
	pTempleEvent.StartTime = (uint32)UNIXTIME;
	pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
	pEventBridge.isBridgeSystemActive = true;
	pEventBridge.isBridgeSystemStartMinutes = (uint32)UNIXTIME;
	m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

	TempleEventStart();
	SendAnnouncement("Gate Keeper of Islant mountain: 70 above level Warriors!! I Will let you enter the cave now.");
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", pPlayInfo->EventName.c_str(), nHour, nMinute, nSeconds);
}
#pragma endregion

#pragma region CGameServerDlg::KnightRoyaleManuelOpening()
void CGameServerDlg::KnightRoyaleManuelOpening()
{
	_ROYALE_EVENT_PLAY_TIMER* pPlayInfo = m_RoyaleEventPlayTimerArray.GetData(EventLocalID::KnightRoyale);
	if (pPlayInfo == nullptr
		|| pPlayInfo->EventSignTime <= 0)
	{
		printf("Royale Event Timer Table is null or sign time : 0 \n");
		return;
	}

	_KNIGHT_ROYALE_INFO* pKnightRoyaleInfo = g_pMain->m_KnightRoyaleRoomInfo.GetData(1);
	if (pKnightRoyaleInfo == nullptr)
	{
		printf("Royale Info Array is null \n");
		return;
	}

	if (pTempleEvent.ActiveEvent != -1
		|| pKnightRoyaleEvent.ActiveEvent != -1)
	{
		printf("event is ongoing \n");
		return;
	}

	m_nKnightRoyaleEventRemainSeconds = pPlayInfo->EventSignTime * MINUTE;
	pKnightRoyaleEvent.ActiveEvent = ZONE_KNIGHT_ROYALE;
	pKnightRoyaleEvent.StartTime = (uint32)UNIXTIME;
	pKnightRoyaleEvent.CloseTime = (uint32)UNIXTIME + (pPlayInfo->EventManuelCloseTimer * MINUTE);
	pKnightRoyaleEvent.AllowJoin = true;
	KickOutZoneUsers(ZONE_KNIGHT_ROYALE, ZONE_MORADON);
	KnightRoyaleStart();

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", pPlayInfo->EventName.c_str(), nHour, nMinute, nSeconds);
}
#pragma endregion

void CGameServerDlg::BeefEventManuelOpening()
{
	_BEEF_EVENT_PLAY_TIMER* pPlayInfo = m_BeefEventPlayTimerArray.GetData(EventLocalID::BeefEvent);
	if (pPlayInfo == nullptr
		|| pPlayInfo->MonumentTime <= 0
		|| pPlayInfo->LoserSignTime <= 0
		|| pPlayInfo->FarmingTime <= 0)
	{
		printf("Beef Event Timer Table is null or monument time : 0 or loser time : 0 farming time : 0 \n");
		return;
	}

	if (pBeefEvent.isActive)
	{
		printf("event is ongoing \n");
		return;
	}

	pBeefEvent.isActive = true;
	pBeefEvent.isAttackable = true;
	pBeefEvent.BeefMonumentStartTime = (uint32)UNIXTIME;
	pBeefEvent.BeefSendTime = pPlayInfo->MonumentTime * MINUTE;
	pBeefEvent.isMonumentDead = false;
	pBeefEvent.WictoryNation = 0;
	BeefEventUpdateTime();
	BeefEventSendNotice(1);

	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;
	printf("[Manuel Event] call for the %s Event is started at %02d:%02d:%02d \n", "Beef Event", nHour, nMinute, nSeconds);
}
#pragma endregion

#pragma region CGameServerDlg::EventMainTimer()
void CGameServerDlg::EventMainTimer()
{
	tournamentEventTimer();
	VirtualEventTimer();
	SingleLunarEventTimer();
	SingleOtherEventTimer();
	CastleSiegeWarMainTimer();
}
#pragma endregion

#pragma region CGameServerDlg::VirtualEventLocalTimer()
void CGameServerDlg::VirtualEventLocalTimer()
{
	if (m_nVirtualEventRoomRemainSeconds > 0)
		m_nVirtualEventRoomRemainSeconds--;

	if (m_nKnightRoyaleEventRemainSeconds > 0)
		m_nKnightRoyaleEventRemainSeconds--;
}
#pragma endregion

#pragma region CGameServerDlg::VirtualEventLocalTimer()
void CGameServerDlg::SingleOtherEventLocalTimer()
{
	if (m_nUnderTheCastleEventTime > 0)
		m_nUnderTheCastleEventTime--;

	if (pBeefEvent.BeefSendTime > 0)
		pBeefEvent.BeefSendTime--;
}
#pragma endregion

/**
* @brief	Chaos Expansion - Border Defence War - Juraid Mountain
*/
#pragma region CGameServerDlg::VirtualEventTimer()
void CGameServerDlg::VirtualEventTimer()
{
	uint32 nWeekDay = g_localTime.tm_wday;
	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	VirtualEventLocalTimer();

	if (pTempleEvent.ActiveEvent == -1 && pKnightRoyaleEvent.ActiveEvent == -1)
	{
		if (m_EventScheduleArray.GetSize() != 0)
		{
			for (int EventIDi = 1; EventIDi <= m_EventScheduleArray.GetSize(); EventIDi++)
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventIDi);
				if (pEventStatus == nullptr ||
					pEventStatus->EventStatus == 0 ||
					pEventStatus->EventType != EventType::VirtualRoom)
					continue;

				bool VirtualEventisOkey(pEventStatus->EventLocalID == EventLocalID::BorderDefenceWar
					|| pEventStatus->EventLocalID == EventLocalID::ChaosExpansion
					|| pEventStatus->EventLocalID == EventLocalID::JuraidMountain
					|| pEventStatus->EventLocalID == EventLocalID::DeathMatchs
					|| pEventStatus->EventLocalID == EventLocalID::KnightRoyale);

				if (!VirtualEventisOkey)
					continue;

				std::list<std::string> nInGameEvent = StrSplit(pEventStatus->StartDays, ",");
				uint8 nInGameEventDaysSize = (uint8)nInGameEvent.size();
				if (nInGameEventDaysSize > 0)
				{
					uint8 nInGameDays = 0;
					for (int EventDayi = 0; EventDayi < nInGameEventDaysSize; EventDayi++)
					{
						nInGameDays = atoi(nInGameEvent.front().c_str());
						if (nInGameDays == nWeekDay)
						{
							for (int EventTimei = 0; EventTimei < EVENT_START_TIMES; EventTimei++)
							{
								if (pEventStatus->TimeActive[EventTimei] != 1
									|| pEventStatus->EventStartMinutes[EventTimei] == 99)
									continue;

								if (nHour == pEventStatus->EventStartHour[EventTimei]
									&& nMinute == pEventStatus->EventStartMinutes[EventTimei]
									&& nSeconds == 0)
								{
									if (pEventStatus->EventLocalID == EventLocalID::BorderDefenceWar)
									{
										_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::BorderDefenceWar);
										if (pPlayInfo == nullptr)
										{
											printf("Event Alert _EVENT_SCHEDULE_PLAY_TIMER is NULL Name: %s \n", pEventStatus->EventName.c_str());
											continue;
										}

										if (pPlayInfo->EventSignTime <= 0
											|| pPlayInfo->EventPlayTime <= 0)
											continue;

										uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

										TempleEventReset(TEMPLE_EVENT_BORDER_DEFENCE_WAR);
										TrashBorderDefenceWarRanking();
										pTempleEvent.isAttackable = true;
										pTempleEvent.bAllowJoin = true;
										pTempleEvent.ActiveEvent = TEMPLE_EVENT_BORDER_DEFENCE_WAR;
										pTempleEvent.ZoneID = (int8)pEventStatus->EventZoneID;
										pTempleEvent.StartTime = (uint32)UNIXTIME;
										pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
										m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

										TempleEventStart();
										printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
									}
									else if (pEventStatus->EventLocalID == EventLocalID::ChaosExpansion)
									{
										_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::ChaosExpansion);
										if (pPlayInfo == nullptr)
										{
											printf("Event Alert _EVENT_SCHEDULE_PLAY_TIMER is NULL Name: %s \n", pEventStatus->EventName.c_str());
											continue;
										}

										if (pPlayInfo->EventSignTime <= 0
											|| pPlayInfo->EventPlayTime <= 0)
											continue;

										uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

										TempleEventReset(TEMPLE_EVENT_CHAOS);
										TrashChaosExpansionRanking();
										pTempleEvent.isAttackable = true;
										pTempleEvent.bAllowJoin = true;
										pTempleEvent.ActiveEvent = TEMPLE_EVENT_CHAOS;
										pTempleEvent.ZoneID = (int8)pEventStatus->EventZoneID;
										pTempleEvent.StartTime = (uint32)UNIXTIME;
										pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
										m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

										TempleEventStart();
										printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
									}
									else if (pEventStatus->EventLocalID == EventLocalID::JuraidMountain)
									{
										_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::JuraidMountain);
										if (pPlayInfo == nullptr)
										{
											printf("Event Alert _EVENT_SCHEDULE_PLAY_TIMER is NULL Name: %s \n", pEventStatus->EventName.c_str());
											continue;
										}

										if (pPlayInfo->EventSignTime <= 0
											|| pPlayInfo->EventPlayTime <= 0)
											continue;

										uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

										TempleEventReset(TEMPLE_EVENT_JURAD_MOUNTAIN);
										pTempleEvent.isAttackable = true;
										pTempleEvent.bAllowJoin = true;
										pTempleEvent.ActiveEvent = TEMPLE_EVENT_JURAD_MOUNTAIN;
										pTempleEvent.ZoneID = (int8)pEventStatus->EventZoneID;
										pTempleEvent.StartTime = (uint32)UNIXTIME;
										pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
										m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

										TempleEventStart();
										pEventBridge.isBridgeSystemActive = true;
										pEventBridge.isBridgeSystemStartMinutes = (uint32)UNIXTIME;

										SendAnnouncement("Gate Keeper of Islant mountain: 70 above level Warriors!! I Will let you enter the cave now.");
										printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
									}
									else if (pEventStatus->EventLocalID == EventLocalID::KnightRoyale)
									{
										_ROYALE_EVENT_PLAY_TIMER* pPlayInfo = m_RoyaleEventPlayTimerArray.GetData(EventLocalID::KnightRoyale);
										if (pPlayInfo == nullptr)
										{
											printf("Event Alert EVENT_ROYALE_PLAY_TIMER is NULL Name: %s \n", pEventStatus->EventName.c_str());
											continue;
										}

										_KNIGHT_ROYALE_INFO* pKnightRoyaleInfo = g_pMain->m_KnightRoyaleRoomInfo.GetData(1);
										if (pKnightRoyaleInfo != nullptr)
										{
											m_nKnightRoyaleEventRemainSeconds = pPlayInfo->EventSignTime * MINUTE;
											pKnightRoyaleEvent.ActiveEvent = ZONE_KNIGHT_ROYALE;
											pKnightRoyaleEvent.StartTime = (uint32)UNIXTIME;
											pKnightRoyaleEvent.CloseTime = (uint32)UNIXTIME + (pPlayInfo->EventManuelCloseTimer * MINUTE);
											pKnightRoyaleEvent.AllowJoin = true;
											KickOutZoneUsers(ZONE_KNIGHT_ROYALE, ZONE_MORADON);

											KnightRoyaleStart();
											printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
										}
									}
									else if (pEventStatus->EventLocalID == EventLocalID::DeathMatchs)
									{
										_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::DeathMatchs);
										if (pPlayInfo == nullptr)
										{
											printf("Event Alert _EVENT_SCHEDULE_PLAY_TIMER is NULL Name: %s \n", pEventStatus->EventName.c_str());
											continue;
										}

										if (pPlayInfo->EventSignTime <= 0
											|| pPlayInfo->EventPlayTime <= 0)
											continue;
										
										uint32 CloseTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime) * MINUTE;

										TempleEventReset(DEATHMATCH);
										TrashChaosExpansionRanking();
										pTempleEvent.isAttackable = true;
										pTempleEvent.bAllowJoin = true;
										pTempleEvent.ActiveEvent = DEATHMATCH;
										pTempleEvent.ZoneID = (int8)pEventStatus->EventZoneID;
										pTempleEvent.StartTime = (uint32)UNIXTIME;
										pTempleEvent.ClosedTime = (uint32)UNIXTIME + CloseTime;
										m_nVirtualEventRoomRemainSeconds = pPlayInfo->EventSignTime * MINUTE;

										TempleEventStart();
										g_pMain->SendFormattedNotice("DeathMatch Event Aktif Edildi | Event Suresi : %d Dakika", Nation::ALL, g_pMain->DeathMatchEventFinishTime);
										printf("DeathMatch Event is started at %02d:%02d:%02d \n", nHour, nMinute, nSeconds);
											
										
									}
								}
							}
						}
						nInGameEvent.pop_front();
					}
				}
			}
		}
	}
	else if (pTempleEvent.ActiveEvent != -1 || pKnightRoyaleEvent.ActiveEvent != -1)
	{
		if (pTempleEvent.ActiveEvent != -1)
		{
			switch (pTempleEvent.ActiveEvent)
			{
			case TEMPLE_EVENT_CHAOS:
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventLocalID::ChaosExpansion);
				if (pEventStatus == nullptr
					|| pEventStatus->EventStatus == 0
					|| pEventStatus->EventLocalID != EventLocalID::ChaosExpansion
					|| pEventStatus->EventType != EventType::VirtualRoom)
				{
					TempleEventReset(TEMPLE_EVENT_CHAOS);
					return;
				}

				_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::ChaosExpansion);
				if (pPlayInfo == nullptr
					|| pPlayInfo->EventSignTime <= 0
					|| pPlayInfo->EventPlayTime <= 0)
				{
					TempleEventReset(TEMPLE_EVENT_CHAOS);
					return;
				}

				//Event Start
				if (!pTempleEvent.EventTimerStartControl && !pTempleEvent.isActive)
				{
					uint32 EventSignFinishTime = (pPlayInfo->EventSignTime * MINUTE);

					if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventSignFinishTime))
					{
						pTempleEvent.bAllowJoin = false;
						m_nVirtualEventRoomRemainSeconds = 0;
						pTempleEvent.LastEventRoom = 1;
						pTempleEvent.isActive = true;
						TempleEventStart(); // send user that event being started. remaning = 0;
						TempleEventManageRoom();
						TempleEventTeleportUsers();
						TempleEventScreen(TEMPLE_EVENT_CHAOS, pPlayInfo->EventPlayTime * MINUTE);
						pTempleEvent.EventTimerStartControl = true;
					}
				}

				if (pTempleEvent.isActive)
				{
					if (!pTempleEvent.EventCloseMainControl)
					{
						//Event Attack Start
						if (!pTempleEvent.EventTimerAttackOpenControl && !pTempleEvent.isAttackable)
						{
							uint32 EventAttackOpenTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackOpenTime);
							EventAttackOpenTime = EventAttackOpenTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackOpenTime))
							{
								pTempleEvent.isAttackable = true;
								pTempleEvent.EventTimerAttackOpenControl = true;
							}
						}

						//Event Attack Close
						if (!pTempleEvent.EventTimerAttackCloseControl && pTempleEvent.isAttackable)
						{
							uint32 EventAttackStopTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackCloseTime);
							EventAttackStopTime = EventAttackStopTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackStopTime))
							{
								pTempleEvent.isAttackable = false;
								pTempleEvent.EventTimerAttackCloseControl = true;
							}
						}

						//Event Finish Control
						if (pTempleEvent.EventTimerStartControl)
						{
							uint32 EventFinishTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime);
							EventFinishTime = EventFinishTime * MINUTE;

							if (!pTempleEvent.EventTimerFinishControl)
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
								{
									TempleEventSendWinnerScreen();
									pTempleEvent.EventTimerFinishControl = true;
								}
							}
							else
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
									TempleEventRoomClose();
							}
						}
					}

					if (pTempleEvent.EventCloseMainControl)
						TempleEventRoomClose();

					//Event Reset
					if (!pTempleEvent.EventTimerResetControl)
					{
						uint32 EventResetTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime + 1);
						EventResetTime = EventResetTime * MINUTE;

						if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventResetTime))
						{
							TempleEventFinish();
							TempleEventReset(TEMPLE_EVENT_CHAOS);
							pTempleEvent.EventTimerResetControl = true;
						}
					}
				}
			}
			break;
			case DEATHMATCH:
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventLocalID::DeathMatchs);
				if (pEventStatus == nullptr
					|| pEventStatus->EventStatus == 0
					|| pEventStatus->EventLocalID != EventLocalID::DeathMatchs
					|| pEventStatus->EventType != EventType::VirtualRoom)
				{
					TempleEventReset(DEATHMATCH);
					return;
				}

				_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::DeathMatchs);
				if (pPlayInfo == nullptr
					|| pPlayInfo->EventSignTime <= 0
					|| pPlayInfo->EventPlayTime <= 0)
				{
					TempleEventReset(DEATHMATCH);
					return;
				}

				//Event Start
				if (!pTempleEvent.EventTimerStartControl && !pTempleEvent.isActive)
				{
					uint32 EventSignFinishTime = (pPlayInfo->EventSignTime * MINUTE);

					if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventSignFinishTime))
					{
						pTempleEvent.bAllowJoin = false;
						m_nVirtualEventRoomRemainSeconds = 0;
						pTempleEvent.LastEventRoom = 1;
						/*if (pTempleEvent.isActive) {
							TempleEventStart(); // send user that event being started. remaning = 0;
						}*/
						pTempleEvent.isActive = true;
						TempleEventManageRoom();
						TempleEventTeleportUsers();
						g_pMain->DeathMatchPrepare();
						pTempleEvent.EventTimerStartControl = true;

						
					}
				}

				if (pTempleEvent.isActive)
				{
					if (!pTempleEvent.EventCloseMainControl)
					{
						//Event Attack Start
						if (!pTempleEvent.EventTimerAttackOpenControl && !pTempleEvent.isAttackable)
						{
							uint32 EventAttackOpenTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackOpenTime);
							EventAttackOpenTime = EventAttackOpenTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackOpenTime))
							{
								pTempleEvent.isAttackable = true;
								pTempleEvent.EventTimerAttackOpenControl = true;

								
							}
						}

						//Event Attack Close
						if (!pTempleEvent.EventTimerAttackCloseControl && pTempleEvent.isAttackable)
						{
							uint32 EventAttackStopTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackCloseTime);
							EventAttackStopTime = EventAttackStopTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackStopTime))
							{
								pTempleEvent.isAttackable = false;
								pTempleEvent.EventTimerAttackCloseControl = true;
							}
						}

						//Event Finish Control
						if (pTempleEvent.EventTimerStartControl)
						{
							uint32 EventFinishTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime);
							EventFinishTime = EventFinishTime * MINUTE;

							if (!pTempleEvent.EventTimerFinishControl)
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
								{
									TempleEventSendWinnerScreen();
									pTempleEvent.EventTimerFinishControl = true;
								}
							}
							else
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
									TempleEventRoomClose();
							}
						}
					}

					if (pTempleEvent.EventCloseMainControl)
						TempleEventRoomClose();

					//Event Reset
					if (!pTempleEvent.EventTimerResetControl)
					{
						uint32 EventResetTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime + 1);
						EventResetTime = EventResetTime * MINUTE;

						if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventResetTime))
						{
							TempleEventFinish();
							TempleEventReset(DEATHMATCH);
							pTempleEvent.EventTimerResetControl = true;
						}
					}
				}
			}
			break;
			case TEMPLE_EVENT_BORDER_DEFENCE_WAR:
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventLocalID::BorderDefenceWar);
				if (pEventStatus == nullptr
					|| pEventStatus->EventStatus == 0
					|| pEventStatus->EventLocalID != EventLocalID::BorderDefenceWar
					|| pEventStatus->EventType != EventType::VirtualRoom)
				{
					TempleEventReset(TEMPLE_EVENT_BORDER_DEFENCE_WAR);
					return;
				}

				_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::BorderDefenceWar);
				if (pPlayInfo == nullptr
					|| pPlayInfo->EventSignTime <= 0
					|| pPlayInfo->EventPlayTime <= 0)
				{
					TempleEventReset(TEMPLE_EVENT_BORDER_DEFENCE_WAR);
					return;
				}

				//Event Start
				if (!pTempleEvent.EventTimerStartControl && !pTempleEvent.isActive)
				{
					uint32 EventSignFinishTime = (pPlayInfo->EventSignTime * MINUTE);
					if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventSignFinishTime))
					{
						m_nVirtualEventRoomRemainSeconds = 0;
						pTempleEvent.LastEventRoom = 1;
						pTempleEvent.isActive = true;
						pTempleEvent.bAllowJoin = false;
						TempleEventStart(); // send user that event being started. remaning = 0;
						TempleEventManageRoom();
						TempleEventTeleportUsers();
						TempleEventCreateParties();
						TempleEventScreen(TEMPLE_EVENT_BORDER_DEFENCE_WAR, pPlayInfo->EventPlayTime * MINUTE);
						pTempleEvent.EventTimerStartControl = true;
					}
				}

				if (pTempleEvent.isActive)
				{
					if (!pTempleEvent.EventCloseMainControl)
					{
						//Event Attack Start
						if (!pTempleEvent.EventTimerAttackOpenControl && !pTempleEvent.isAttackable)
						{
							uint32 EventAttackOpenTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackOpenTime);
							EventAttackOpenTime = EventAttackOpenTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackOpenTime))
							{
								pTempleEvent.isAttackable = true;
								pTempleEvent.EventTimerAttackOpenControl = true;
							}
						}

						//Event Attack Close
						if (!pTempleEvent.EventTimerAttackCloseControl && pTempleEvent.isAttackable)
						{
							uint32 EventAttackStopTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackCloseTime);
							EventAttackStopTime = EventAttackStopTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackStopTime))
							{
								pTempleEvent.isAttackable = false;
								pTempleEvent.EventTimerAttackCloseControl = true;
							}
						}

						//Event Finish Control
						if (pTempleEvent.EventTimerStartControl)
						{
							uint32 EventFinishTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime);
							EventFinishTime = EventFinishTime * MINUTE;

							if (!pTempleEvent.EventTimerFinishControl)
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
								{
									TempleEventSendWinnerScreen();
									pTempleEvent.EventTimerFinishControl = true;
								}
							}
							else
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
									TempleEventRoomClose();
							}
						}
					}

					if (pTempleEvent.EventCloseMainControl)
						TempleEventRoomClose();

					//Event Reset
					if (!pTempleEvent.EventTimerResetControl)
					{
						uint32 EventResetTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime + 1);
						EventResetTime = EventResetTime * MINUTE;

						if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventResetTime))
						{
							TempleEventFinish();
							TempleEventReset(TEMPLE_EVENT_BORDER_DEFENCE_WAR);
							pTempleEvent.EventTimerResetControl = true;
						}
					}
				}
			}
			break;
			case TEMPLE_EVENT_JURAD_MOUNTAIN:
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventLocalID::JuraidMountain);
				if (pEventStatus == nullptr
					|| pEventStatus->EventStatus == 0
					|| pEventStatus->EventLocalID != EventLocalID::JuraidMountain
					|| pEventStatus->EventType != EventType::VirtualRoom)
				{
					TempleEventReset(TEMPLE_EVENT_JURAD_MOUNTAIN);
					return;
				}

				_ROOM_EVENT_PLAY_TIMER* pPlayInfo = m_RoomEventPlayTimerArray.GetData(EventLocalID::JuraidMountain);
				if (pPlayInfo == nullptr
					|| pPlayInfo->EventSignTime <= 0
					|| pPlayInfo->EventPlayTime <= 0)
				{
					TempleEventReset(TEMPLE_EVENT_JURAD_MOUNTAIN);
					return;
				}

				//Event Start
				if (!pTempleEvent.EventTimerStartControl && !pTempleEvent.isActive)
				{
					uint32 EventSignFinishTime = (pPlayInfo->EventSignTime * MINUTE);

					if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventSignFinishTime))
					{
						m_nVirtualEventRoomRemainSeconds = 0;
						pTempleEvent.LastEventRoom = 1;
						pTempleEvent.isActive = true;
						pTempleEvent.bAllowJoin = false;
						TempleEventManageRoom();
						TempleEventTeleportUsers();
						TempleEventCreateParties();
						TempleEventScreen(TEMPLE_EVENT_JURAD_MOUNTAIN, pPlayInfo->EventPlayTime * MINUTE);
						pTempleEvent.EventTimerStartControl = true;
					}
				}

				if (pTempleEvent.isActive)
				{
					if (!pTempleEvent.EventCloseMainControl)
					{
						//Event Attack Start
						if (!pTempleEvent.EventTimerAttackOpenControl && !pTempleEvent.isAttackable)
						{
							uint32 EventAttackOpenTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackOpenTime);
							EventAttackOpenTime = EventAttackOpenTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackOpenTime))
							{
								pTempleEvent.isAttackable = true;
								pTempleEvent.EventTimerAttackOpenControl = true;
							}
						}

						//Event Attack Close
						if (!pTempleEvent.EventTimerAttackCloseControl && pTempleEvent.isAttackable)
						{
							uint32 EventAttackStopTime = (pPlayInfo->EventSignTime + pPlayInfo->EventAttackCloseTime);
							EventAttackStopTime = EventAttackStopTime * MINUTE;

							if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventAttackStopTime))
							{
								pTempleEvent.isAttackable = false;
								pTempleEvent.EventTimerAttackCloseControl = true;
							}
						}

						//Bridge System
						if (pEventBridge.isBridgeSystemActive)
						{
							if (!pEventBridge.isBridgeTimerControl1)
							{
								if ((uint32)UNIXTIME >= (pEventBridge.isBridgeSystemStartMinutes + 1200))
								{
									TempleEventBridgeCheck(0);
									pEventBridge.isBridgeTimerControl1 = true;
								}
							}

							if (!pEventBridge.isBridgeTimerControl2)
							{
								if ((uint32)UNIXTIME >= (pEventBridge.isBridgeSystemStartMinutes + 1800))
								{
									TempleEventBridgeCheck(1);
									pEventBridge.isBridgeTimerControl2 = true;
								}
							}

							if (!pEventBridge.isBridgeTimerControl3)
							{
								if ((uint32)UNIXTIME >= (pEventBridge.isBridgeSystemStartMinutes + 2400))
								{
									TempleEventBridgeCheck(2);
									pEventBridge.isBridgeTimerControl3 = true;
								}
							}
						}

						//Event Finish Control
						if (pTempleEvent.EventTimerStartControl)
						{
							uint32 EventFinishTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime);
							EventFinishTime = EventFinishTime * MINUTE;

							if (!pTempleEvent.EventTimerFinishControl)
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
								{
									TempleEventSendWinnerScreen();
									pTempleEvent.EventTimerFinishControl = true;
								}
							}
							else
							{
								if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventFinishTime))
									TempleEventRoomClose();
							}
						}
					}

					if (pTempleEvent.EventCloseMainControl)
						TempleEventRoomClose();

					//Event Reset
					if (!pTempleEvent.EventTimerResetControl)
					{
						uint32 EventResetTime = (pPlayInfo->EventSignTime + pPlayInfo->EventPlayTime + 1);
						EventResetTime = EventResetTime * MINUTE;

						if ((uint32)UNIXTIME >= (pTempleEvent.StartTime + EventResetTime))
						{
							TempleEventFinish();
							TempleEventReset(TEMPLE_EVENT_JURAD_MOUNTAIN);
							pTempleEvent.EventTimerResetControl = true;
						}
					}
				}
			}
			break;
			}
		}
		else if (pKnightRoyaleEvent.ActiveEvent != -1)
		{
			_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventLocalID::KnightRoyale);
			if (pEventStatus == nullptr
				|| pEventStatus->EventStatus == 0
				|| pEventStatus->EventLocalID != EventLocalID::KnightRoyale
				|| pEventStatus->EventType != EventType::VirtualRoom)
			{
				KnightRoyaleTotalReset();
				return;
			}

			_ROYALE_EVENT_PLAY_TIMER* pPlayInfo = m_RoyaleEventPlayTimerArray.GetData(EventLocalID::KnightRoyale);
			if (pPlayInfo == nullptr
				|| pPlayInfo->EventSignTime <= 0)
			{
				KnightRoyaleTotalReset();
				return;
			}

			_KNIGHT_ROYALE_INFO* pKnightRoyaleInfo = g_pMain->m_KnightRoyaleRoomInfo.GetData(1);
			if (pKnightRoyaleInfo == nullptr)
			{
				KnightRoyaleTotalReset();
				return;
			}

			//Event Start
			if (!pKnightRoyaleEvent.EventTimerStartControl && !pKnightRoyaleEvent.isActive)
			{
				uint32 EventSignFinishTime = (pPlayInfo->EventSignTime * MINUTE);
				if ((uint32)UNIXTIME >= (pKnightRoyaleEvent.StartTime + EventSignFinishTime))
				{
					m_nKnightRoyaleEventRemainSeconds = 0;
					pKnightRoyaleEvent.AllowJoin = false;
					pKnightRoyaleEvent.isActive = true;
					KnightRoyaleEventManage();
					KnightRoyaleUserTeleport();
					g_pMain->KnightRoyaleTreasureChestSpawn();
					pKnightRoyaleEvent.EventTimerStartControl = true;
				}
			}

			if (pKnightRoyaleEvent.isActive)
			{
				if (!pKnightRoyaleEvent.EventCloseMainControl)
				{
					if ((uint32)UNIXTIME >= (pKnightRoyaleEvent.CloseTime))
					{
						KnightRoyaleClose();
						KnightRoyaleTotalReset();
						pKnightRoyaleEvent.EventCloseMainControl = true;
					}
				}
			}
		}
	}
}
#pragma endregion

/**
* @brief Lunar War Event
*/
#pragma region CGameServerDlg::SingleLunarEventTimer()
void CGameServerDlg::SingleLunarEventTimer()
{
	uint32 nWeekDay = g_localTime.tm_wday;
	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	if (!isWarOpen() && !pCswEvent.Started)
	{
		if (m_EventScheduleArray.GetSize() != 0)
		{
			for (int EventIDi = 1; EventIDi <= m_EventScheduleArray.GetSize(); EventIDi++)
			{
				_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventIDi);
				if (pEventStatus == nullptr ||
					pEventStatus->EventStatus == 0 ||
					pEventStatus->EventType != EventType::LunarWar)
					continue;

				bool LunarEventisOkey(pEventStatus->EventLocalID == EventLocalID::AlseidsPrairie
					|| pEventStatus->EventLocalID == EventLocalID::NapiesGorge
					|| pEventStatus->EventLocalID == EventLocalID::NereidsIsland
					|| pEventStatus->EventLocalID == EventLocalID::NiedsTriangle
					|| pEventStatus->EventLocalID == EventLocalID::Oreads
					|| pEventStatus->EventLocalID == EventLocalID::SnowWar
					|| pEventStatus->EventLocalID == EventLocalID::TestZone
					|| pEventStatus->EventLocalID == EventLocalID::Zipang
					|| pEventStatus->EventLocalID == EventLocalID::CastleSiegeWar);

				if (!LunarEventisOkey)
					continue;

				std::list<std::string> nInGameEvent = StrSplit(pEventStatus->StartDays, ",");
				uint8 nInGameEventDaysSize = (uint8)nInGameEvent.size();
				if (nInGameEventDaysSize > 0)
				{
					uint8 nInGameDays = 0;
					for (int EventDayi = 0; EventDayi < nInGameEventDaysSize; EventDayi++)
					{
						nInGameDays = atoi(nInGameEvent.front().c_str());
						if (nInGameDays == nWeekDay)
						{
							for (int EventTimei = 0; EventTimei < EVENT_START_TIMES; EventTimei++)
							{
								if (pEventStatus->TimeActive[EventTimei] != 1
									|| pEventStatus->EventStartMinutes[EventTimei] == 99)
									continue;

								for (int NoticeCount = 1; NoticeCount <= 5; NoticeCount++)
								{
									if (pEventStatus->EventType == EventType::LunarWar
										&& pEventStatus->EventLocalID != EventLocalID::CastleSiegeWar)
									{
										if ((pEventStatus->EventStartMinutes[EventTimei] - NoticeCount) < 0)
										{
											int16 sNoticeMinutes = pEventStatus->EventStartMinutes[EventTimei] - NoticeCount;
											if (sNoticeMinutes < 0)
											{
												sNoticeMinutes = 60 + sNoticeMinutes;
												int16 sNoticeHour = pEventStatus->EventStartHour[EventTimei] - 1;
												if (sNoticeHour < 0)
													sNoticeHour = 23;

												if (sNoticeHour == nHour && sNoticeMinutes == nMinute && nSeconds == 0)
												{
													printf("War Open Notice (1) left %d Minutes \n", NoticeCount);
													m_byBattleNoticeTime = NoticeCount;
													Announcement(IDS_PRE_BATTLE_ANNOUNCEMENT);
												}
											}
										}
										else
										{
											if (pEventStatus->EventStartHour[EventTimei] == nHour
												&& (pEventStatus->EventStartMinutes[EventTimei] - NoticeCount == nMinute)
												&& nSeconds == 0)
											{
												printf("War Open Notice (2) left %d Minutes \n", NoticeCount);
												m_byBattleNoticeTime = NoticeCount;
												Announcement(IDS_PRE_BATTLE_ANNOUNCEMENT);
											}
										}
									}
								}

								if (nHour == pEventStatus->EventStartHour[EventTimei]
									&& nMinute == pEventStatus->EventStartMinutes[EventTimei]
									&& nSeconds == 0)
								{
									if (pEventStatus->EventType == EventType::LunarWar)
									{
										if (pEventStatus->EventLocalID == EventLocalID::CastleSiegeWar)
										{
											_CSW_EVENT_PLAY_TIMER* pPlayTimer = m_CswEventPlayTimerArray.GetData(EventLocalID::CastleSiegeWar);
											if (pPlayTimer == nullptr
												|| pPlayTimer->BarrackCreatedTime <= 0
												|| pPlayTimer->DeathMatchTime <= 0
												|| pPlayTimer->PreparingTime <= 0
												|| pPlayTimer->CastellanTime <= 0)
												continue;

											printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
											CastleSiegeDeathmatchBarrackCreated();
										}
										else
										{
											printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);
											BattleZoneOpen(BATTLEZONE_OPEN, (uint8)pEventStatus->EventZoneID - ZONE_BATTLE_BASE);
										}
									}
								}
							}
						}
						nInGameEvent.pop_front();
					}
				}
			}
		}
	}
	else if (m_byBattleOpen == NATION_BATTLE && !pCswEvent.Started)
	{
		BattleZoneCurrentUsers();

		int32 WarElapsedTime = int32(UNIXTIME) - m_byBattleOpenedTime;
		m_byBattleRemainingTime = m_byBattleTime - WarElapsedTime;
		uint8 nBattleZone = g_pMain->m_byBattleZone + ZONE_BATTLE_BASE;

		if ((m_byNereidsIslandRemainingTime != 0 && nBattleZone == ZONE_BATTLE4)
			|| (m_byNereidsIslandRemainingTime != 0 && nBattleZone == ZONE_BATTLE5))
			m_byNereidsIslandRemainingTime--;

		if (m_bVictory == 0)
		{
			if (m_bCommanderSelected == false && WarElapsedTime >= (m_byBattleTime / 24)) // Select captain
				BattleZoneSelectCommanders();
			else if ((WarElapsedTime == (m_byBattleTime / 8)
				&& nBattleZone == ZONE_BATTLE4)
				|| (WarElapsedTime == (m_byBattleTime / 8)
					&& nBattleZone == ZONE_BATTLE5)) // War half time + 15 minutes for Nereid's , Zipang Island.
			{
				if ((nBattleZone == ZONE_BATTLE4)
					|| (nBattleZone == ZONE_BATTLE5)) // Nereid's Island
				{
					if (m_sKarusMonuments >= 7 && m_sElmoMonuments == 0)
						BattleZoneResult(KARUS);
					else if (m_sKarusMonuments == 0 && m_sElmoMonuments >= 7)
						BattleZoneResult(ELMORAD);
				}
			}
			else if ((m_byNereidsIslandRemainingTime == 0 && nBattleZone == ZONE_BATTLE4)
				|| (m_byNereidsIslandRemainingTime == 0 && nBattleZone == ZONE_BATTLE5))
			{
				if (m_sKarusMonuments >= 7 && m_sElmoMonuments == 0)
					BattleZoneResult(KARUS);
				else if (m_sKarusMonuments == 0 && m_sElmoMonuments >= 7)
					BattleZoneResult(ELMORAD);
			}
			else if (WarElapsedTime == (m_byBattleTime / 2)) // War half time.
			{
				if (nBattleZone == ZONE_BATTLE
					|| nBattleZone == ZONE_BATTLE2
					|| nBattleZone == ZONE_BATTLE3)
					BattleWinnerResult(BATTLE_WINNER_NPC);
				else if (nBattleZone == ZONE_BATTLE4) // Nereid's Island
					BattleWinnerResult(BATTLE_WINNER_MONUMENT);
				else if (nBattleZone == ZONE_BATTLE5) // Zipang's Island
					BattleWinnerResult(BATTLE_WINNER_MONUMENT);
				else if (nBattleZone == ZONE_BATTLE6) // Oreads
					BattleWinnerResult(BATTLE_WINNER_KILL);
			}
			m_sBattleTimeDelay++;

			if (m_sBattleTimeDelay >= (nBattleZone == ZONE_BATTLE4 ? (m_byBattleTime / 48) : (m_byBattleTime / 24))
				|| m_sBattleTimeDelay >= (nBattleZone == ZONE_BATTLE5 ? (m_byBattleTime / 48) : (m_byBattleTime / 24)))
			{
				m_sBattleTimeDelay = 0;
				Announcement(DECLARE_BATTLE_ZONE_STATUS);
			}
		}
		else
		{
			if (WarElapsedTime < m_byBattleTime) // Won the war.
			{
				m_sBattleTimeDelay++;
				if (m_sBattleTimeDelay >= (m_byBattleTime / 24))
				{
					m_sBattleTimeDelay = 0;
					Announcement(UNDER_ATTACK_NOTIFY);
				}
				CheckNationMonumentRewards();
			}
		}
		if (m_bResultDelay)
		{
			m_sBattleResultDelay++;
			if (m_sBattleResultDelay == (m_byBattleTime / (m_byBattleTime / 10))) {
				m_bResultDelay = false;
				BattleZoneResult(m_bResultDelayVictory);
			}
		}

		if (WarElapsedTime >= m_byBattleTime) // War is over.
			BattleZoneClose();
	}

	if (m_byBanishFlag)
	{
		m_sBanishDelay++;

		if (m_sBanishDelay == (m_byBattleTime / 360))
			Announcement(DECLARE_BAN);
		else if (m_sBanishDelay == (m_byBattleTime / 120))
		{
			m_byBanishFlag = false;
			m_sBanishDelay = 0;
			BanishLosers();
		}
	}
}
#pragma endregion


#pragma region CGameServerDlg::SingleOtherEventTimer()
void CGameServerDlg::SingleOtherEventTimer()
{
	uint32 nWeekDay = g_localTime.tm_wday;
	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	SingleOtherEventLocalTimer();

	if (!m_EventScheduleArray.IsEmpty())
	{
		for (int EventIDi = 1; EventIDi <= m_EventScheduleArray.GetSize(); EventIDi++)
		{
			_EVENT_SCHEDULE* pEventStatus = m_EventScheduleArray.GetData(EventIDi);
			if (pEventStatus == nullptr ||
				pEventStatus->EventStatus == 0 ||
				pEventStatus->EventType != EventType::SingleRoom)
				continue;

			if (pEventStatus->EventLocalID == EventLocalID::UnderTheCastle)
			{
				if (m_bUnderTheCastleIsActive)
					continue;
			}
			if (pEventStatus->EventLocalID == EventLocalID::ForgettenTempleLow)
			{
				if (m_bForgettenTempleIsActive)
					continue;
			}
			if (pEventStatus->EventLocalID == EventLocalID::ForgettenTempleHigh)
			{
				if (m_bForgettenTempleIsActive)
					continue;
			}
			if (pEventStatus->EventLocalID == EventLocalID::BeefEvent)
			{
				if (pBeefEvent.isActive)
					continue;
			}

			bool SingleEventisOkey(pEventStatus->EventLocalID == EventLocalID::UnderTheCastle
				|| pEventStatus->EventLocalID == EventLocalID::ForgettenTempleLow
				|| pEventStatus->EventLocalID == EventLocalID::ForgettenTempleHigh
				|| pEventStatus->EventLocalID == EventLocalID::BeefEvent);

			if (!SingleEventisOkey)
				continue;

			std::list<std::string> nInGameEvent = StrSplit(pEventStatus->StartDays, ",");
			uint8 nInGameEventDaysSize = (uint8)nInGameEvent.size();
			if (nInGameEventDaysSize > 0)
			{
				uint8 nInGameDays = 0;
				for (int EventDayi = 0; EventDayi < nInGameEventDaysSize; EventDayi++)
				{
					nInGameDays = atoi(nInGameEvent.front().c_str());
					if (nInGameDays == nWeekDay)
					{
						for (int EventTimei = 0; EventTimei < EVENT_START_TIMES; EventTimei++)
						{
							if (pEventStatus->TimeActive[EventTimei] != 1
								|| pEventStatus->EventStartMinutes[EventTimei] == 99)
								continue;

							for (int NoticeCount = 1; NoticeCount <= 5; NoticeCount++)
							{
								if (pEventStatus->EventLocalID == EventLocalID::ForgettenTempleHigh
									|| pEventStatus->EventLocalID == EventLocalID::ForgettenTempleLow)
								{
									if (NoticeCount == 1 || NoticeCount == 5)
									{
										if ((pEventStatus->EventStartMinutes[EventTimei] - NoticeCount) < 0)
										{
											int16 sNoticeMinutes = pEventStatus->EventStartMinutes[EventTimei] - NoticeCount;
											if (sNoticeMinutes < 0)
											{
												sNoticeMinutes = 60 + sNoticeMinutes;
												int16 sNoticeHour = pEventStatus->EventStartHour[EventTimei] - 1;
												if (sNoticeHour < 0)
													sNoticeHour = 23;

												if (sNoticeHour == nHour && sNoticeMinutes == nMinute && nSeconds == 0)
												{
													m_nForgettenTempleStartHour = pEventStatus->EventStartHour[EventTimei];
													m_nForgettenTempleLevelMin = pEventStatus->EventLocalID ? EventLocalID::ForgettenTempleLow == 46 : 60;
													m_nForgettenTempleLevelMax = pEventStatus->EventLocalID ? EventLocalID::ForgettenTempleLow == 59 : 83;
													Announcement(IDS_MONSTER_CHALLENGE_ANNOUNCEMENT);
													printf("Forgetten Temple Open Notice (1) left %d Minutes \n", NoticeCount);
												}
											}
										}
										else
										{
											if (pEventStatus->EventStartHour[EventTimei] == nHour
												&& (pEventStatus->EventStartMinutes[EventTimei] - NoticeCount == nMinute)
												&& nSeconds == 0)
											{
												m_nForgettenTempleStartHour = pEventStatus->EventStartHour[EventTimei];
												m_nForgettenTempleLevelMin = pEventStatus->EventLocalID ? EventLocalID::ForgettenTempleLow == 46 : 60;
												m_nForgettenTempleLevelMax = pEventStatus->EventLocalID ? EventLocalID::ForgettenTempleLow == 59 : 83;
												Announcement(IDS_MONSTER_CHALLENGE_ANNOUNCEMENT);
												printf("Forgetten Temple Open Notice (2) left %d Minutes \n", NoticeCount);
											}
										}
									}
								}
							}

							if (nHour == pEventStatus->EventStartHour[EventTimei]
								&& nMinute == pEventStatus->EventStartMinutes[EventTimei]
								&& nSeconds == 0)
							{
								if (pEventStatus->EventLocalID == EventLocalID::ForgettenTempleLow)
								{
									m_nForgettenTempleChallengeTime = 2;

									m_bForgettenTempleIsActive = true;
									m_nForgettenTempleStartTime = int32(UNIXTIME);

									foreach_stlmap(itr, m_MonsterChallengeSummonListArray)
										if (itr->second->bLevel == m_nForgettenTempleChallengeTime)
											m_nForgettenTempleLastStage++;

									if (m_nForgettenTempleLastStage > 56)
										m_nForgettenTempleLastStage = 56;

									Announcement(IDS_MONSTER_CHALLENGE_OPEN);
								}
								else if (pEventStatus->EventLocalID == EventLocalID::ForgettenTempleHigh)
								{
									m_nForgettenTempleChallengeTime = 3;

									m_bForgettenTempleIsActive = true;
									m_nForgettenTempleStartTime = int32(UNIXTIME);

									foreach_stlmap(itr, m_MonsterChallengeSummonListArray)
										if (itr->second->bLevel == m_nForgettenTempleChallengeTime)
											m_nForgettenTempleLastStage++;

									if (m_nForgettenTempleLastStage > 56)
										m_nForgettenTempleLastStage = 56;

									Announcement(IDS_MONSTER_CHALLENGE_OPEN);
								}
								else if (pEventStatus->EventLocalID == EventLocalID::UnderTheCastle)
								{
									m_bUnderTheCastleIsActive = true;
									m_bUnderTheCastleMonster = true;
									m_nUnderTheCastleEventTime = (180 - nMinute) * MINUTE;
									Announcement(IDS_UNDER_THE_CASTLE_OPEN);
								}
								else if (pEventStatus->EventLocalID == EventLocalID::BeefEvent)
								{
									_BEEF_EVENT_PLAY_TIMER* pPlayInfo = m_BeefEventPlayTimerArray.GetData(EventLocalID::BeefEvent);
									if (pPlayInfo == nullptr
										|| pPlayInfo->MonumentTime <= 0
										|| pPlayInfo->LoserSignTime <= 0
										|| pPlayInfo->FarmingTime <= 0)
										continue;

									pBeefEvent.isActive = true;
									pBeefEvent.isAttackable = true;
									pBeefEvent.BeefMonumentStartTime = (uint32)UNIXTIME;
									pBeefEvent.BeefSendTime = pPlayInfo->MonumentTime * MINUTE;
									pBeefEvent.isMonumentDead = false;
									pBeefEvent.WictoryNation = 0;
									BeefEventUpdateTime();
									BeefEventSendNotice(1);

									printf("%d. call for the %s Event is started at %02d:%02d:%02d \n", EventTimei, pEventStatus->EventName.c_str(), nHour, nMinute, nSeconds);

								}
							}
						}
					}
					nInGameEvent.pop_front();
				}
			}
		}
	}

	if (pBeefEvent.isActive)
	{
		_BEEF_EVENT_PLAY_TIMER* pPlayInfo = m_BeefEventPlayTimerArray.GetData(EventLocalID::BeefEvent);
		if (pPlayInfo != nullptr)
		{
			if (!pBeefEvent.isMonumentDead)
			{
				if (pBeefEvent.BeefMonumentStartTime != 0)
				{
					uint32 MonumentTime = pPlayInfo->MonumentTime * MINUTE;
					if ((uint32)UNIXTIME >= (pBeefEvent.BeefMonumentStartTime + MonumentTime))
					{
						ResetBeefEvent();
						BeefEventUpdateTime();
						BeefEventSendNotice(2);
					}
				}
			}
			else
			{
				if (!pBeefEvent.isLoserSign)
				{
					if (pBeefEvent.LoserNationSignTime != 0)
					{
						if ((uint32)UNIXTIME >= pBeefEvent.LoserNationSignTime)
						{
							pBeefEvent.isLoserSign = true;
							pBeefEvent.LoserNationSignTime = 0;
							BeefEventSendNotice(5);
						}
					}
				}

				if (pBeefEvent.isFarmingPlay)
				{
					if (pBeefEvent.BeefFarmingPlayTime != 0)
					{
						if ((uint32)UNIXTIME >= pBeefEvent.BeefFarmingPlayTime)
						{
							ResetBeefEvent();
							BeefEventUpdateTime();
							KickOutZoneUsers(ZONE_BIFROST, ZONE_RONARK_LAND);
							BeefEventSendNotice(4);
						}
					}
				}
			}
		}
	}

	if (m_bForgettenTempleIsActive)
	{
		if (int32(UNIXTIME) - m_nForgettenTempleStartTime == 300 /* 5 minutes */
			&& !m_bForgettenTempleSummonMonsters)
		{
			m_bForgettenTempleSummonMonsters = true;
			m_nForgettenTempleCurrentStage = 1;
			m_nForgettenTempleLastSummonTime = int32(UNIXTIME) + 30;
			Announcement(IDS_MONSTER_CHALLENGE_START);
		}
		else if (m_bForgettenTempleSummonMonsters)
		{
			foreach_stlmap(itr, m_MonsterChallengeSummonListArray)
			{
				if (m_nForgettenTempleChallengeTime != itr->second->bLevel
					|| int32(UNIXTIME) - m_nForgettenTempleLastSummonTime < 45
					|| itr->second->bStage != m_nForgettenTempleCurrentStage
					&& m_nForgettenTempleCurrentStage != 57)
					continue;

				if (m_nForgettenTempleCurrentStage > m_nForgettenTempleLastStage)
					m_bForgettenTempleSummonMonsters = false;
				else
				{
					SpawnEventNpc(itr->second->sSid, true, ZONE_FORGOTTEN_TEMPLE, (float)itr->second->sPosX, 0, (float)itr->second->sPosZ, itr->second->sCount, itr->second->bRange);
					m_nForgettenTempleCurrentStage++;
					m_nForgettenTempleLastSummonTime = int32(UNIXTIME);
				}
			}
		}
		else
		{
			if (m_nForgettenTempleCurrentStage > m_nForgettenTempleLastStage && !m_nForgettenTempleBanishFlag)
			{
				if (m_ForgettenTempleMonsterList.size() == 0)
				{
					m_nForgettenTempleBanishFlag = true;
					m_nForgettenTempleBanishTime = 200;
					Announcement(IDS_MONSTER_CHALLENGE_VICTORY);
					switch (m_nForgettenTempleChallengeTime)
					{
					case 1:
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, RED_TREASURE_CHEST);
						break;
					case 2:
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, LUNAR_ORDER_TOKEN);
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, GREEN_TREASURE_CHEST);
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, FORGETTEN_TEMPLE_COIN1);
						break;
					case 3:
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, LUNAR_ORDER_TOKEN);
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, BLUE_TREASURE_CHEST);
						SendItemZoneUsers(ZONE_FORGOTTEN_TEMPLE, FORGETTEN_TEMPLE_COIN2);
						break;
					}
				}
				else if (int32(UNIXTIME) - m_nForgettenTempleStartTime >= (HOUR - (10 * MINUTE)))
				{
					m_nForgettenTempleBanishFlag = true;
					m_nForgettenTempleBanishTime = 0;
				}
			}
		}

		if (m_nForgettenTempleBanishFlag)
		{
			m_nForgettenTempleBanishTime++;

			if (m_nForgettenTempleBanishTime == 30 /* 30 seconds */)
				Announcement(IDS_MONSTER_CHALLENGE_CLOSE);
			else if (m_nForgettenTempleBanishTime == 230 /* 5 minutes */)
			{
				m_nForgettenTempleBanishFlag = false;
				m_nForgettenTempleBanishTime = 0;

				m_nForgettenTempleUsers.clear();
				m_bForgettenTempleIsActive = false;
				m_nForgettenTempleStartHour = 0;
				m_nForgettenTempleLevelMin = 0;
				m_nForgettenTempleLevelMax = 0;
				m_nForgettenTempleStartTime = 0;
				m_nForgettenTempleChallengeTime = 0;
				m_bForgettenTempleSummonMonsters = false;
				m_nForgettenTempleCurrentStage = 0;
				m_nForgettenTempleLastStage = 0;

				foreach(itr, m_ForgettenTempleMonsterList)
					KillNpc(itr->first, ZONE_FORGOTTEN_TEMPLE);

				m_ForgettenTempleMonsterList.clear();
				m_nForgettenTempleLastSummonTime = 0;

				KickOutZoneUsers(ZONE_FORGOTTEN_TEMPLE);
			}
		}
	}

	if (m_bUnderTheCastleIsActive)
	{
		if (m_bUnderTheCastleMonster == true)
		{
			foreach_stlmap(itr, m_MonsterUnderTheCastleArray)
				SpawnEventNpc(itr->second->sSid, itr->second->bType == 0 ? true : false, ZONE_UNDER_CASTLE, (float)itr->second->X, itr->second->Y, (float)itr->second->Z, itr->second->sCount, itr->second->bRadius, 0, 0, -1, 0, itr->second->byDirection, 1, 0, SpawnEventType::UnderTheCastleSummon, itr->second->bTrapNumber);

			m_bUnderTheCastleMonster = false;
		}
		if (m_nUnderTheCastleEventTime == 10785)
		{
			Packet result(WIZ_UTC_MOVIE, uint8(2));
			result << uint8(1) << uint16(1) << uint32(1);
			Send_Zone(&result, ZONE_UNDER_CASTLE);
		}
		else
		{
			if (m_nUnderTheCastleEventTime == 0)
			{
				foreach(itr, m_UnderTheCastleMonsterList)
					KillNpc(itr->first, ZONE_UNDER_CASTLE);

				m_bUnderTheCastleIsActive = false;
				m_bUnderTheCastleMonster = false;
				m_nUnderTheCastleUsers.clear();
				m_UnderTheCastleMonsterList.clear();

				Announcement(IDS_UNDER_THE_CASTLE_CLOSE);
				KickOutZoneUsers(ZONE_UNDER_CASTLE);
			}
		}
	}
}
#pragma endregion







void CGameServerDlg::tournamentEventTimer()
{
	std::queue<std::string> v_userContainer;
	std::vector<_DEATHMATCH_WAR_INFO> ClanRankingSorted;
	uint32 nWeekDay = g_localTime.tm_wday;
	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;
	std::string clanlar[16];

	



	VirtualEventLocalTimer();

	if (toplamclan == 16 && active == 0) { //16 normalde aktif olmasý için
		g_DBAgent.TournumentUpdate(1);
	}
	else if (toplamclan == 0 && active == 1) {
		g_DBAgent.TournumentUpdate(0);
	}

	
	_TOURNAMENT_SCHEDULE* pEventStatus = m_EventTournamentScheduleArray.GetData(1);
	if (pEventStatus == nullptr ||
	pEventStatus->EventStatus == 0 )
	return;


	std::list<std::string> nInGameEvent = StrSplit(pEventStatus->Start, ",");
	uint8 nInGameEventDaysSize = (uint8)nInGameEvent.size();
	if (nInGameEventDaysSize > 0)
	{
		uint8 nInGameDays = 0;
		for (int EventDayi = 0; EventDayi < nInGameEventDaysSize; EventDayi++)
		{
		nInGameDays = atoi(nInGameEvent.front().c_str());
			if (nInGameDays == nWeekDay)
			{
			if (pEventStatus->TimeActive != 1
				|| pEventStatus->EventStartMinutes == 99)
				continue;
			if (nHour == pEventStatus->EventStartHour
				&& nMinute == pEventStatus->EventStartMinutes
				&& nSeconds == 0)
			{



			

				for(int i =0; i< toplamclan;i += 2)
				{
					int bRoom = 1;
				// /tournamentstart TestingOneClan TestingTwoClan 77

				std::string TournamentName1 = g_pMain->clanlar[i];

				std::string TournamentName2 = g_pMain->clanlar[i+1];


				uint8 TournamentStartZoneID = 78;

				bool ClaniGM = false;
				bool SucsessZoneID = (TournamentStartZoneID == 77
					|| TournamentStartZoneID == 78
					|| TournamentStartZoneID == 96
					|| TournamentStartZoneID == 97
					|| TournamentStartZoneID == 98
					|| TournamentStartZoneID == 99);

				if (!SucsessZoneID)
				{
					// Send Game Server Description
					printf("Error: Invalid Tournament Zone(%d) \n", TournamentStartZoneID);
					return;
				}

				if (TournamentName1.empty() || TournamentName1.size() > 21)
				{
					// Send Game Server Description
					printf("Error: TournamentName1 is empty or size > 21 \n");
					return;
				}

				if (TournamentName2.empty() || TournamentName2.size() > 21)
				{
					// Send Game Server Description
					printf("Error: TournamentName2 is empty or size > 21 \n");
					return;
				}

				if (TournamentName1.c_str() == TournamentName2.c_str())
				{
					// Send Game Server Description
					printf("Error: Two clan names are the same. \n");
					return;
				}

				_TOURNAMENT_DATA* TournamentClanInfo = g_pMain->m_ClanVsDataList.GetData(TournamentStartZoneID);
				if (TournamentClanInfo != nullptr)
				{
					// Send Game Server Description
					printf("Error: Tournament is Zone(%d) Already Active \n", TournamentStartZoneID);
					return;
				}

				_TOURNAMENT_DATA* Tour77 = g_pMain->m_ClanVsDataList.GetData(77);
				_TOURNAMENT_DATA* Tour78 = g_pMain->m_ClanVsDataList.GetData(78);
				_TOURNAMENT_DATA* Tour96 = g_pMain->m_ClanVsDataList.GetData(96);
				_TOURNAMENT_DATA* Tour97 = g_pMain->m_ClanVsDataList.GetData(97);
				_TOURNAMENT_DATA* Tour98 = g_pMain->m_ClanVsDataList.GetData(98);
				_TOURNAMENT_DATA* Tour99 = g_pMain->m_ClanVsDataList.GetData(99);

				CKnights* pAlreadyFirstClan = nullptr, * pAlreadySecondClan = nullptr;
				foreach_stlmap(itr, g_pMain->m_KnightsArray)
				{
					if (!itr->second->GetName().compare(TournamentName1))
						pAlreadyFirstClan = itr->second;

					if (!itr->second->GetName().compare(TournamentName2))
						pAlreadySecondClan = itr->second;
				}

				if (pAlreadyFirstClan != nullptr)
				{
					if (Tour77 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour77->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour77->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour77->aTournamentZoneID);
							return;

						}
					}

					if (Tour78 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour78->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour78->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour78->aTournamentZoneID);
							return;

						}
					}

					if (Tour96 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour96->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour96->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour96->aTournamentZoneID);
							return;

						}
					}

					if (Tour97 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour97->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour97->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour97->aTournamentZoneID);
							return;

						}
					}

					if (Tour98 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour98->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour98->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour98->aTournamentZoneID);
							return;

						}
					}

					if (Tour99 != nullptr)
					{
						if (pAlreadyFirstClan->GetID() == Tour99->aTournamentClanNum[0]
							|| pAlreadyFirstClan->GetID() == Tour99->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour99->aTournamentZoneID);
							return;

						}
					}
				}

				if (pAlreadySecondClan != nullptr)
				{
					if (Tour77 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour77->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour77->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour77->aTournamentZoneID);
							return;

						}
					}

					if (Tour78 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour78->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour78->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour78->aTournamentZoneID);
							return;

						}
					}

					if (Tour96 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour96->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour96->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour96->aTournamentZoneID);
							return;

						}
					}

					if (Tour97 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour97->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour97->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour97->aTournamentZoneID);
							return;

						}
					}

					if (Tour98 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour98->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour98->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour98->aTournamentZoneID);
							return;

						}
					}

					if (Tour99 != nullptr)
					{
						if (pAlreadySecondClan->GetID() == Tour99->aTournamentClanNum[0]
							|| pAlreadySecondClan->GetID() == Tour99->aTournamentClanNum[1])
						{
							// Send Game Server Description
							printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour99->aTournamentZoneID);
							return;

						}
					}
				}

				bool isClanVs = (TournamentStartZoneID == 77 || TournamentStartZoneID == 78);
				bool isPartyVs = (TournamentStartZoneID == 96 || TournamentStartZoneID == 97 || TournamentStartZoneID == 98 || TournamentStartZoneID == 99);
				if (isClanVs)
				{
					CKnights* pFirstClan = nullptr, * pSecondClan = nullptr;
					g_pMain->m_KnightsArray.m_lock.lock();
					foreach_stlmap_nolock(itr, g_pMain->m_KnightsArray)
					{
						if (!itr->second->GetName().compare(TournamentName1))
							pFirstClan = itr->second;

						if (!itr->second->GetName().compare(TournamentName2))
							pSecondClan = itr->second;
					}
					g_pMain->m_KnightsArray.m_lock.unlock();

					if (pFirstClan == nullptr)
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: First clan was not found in database \n");
						return;
					}

					if (pSecondClan == nullptr)
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: Second clan was not found in database \n");
						return;
					}

					uint16 FirstClanOnlineCount = 0;
					pFirstClan->m_arKnightsUser.m_lock.lock();
					foreach_stlmap_nolock(itr, pFirstClan->m_arKnightsUser)
					{
						_KNIGHTS_USER* pFirstClanKnightUser = itr->second;
						if (pFirstClanKnightUser == nullptr)
							continue;

						CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanKnightUser->strUserName, TYPE_CHARACTER);
						if (pFirstClanUser == nullptr)
							continue;

						if (pFirstClanUser->isGM())
						{
							ClaniGM = true;
							break;
						}

						if (pFirstClanUser != nullptr
							&& pFirstClanUser->isInGame()
							&& pFirstClanUser->GetZoneID() != TournamentStartZoneID)
							FirstClanOnlineCount++;
					}
					pFirstClan->m_arKnightsUser.m_lock.unlock();

					if (ClaniGM == true)
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: There is an administrator in Clan (%s) \n", TournamentName1.c_str());
						return;
					}

					if (FirstClanOnlineCount > TOURNAMENT_MAX_CLAN_COUNT)
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: %s Clan is clan high count. (count: %d) Maxmium Count: (%d)\n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MAX_CLAN_COUNT);
						return;
					}

					if (FirstClanOnlineCount < 1) //TOURNAMENT_MIN_CLAN_COUNT
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: %s Clan is clan low count. (count: %d) Minumum Count: (%d)\n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MIN_CLAN_COUNT);
						return;
					}

					uint16 SecondClanOnlineCount = 0;
					pSecondClan->m_arKnightsUser.m_lock.lock();
					foreach_stlmap_nolock(itr, pSecondClan->m_arKnightsUser)
					{
						_KNIGHTS_USER* pSecondClanKnightUser = itr->second;
						if (pSecondClanKnightUser == nullptr)
							continue;

						CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanKnightUser->strUserName, TYPE_CHARACTER);
						if (pSecondClanUser == nullptr)
							continue;

						if (pSecondClanUser->isGM())
						{
							ClaniGM = true;
							break;
						}

						if (pSecondClanUser != nullptr
							&& pSecondClanUser->isInGame()
							&& pSecondClanUser->GetZoneID() != TournamentStartZoneID)
							SecondClanOnlineCount++;
					}
					pSecondClan->m_arKnightsUser.m_lock.unlock();

					if (ClaniGM == true)
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: There is an administrator in Clan (%s) \n", TournamentName2.c_str());
						return;
					}

					if (SecondClanOnlineCount > TOURNAMENT_MAX_CLAN_COUNT) //TOURNAMENT_MAX_CLAN_COUNT
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: %s Clan is clan high count. (count: %d) Maxmium Count: (%d) \n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MAX_CLAN_COUNT);
						return;
					}

					if (SecondClanOnlineCount < 1) //TOURNAMENT_MIN_CLAN_COUNT
					{
						// Send Game Server Description
						printf("Error : Clan Tournament: %s Clan is clan low count. (count: %d) Minumum Count: (%d) \n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MIN_CLAN_COUNT);
						return;
					}

					KickOutZoneUsers(TournamentStartZoneID);

					if (TournamentClanInfo == nullptr)
					{
						_TOURNAMENT_DATA* cTournamentSign = new _TOURNAMENT_DATA;
						cTournamentSign->aTournamentZoneID = TournamentStartZoneID;
						cTournamentSign->aTournamentClanNum[0] = pFirstClan->GetID();
						cTournamentSign->aTournamentClanNum[1] = pSecondClan->GetID();
						cTournamentSign->aTournamentScoreBoard[0] = 0;
						cTournamentSign->aTournamentScoreBoard[1] = 0;
						cTournamentSign->aTournamentTimer = 30 * 60;
						cTournamentSign->aTournamentMonumentKilled = 0;
						cTournamentSign->aTournamentOutTimer = UNIXTIME;
						cTournamentSign->aTournamentisAttackable = false;
						cTournamentSign->aTournamentisStarted = true;
						cTournamentSign->aTournamentisFinished = false;

						if (!m_ClanVsDataList.PutData(cTournamentSign->aTournamentZoneID, cTournamentSign))
							delete cTournamentSign;
					}

					pFirstClan->m_arKnightsUser.m_lock.lock();
					
					foreach_stlmap_nolock(itr, pFirstClan->m_arKnightsUser)
					{
						
						
						_KNIGHTS_USER* pFirstClanKnightUser = itr->second;
						if (pFirstClanKnightUser == nullptr)
							continue;

						std::string strUserID = pFirstClanKnightUser->strUserName;
						v_userContainer.push(strUserID);

						

						CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanKnightUser->strUserName, TYPE_CHARACTER);
						if (pFirstClanUser == nullptr
							|| !pFirstClanUser->isInGame()
							|| pFirstClanUser->GetZoneID() == TournamentStartZoneID)
							continue;
					}
					pFirstClan->m_arKnightsUser.m_lock.unlock();

					
						_CHAOS_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventChaosRoomList.GetData(bRoom);
						if (pRoomInfo == nullptr)
							continue;

						
						for (int x = 0; x < pFirstClan->m_sMembers; x++)
						{
							if (v_userContainer.empty())
								break;

							std::string strUserID = v_userContainer.front();
							v_userContainer.pop();

							_TEMPLE_STARTED_EVENT_USER* pEventUser = new  _TEMPLE_STARTED_EVENT_USER;
							pEventUser->strUserID = strUserID;
							pEventUser->isPrizeGiven = false;
							if (!pRoomInfo->m_UserList.PutData(pEventUser->strUserID, pEventUser))
							{
								delete pEventUser;
								continue;
							}
						}
					

					

					


					pSecondClan->m_arKnightsUser.m_lock.lock();
					foreach_stlmap_nolock(itr, pSecondClan->m_arKnightsUser)
					{
						_KNIGHTS_USER* pSecondClanKnightUser = itr->second;
						if (pSecondClanKnightUser == nullptr)
							continue;

						std::string strUserID = pSecondClanKnightUser->strUserName;
						v_userContainer.push(strUserID);

						CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanKnightUser->strUserName, TYPE_CHARACTER);
						if (pSecondClanUser == nullptr
							|| !pSecondClanUser->isInGame()
							|| pSecondClanUser->GetZoneID() == TournamentStartZoneID)
							continue;

						
					}
					pSecondClan->m_arKnightsUser.m_lock.unlock();
	
						for (int x = 0; x < pSecondClan->m_sMembers; x++)
						{
							if (v_userContainer.empty())
								break;

							std::string strUserID = v_userContainer.front();
							v_userContainer.pop();

							_TEMPLE_STARTED_EVENT_USER* pEventUser = new  _TEMPLE_STARTED_EVENT_USER;
							pEventUser->strUserID = strUserID;
							pEventUser->isPrizeGiven = false;
							if (!pRoomInfo->m_UserList.PutData(pEventUser->strUserID, pEventUser))
							{
								delete pEventUser;
								continue;
							}
						}
					

					foreach_stlmap(itr, g_pMain->m_TempleEventChaosRoomList)
					{
						_CHAOS_ROOM_INFO* pRoomInfo = itr->second;
						if (pRoomInfo == nullptr)
							continue;


						pRoomInfo->m_UserList.m_lock.lock();
						foreach_stlmap_nolock(itrUser, pRoomInfo->m_UserList)
						{
							uint16 aTournamentClanNum;
							uint16 aTournamentClanNum1;
							aTournamentClanNum = pFirstClan->m_sIndex;
							aTournamentClanNum1 = pSecondClan->m_sIndex;

							_TEMPLE_STARTED_EVENT_USER* pEventUser = itrUser->second;
							if (pEventUser == nullptr)
								continue;

							CUser* pUser = g_pMain->GetUserPtr(pEventUser->strUserID, TYPE_CHARACTER);
							if (pUser == nullptr
								|| !pUser->isInGame())
								continue;

							/*C3DMap* pMap = g_pMain->GetZoneByID(ZONE_CLAN_WAR_RONARK);
							if (pMap)
								pMap->UpdateMap(true, ZoneAbilitySiege1);*/

							pUser->UserInOut(INOUT_OUT);
							pUser->m_bEventRoom = itr->first;
							//pUser->ChaosExpansionAddPlayerRank();
							if (aTournamentClanNum == pUser->m_bKnights)
								pUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
							else if (aTournamentClanNum1 == pUser->m_bKnights)
								pUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
						}
						pRoomInfo->m_UserList.m_lock.unlock();

						
					}


					
				}
				

				printf("Final : Tournament is Start: Red Clan: (%s) - (%s) :Blue Clan Tournament Zone (%d) \n", TournamentName1.c_str(), TournamentName2.c_str(), TournamentStartZoneID);
				return;

			}





			














			}
						
		}
					nInGameEvent.pop_front();
	}
}

	

	return;
}