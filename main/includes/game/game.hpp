#pragma once

class entity {
public:
	uintptr_t
		actor,
		skeletal_mesh,
		root_component,
		player_state;
	int
		team_index;
};

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name;
	bool
		isVehicle,
		isChest,
		isPickup,
		isAmmoBox;
	float
		distance;


};

int enemyID = 0;
int visennemy = 0;
int InFovEnemy = 0;
std::vector<item> item_pawns;
std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;

inline void Ud_Draw_line(FVector2d ScreenPositionA, FVector2d ScreenPositionB, float Thickness, ImColor RenderColor) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.x, ScreenPositionA.y), ImVec2(ScreenPositionB.x, ScreenPositionB.y), RenderColor, Thickness);
}
inline void Ud_Draw_line1(FVector2d ScreenPositionA, FVector2d ScreenPositionB, float Thickness, ImColor RenderColor) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.x, ScreenPositionA.y), ImVec2(ScreenPositionB.x + 1, ScreenPositionB.y + 1), RenderColor, Thickness);
}

inline std::wstring MBytesToWString(const char* lpcszString)
{


	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
inline std::string WStringToUTF8(const wchar_t* lpwcszWString)
{


	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

inline void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
}

FVector target_prediction(FVector TargetPosition, FVector ComponentVelocity, float player_distance, float ProjectileSpeed)
{
	float gravity = abs(-336); // Gravity (Probably other ways to get this but I set it as a constant value)
	float time = player_distance / abs(ProjectileSpeed);
	float bulletDrop = (gravity / 250) * time* time;
	return FVector(TargetPosition.x += time * (ComponentVelocity.x), TargetPosition.y += time * (ComponentVelocity.y), TargetPosition.z += time * (ComponentVelocity.z));
}

#define FNAMEPOOL_OFFSET 0x11400C80

static std::string GetNameFromIndex(int key)
{
	SPOOF_FUNC;
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;
	uint64_t NamePoolChunk = read<uint64_t>(virtualaddy + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset);
	uint16_t nameEntry = read<uint16_t>(NamePoolChunk);
	int nameLength = nameEntry >> 6;
	char buff[1024] = {};

	char* v2 = buff; // rdi
	int v4 = nameLength; // ebx
	int v5; // edx
	int v6; // ecx
	int v7; // ecx
	__int16 v8; // ax
	int v9 = 0; // ecx

	v5 = 0;
	v6 = 42;
	v8 = v5 % 0x21;
	if (v4)
	{
		mem::read_physical(reinterpret_cast<void*>(NamePoolChunk + 2), static_cast<uint8_t*>(static_cast<void*>(buff)), 2 * nameLength);
		do
		{
			v7 = v9++ | v8;
			++v9;
			v6 = v7;
			*v2++ ^= ~v6;
		} while (v9 < v4);
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	return std::string("");
}

static std::string GetNameFromFName(int key)
{
	SPOOF_FUNC;
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(virtualaddy + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	if (read<uint16_t>(NamePoolChunk) < 64)
	{
		auto a1 = read<DWORD>(NamePoolChunk + 2);
		return GetNameFromIndex(a1);
	}
	else
	{
		return GetNameFromIndex(key);
	}
}
std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void DrawNewTextCustomWithOutline(int x, int y, int R, int G, int B, int A, const char* str)
{
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	float outlineThickness = 1.5f; // Adjust the outline thickness as needed

	// Draw the text with black outline
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x - outlineThickness, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x + outlineThickness, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y - outlineThickness), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y + outlineThickness), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());

	// Draw the main text with the specified color
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f)), utf_8_2.c_str());
}


uintptr_t RootComponent(uintptr_t actor)
{

	return read<uintptr_t>(actor + 0x198);
}
FVector GetLocation(uintptr_t actor)
{

	return read<FVector>(RootComponent(actor) + 0x128);
}
char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}


enum GAME_INFORMATION : int {
	GAME_THREAD_FAILED = 0,
	GAME_SETUP_SUCCESSFUL = 1
};
int HeldWeaponType;

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};
static int current_item = 0;
ImColor visible_color;
inline int headfunction = 0;
inline int bodyfunction = 0;
inline int handsfunction = 0;
inline int legsfunction = 0;
namespace game {
	class c_game {
	public:

		auto Setup() -> GAME_INFORMATION {

			std::thread(game::c_game::CacheData).detach();


		
			///std::thread(game::c_game::CacheLevels).detach();
			


			return GAME_INFORMATION::GAME_SETUP_SUCCESSFUL;
		}
		auto isVisible(uintptr_t test) -> bool
		{
			float fLastSubmitTime = read<float>(offset::skeletal_mesh + offset::fLastSubmitTime);
			float fLastRenderTimeOnScreen = read<float>(offset::skeletal_mesh + offset::fLastRenderTimeOnScreen);
			const float fVisionTick = 0.06f;
			bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
			return bVisible;
		}
		double Distance2(const FVector& v2)
		{
			double x;
			double y;
			double z;

			double dx = v2.x - x;
			double dy = v2.y - y;
			double dz = v2.z - z;

			return sqrt(dx * dx + dy * dy + dz * dz);
		}
		
		float WorldGravityZ;

		FVector LocationPredict(FVector Target, FVector Velocityy, float GetProjectileSpeed, float ProjectileGravity, float DistanceToTarget)
		{
			float H_time = DistanceToTarget / GetProjectileSpeed;
			float V_time = DistanceToTarget / GetProjectileSpeed;

			Target.x += Velocityy.x * H_time;
			Target.y += Velocityy.y * H_time;
			Target.z += Velocityy.z * H_time +
			abs(WorldGravityZ * ProjectileGravity) * 0.5f * (V_time * V_time);

			return Target;
		}
		

		FVector CalculatePrediction(FVector AimedBone, FVector Velocity, float Distance, float BulletSpeed)
		{
			BulletSpeed *= 100;
			float Gravity = 9.81f * 100.0f;
			float Time = Distance / BulletSpeed;

			FVector TargetPos = AimedBone + (Velocity * Time);

			return TargetPos;
		}

		void DrawBox(int x, int y, int w, int h, ImColor color, int thickness) {
			ImDrawList* Drawlist = ImGui::GetBackgroundDrawList();

			Drawlist->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f, 0, thickness);
		}

		void DrawCorneredBox(int x, int y, int w, int h, ImColor color, int thickness) {

			ImDrawList* Drawlist = ImGui::GetBackgroundDrawList();

			float line_w = (w / 8);
			float line_h = (h / 8);
			Drawlist->AddLine(ImVec2(x, y), ImVec2(x, y + line_h), color, thickness);
			Drawlist->AddLine(ImVec2(x, y), ImVec2(x + line_w, y), color, thickness);
			Drawlist->AddLine(ImVec2(x + w - line_w, y), ImVec2(x + w, y), color, thickness);
			Drawlist->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + line_h), color, thickness);
			Drawlist->AddLine(ImVec2(x, y + h - line_h), ImVec2(x, y + h), color, thickness);
			Drawlist->AddLine(ImVec2(x, y + h), ImVec2(x + line_w, y + h), color, thickness);
			Drawlist->AddLine(ImVec2(x + w - line_w, y + h), ImVec2(x + w, y + h), color, thickness);
			Drawlist->AddLine(ImVec2(x + w, y + h - line_h), ImVec2(x + w, y + h), color, thickness);
		}

	/*	void LevelDrawing()
		{

			SPOOF_FUNC;
			auto levelListCopy = item_pawns;

			for (auto entity : levelListCopy)
			{
				if (pointer->acknowledged_pawn && entity.Actor)
				{

				

					if (globals->vehicle && strstr(entity.Name.c_str(), _xor_("Rufus_GolfCartVehicleSK_C").c_str()))
					{
						if (entity.distance <= globals->max_distance2)
						{
							FVector Location = GetLocation(entity.Actor);
							FVector2d Screen = Utilities->WorldToScreen(Location);
							if (Utilities->InScreen(Screen))
							{

								std::string BuiltString = "Golf Cart";
								ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

								if (globals->outline)
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}
						if (globals->vehicle && strstr(entity.Name.c_str(), _xor_("Rufus_ShoppingCartVehicleSK_C").c_str()))
						{
							if (entity.distance <= globals->max_distance2)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{

									std::string BuiltString = "Shopping Cart";
									ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

									if (globals->outline)
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
									}
									else
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
									}
								}
							}
						}

						if (globals->vehicle && strstr(entity.Name.c_str(), _xor_("Rufus_FerretVehicle_C").c_str()))
						{
							if (entity.distance <= globals->max_distance2)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{

									std::string BuiltString = "Plane";
									ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

									if (globals->outline)
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
									}
									else
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
									}
								}
							}
						}
						if (globals->vehicle && strstr(entity.Name.c_str(), _xor_("Rufus_AntelopeVehicle_C").c_str()))
						{
							if (entity.distance <= globals->max_distance2)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{

									std::string BuiltString = "Quad Crasher";
									ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

									if (globals->outline)
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
									}
									else
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
									}
								}
							}
						}

						

				

						if (globals->vehicle && strstr(entity.Name.c_str(), _xor_("JackalVehicle_Rufus_C").c_str()))
						{
							if (entity.distance <= globals->max_distance2)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{

									std::string BuiltString = "Hover Board";
									ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

									if (globals->outline)
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
									}
									else
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
									}
								}
							}
						}


						if (globals->Lamma && strstr(entity.Name.c_str(), _xor_("AthenaSupplyDrop_Llama_C").c_str()))
						{
							if (entity.distance <= globals->max_distance3)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{
									std::string BuiltString = "llama";

									ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

									if (globals->outline)
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 0, 255), false, true, BuiltString.c_str());
									}
									else
									{
										DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 0, 255), false, false, BuiltString.c_str());
									}

								}
							}
						}
						
							if (globals->pickups && strstr(entity.Name.c_str(), _xor_("FortPickupAthena").c_str()) || strstr(entity.Name.c_str(), _xor_("Fort_Pickup_Creative_C").c_str()))
							{
								if (entity.distance <= globals->max_distance1)
								{

									auto definition = driver.read<uint64_t>(entity.Actor + 0x350 + 0x18);
									if (is_valid(definition))
									{
										BYTE tier = driver.read<BYTE>(definition + 0x73);

										ImColor Color, RGBAColor;
										FVector Location = GetLocation(entity.Actor);

										FVector2d ChestPosition = Utilities->WorldToScreen(Location);


										uint64_t ftext_ptr = driver.read<uint64_t>(definition + 0x90);

										if (is_valid(ftext_ptr)) {
											uint64_t ftext_data = driver.read<uint64_t>(ftext_ptr + 0x28);
											int ftext_length = driver.read<int>(ftext_ptr + 0x30);
											if (ftext_length > 0 && ftext_length < 50) {
												wchar_t* ftext_buf = new wchar_t[ftext_length];
												driver.read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
												wchar_t* WeaponName = ftext_buf;
												delete[] ftext_buf;


												std::string Text = wchar_to_char(WeaponName);
												std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");
												if (tier == 2 && (globals->commom))
												{
													Color = ImColor(0, 255, 0);
												}
												else if ((tier == 3) && (globals->rare))
												{
													Color = ImColor(0, 0, 255);
												}
												else if ((tier == 4) && (globals->epic))
												{
													Color = ImColor(255, 0, 255);
												}
												else if ((tier == 5) && (globals->lengendery))
												{
													Color = ImColor(255, 255, 0);
												}
												else if ((tier == 6) && (globals->mythic))
												{
													Color = ImColor(245, 245, 0);
												}

												else if ((tier == 0) || (tier == 1) && (globals->uncommon))
												{
													Color = ImColor(255, 255, 255);
												}


												ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());



												if (globals->outline)
												{
													DrawString(13, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, Color, false, true, wtf2.c_str());
												}
												else
												{
													DrawString(13, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, Color, false, false, wtf2.c_str());
												}





											}
										}
									}
								}
							}
						

						if (globals->chests && strstr(entity.Name.c_str(), _xor_("Tiered_Chest").c_str()))
						{
							if (entity.distance <= globals->max_distance4)
							{
								FVector Location = GetLocation(entity.Actor);
								FVector2d Screen = Utilities->WorldToScreen(Location);
								if (Utilities->InScreen(Screen))
								{

										std::string BuiltString = "Chest";
										ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

										if (globals->outline)
										{
											DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 0), false, true, BuiltString.c_str());
										}
										else
										{
											DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 0), false, false, BuiltString.c_str());
										}
									

								}
							}
						}

				}
			}
		}*/


	
		//static auto CacheLevels() -> void {
		//
		//	for (;;)
		//	{


		//		if (pointer->acknowledged_pawn)
		//		{
		//			if (!globals->chests && !globals->Lamma && !globals->pickups && !globals->vehicle) continue;
		//			if (!pointer->uworld) continue;
		//			std::vector<item> mrxd;
		//			uintptr_t ItemLevels = driver.read<uintptr_t>(pointer->uworld + 0x170);

		//			

		//				for (int i = 0; i < driver.read<DWORD>(pointer->uworld + (0x170 + sizeof(PVOID))); ++i) {


		//					uintptr_t ItemLevel = driver.read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));

		//					for (int i = 0; i < driver.read<DWORD>(ItemLevel + (0xA0 + sizeof(PVOID))); ++i) {

		//						uintptr_t ItemsPawns = driver.read<uintptr_t>(ItemLevel + 0xA0);
		//					
		//							uintptr_t CurrentItemPawn = driver.read<uintptr_t>(ItemsPawns + (i * sizeof(uintptr_t))); // CurrentActor
		//						
		//								FVector ItemPosition = GetLocation(CurrentItemPawn);
		//								float ItemDist = camera::location.Distance(ItemPosition) / 100.f;

		//								int ItemIndex = driver.read<int>(CurrentItemPawn + 0x18);

		//						
		//									auto CurrentItemPawnName = GetNameFromFName(ItemIndex);
		//									std::cout << "Obecjects - > " << CurrentItemPawnName << std::endl;



		//									bool shouldUpdate;
		//									bool bIsPickup;

		//									if (is_valid(CurrentItemPawn))
		//									{

		//										if (globals->pickups or globals->vehicle or globals->Lamma or globals->chests)
		//										{


		//											bIsPickup = true;
		//											shouldUpdate = true;

		//										}
		//									}
		//									






		//									if (shouldUpdate)
		//									{
		//										item Item{ };
		//										Item.Actor = CurrentItemPawn;
		//										Item.Name = CurrentItemPawnName;
		//										Item.isPickup = bIsPickup;
		//										Item.distance = ItemDist;


		//										mrxd.push_back(Item);

		//									}



		//					}
		//				}
		//				item_pawns.clear();
		//				item_pawns = mrxd;
		//				std::this_thread::sleep_for(std::chrono::seconds(3));
		//		
		//		}
		//	}
		//}

		static auto CacheData() -> void {

			for (;; ) {

				temporary_entity_list.clear();

				pointer->uworld = read<uintptr_t>(virtualaddy + offset::uworld);
				if (Utilities->debug)
				{
					std::printf(" [uworld] -> %I64d\n", pointer->uworld);
				}
				pointer->game_instance = read<uintptr_t>(pointer->uworld + offset::game_instance);
				if (Utilities->debug)
				{
					std::printf(" [game_instance] -> %I64d\n", pointer->game_instance);
				}
				pointer->levels = read<uintptr_t>(pointer->levels + 0x170);
				if (Utilities->debug)
				{
					std::printf(" [levels] -> %I64d\n", pointer->levels);
				}
				pointer->game_state = read<uintptr_t>(pointer->uworld + offset::game_state);
				if (Utilities->debug)
				{
					std::printf(" [game_state] -> %I64d\n", pointer->game_state);
				}
				pointer->local_player = read<uintptr_t>(read<uintptr_t>(pointer->game_instance + offset::local_player));
				if (Utilities->debug)
				{
					std::printf(" [local_player] -> %I64d\n", pointer->local_player);
				}
				pointer->player_controller = read<uintptr_t>(pointer->local_player + offset::player_controller);
				if (Utilities->debug)
				{
					std::printf(" [player_controller] -> %I64d\n", pointer->player_controller);
				}
				pointer->acknowledged_pawn = read<uintptr_t>(pointer->player_controller + offset::acknowledged_pawn);
				if (Utilities->debug)
				{
					std::printf(" [acknowledged_pawn] -> %I64d\n", pointer->acknowledged_pawn);
				}
				pointer->current_weapon = read<uintptr_t>(pointer->acknowledged_pawn + offset::current_weapon);
				if (Utilities->debug)
				{
					std::printf(" [current_weapon] -> %I64d\n", pointer->current_weapon);
				}
				pointer->skeletal_mesh = read<uintptr_t>(pointer->acknowledged_pawn + offset::skeletal_mesh);
				if (Utilities->debug)
				{
					std::printf(" [skeletal_mesh] -> %I64d\n", pointer->skeletal_mesh);
				}
				pointer->player_state = read<uintptr_t>(pointer->acknowledged_pawn + offset::player_state);
				if (Utilities->debug)
				{
					std::printf(" [player_state] -> %I64d\n", pointer->player_state);
				}
				pointer->root_component = read<uintptr_t>(pointer->acknowledged_pawn + offset::root_component);
				if (Utilities->debug)
				{
					std::printf(" [root_component] -> %I64d\n", pointer->root_component);
				}
				pointer->team_index = read<int>(pointer->player_state + offset::team_index);
				if (Utilities->debug)
				{
					std::printf(" [team_index] -> %I64d\n", pointer->team_index);
				}
				pointer->relative_location = read<FVector>(pointer->root_component + offset::relative_location);
				if (Utilities->debug)
				{
					std::printf(" [relative_location.x] -> %I64d\n", pointer->relative_location.x);
					std::printf(" [relative_location.y] -> %I64d\n", pointer->relative_location.y);
					std::printf(" [relative_location.z] -> %I64d\n", pointer->relative_location.z);
				}
				pointer->player_array = read<uintptr_t>(pointer->game_state + offset::player_array);
				if (Utilities->debug)
				{
					std::printf(" [player_array] -> %I64d\n", pointer->player_array);
				}
				pointer->player_array_size = read<int>(pointer->game_state + (offset::player_array + sizeof(uintptr_t)));
				if (Utilities->debug)
				{
					std::printf(" [player_array_size] -> %I64d\n", pointer->player_array_size);
				}


				




				for (int i = 0; i < pointer->player_array_size; ++i) {
					auto player_state = read<uintptr_t>(pointer->player_array + (i * sizeof(uintptr_t)));
					auto current_actor = read<uintptr_t>(player_state + offset::pawn_private);
					if (Utilities->debug)
					{
						std::printf(" [current_actor] -> %I64d\n", current_actor);
					}
					if (current_actor == pointer->acknowledged_pawn) continue;
					auto skeletalmesh = read<uintptr_t>(current_actor + offset::skeletal_mesh);
					if (!skeletalmesh) continue;
					if (Utilities->debug)
					{
						std::printf(" [skeletalmesh] -> %I64d\n", skeletalmesh);
					}
					auto base_bone = Utilities->GetBoneLocation(skeletalmesh, bone::HumanBase);
					if (base_bone.x == 0 && base_bone.y == 0 && base_bone.z == 0) continue;

					if (!Utilities->InScreen(Utilities->WorldToScreen(Utilities->GetBoneLocation(skeletalmesh, bone::HumanPelvis)))) continue;

					auto is_despawning = (read<char>(current_actor + 0x758) >> 3);
					if (Utilities->debug)
					{
						std::printf(" [is_despawning] -> %I64d\n", is_despawning);
					}

					if (is_despawning) continue;
					if (Utilities->debug)
					{
						std::printf(" GOT PAST IS DESPAWNING ");

					}

					if (pointer->acknowledged_pawn)
					{
						auto team_index = read<int>(player_state + offset::team_index);
						if (pointer->team_index == team_index) continue;
						if (Utilities->debug)
						{
							std::printf(" GOT PAST TEAM CHECK ");

						}
					}

					entity cached_actors{};
					cached_actors.actor = current_actor;
					cached_actors.skeletal_mesh = read<uintptr_t>(current_actor + offset::skeletal_mesh);
					cached_actors.root_component = read<uintptr_t>(current_actor + offset::root_component);
					cached_actors.player_state = read<uintptr_t>(current_actor + offset::player_state);
					cached_actors.team_index = read<int>(cached_actors.player_state + offset::team_index);
					temporary_entity_list.push_back(cached_actors);
				}
				entity_list.clear();
				entity_list = temporary_entity_list;

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

		}


		auto ActorLoop() -> void {



			target_dist = FLT_MAX;
			target_entity = NULL;
			desync_target_entity = NULL;

			if (GetAsyncKeyState_Spoofed(VK_END)) {
				ExitProcess(0);
			}
			Utilities->UpdateCamera();

			if (pointer->acknowledged_pawn) {



				uint64_t player_weapon = read<uint64_t>(pointer->acknowledged_pawn + 0xa20); // 	struct AFortWeapon* CurrentWeapon;

				if (is_valid(player_weapon)) {





					uint64_t weapon_data = read<uint64_t>(player_weapon + 0x4a8);	//struct UFortWeaponItemDefinition* WeaponData;


					if (is_valid(weapon_data)) {

						uint64_t ftext_ptr = read<uint64_t>(weapon_data + 0x90);

						if (is_valid(ftext_ptr)) {
							uint64_t ftext_data = read<uint64_t>(ftext_ptr + 0x28);
							int ftext_length = read<int>(ftext_ptr + 0x30);
							if (ftext_length > 0 && ftext_length < 50) {
								wchar_t* ftext_buf = new wchar_t[ftext_length];

								mem::read_physical(reinterpret_cast<void*>(ftext_data), reinterpret_cast<uint8_t*>(ftext_buf), ftext_length * sizeof(wchar_t));
								std::wstring wstr_buf(ftext_buf);



								LocalPlayerWeapon = std::string(wstr_buf.begin(), wstr_buf.end());

								wchar_t* WeaponName = ftext_buf;

								delete[] ftext_buf;


								if (Utilities->debug)
								{
									std::cout << "Current Local Player Weapon ->" << LocalPlayerWeapon << std::endl;
								}


								if (wcsstr(WeaponName, E(L"Assault Rifle")) || wcsstr(WeaponName, E(L"Havoc Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Red-Eye Assault Rifle"))
									|| wcsstr(WeaponName, E(L"Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Striker Burst Rifle")) || wcsstr(WeaponName, E(L"Burst Assault Rifle"))
									|| wcsstr(WeaponName, E(L"Ranger Assault Rifle")) || wcsstr(WeaponName, E(L"Flapjack Rifle")) || wcsstr(WeaponName, E(L"Heavy Assault Rifle"))
									|| wcsstr(WeaponName, E(L"MK-Seven Assault Rifle")) || wcsstr(WeaponName, E(L"MK-Alpha Assault Rifle")) || wcsstr(WeaponName, E(L"Combat Assault Rifle"))
									|| wcsstr(WeaponName, E(L"Tactical Assault Rifle")) || wcsstr(WeaponName, E(L"Hammer Assault Rifle")) || wcsstr(WeaponName, E(L"Striker AR")) || wcsstr(WeaponName, E(L"Sideways Rifle")) || wcsstr(WeaponName, E(L"AR")) || wcsstr(WeaponName, E(L"Nemesis AR")) || wcsstr(WeaponName, E(L"Makeshift Rifle"))) {
									HeldWeaponType = EFortWeaponType::Rifle;
									if (Utilities->debug)
									{
										std::cout << "Holding AR" << std::endl;

									}
									if (globals->weapon_cfg)
									{



										globals->aimbot = rifle::aimbot;
										globals->render_field_of_view = rifle::render_fov;
										globals->field_of_view = rifle::aim_fov;
										globals->smooth = rifle::smooth;
									}
								}
								if (wcsstr(WeaponName, E(L"Shotgun"))) {

									if (Utilities->debug)
									{
										std::cout << "Holding Shotgun" << std::endl;

									}
									HeldWeaponType = EFortWeaponType::Shotgun;
									if (globals->weapon_cfg)
									{

										globals->aimbot = shotgun::aimbot;
										globals->render_field_of_view = shotgun::render_fov;
										globals->field_of_view = shotgun::aim_fov;
										globals->smooth = shotgun::smooth;
									}


								}
								if (wcsstr(WeaponName, E(L"Smg")) || wcsstr(WeaponName, E(L"Submachine Gun")) || wcsstr(WeaponName, E(L"Hyper SMG")) || wcsstr(WeaponName, E(L"Thunder Burst SMG")) || wcsstr(WeaponName, E(L"SMG")) || wcsstr(WeaponName, E(L"Combat Smg")) || wcsstr(WeaponName, E(L"Ranger Pistol")) || wcsstr(WeaponName, E(L"Pistol"))) {
									HeldWeaponType = EFortWeaponType::Smg;
									if (Utilities->debug)
									{
										std::cout << "Holding SMG" << std::endl;

									}
									if (globals->weapon_cfg)
									{

										globals->aimbot = smg::aimbot;
										globals->render_field_of_view = smg::render_fov;
										globals->field_of_view = smg::aim_fov;
										globals->smooth = smg::smooth;
									}


								}
								if (wcsstr(WeaponName, E(L"Hunting Rifle")) || wcsstr(WeaponName, E(L"Heavy Sniper Rifle")) || wcsstr(WeaponName, E(L"Suppressed Sniper Rifle"))
									|| wcsstr(WeaponName, E(L"Storm Scout")) || wcsstr(WeaponName, E(L"Bolt-Action Sniper Rifle")) || wcsstr(WeaponName, E(L"Automatic Sniper Rifle"))
									|| wcsstr(WeaponName, E(L"DMR")) || wcsstr(WeaponName, E(L"Thermal DMR")) || wcsstr(WeaponName, E(L"Hunter Bolt-Action Sniper"))
									|| wcsstr(WeaponName, E(L"Crossbow")) || wcsstr(WeaponName, E(L"Reaper Sniper Rifle")) || wcsstr(WeaponName, E(L"Mechanical Bow"))) {
									HeldWeaponType = EFortWeaponType::Sniper;
									if (Utilities->debug)
									{
										std::cout << "Holding SNIPER" << std::endl;

									}
									if (globals->weapon_cfg)
									{

										globals->aimbot = sniper::aimbot;
										globals->render_field_of_view = sniper::render_fov;
										globals->field_of_view = sniper::aim_fov;
										globals->smooth = sniper::smooth;

									}

								}
							}
						}
					}
				}
			}


			for (auto& cached_entity : entity_list) {

				auto root_location = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanBase);
				auto root_screen = Utilities->WorldToScreen(root_location);
				auto head_location = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead);
				auto head_screen = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead));
				auto head_screen1 = Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead);

				FVector2d head_box = Utilities->WorldToScreen(FVector(head_screen1.x, head_screen1.y, head_screen1.z + 15));
				FVector2d head_2d = Utilities->WorldToScreen(FVector(head_location.x, head_location.y, head_location.z + 20));

				float box_height = abs(head_screen.y - root_screen.y);
				float box_width = box_height * 0.50f;
				float distance = camera::location.Distance(root_location) / 100;

				if (globals->aimbot) {
					auto dx = head_screen.x - (globals->width / 2);
					auto dy = head_screen.y - (globals->height / 2);
					auto dist = sqrtf(dx * dx + dy * dy);

					if (dist < globals->field_of_view && dist < target_dist) {
						target_dist = dist;
						target_entity = cached_entity.actor;
					}
				}

				enemyID += 1;

				ImColor visible_color_box;
				ImColor visible_color_skel;
				ImColor visible_color_Snapline;
				ImColor visible_color_Text;
				ImColor visible_color_Text1;

				if (distance <= 15)
				{
					InFovEnemy += 1;
				}

				if (globals->DoVisibleCheck) {
					if (Utilities->IsVisible(cached_entity.skeletal_mesh)) {
						visennemy += 1;
						visible_color_box = ImVec4(PlayerColor::BoxVisible[0], PlayerColor::BoxVisible[1], PlayerColor::BoxVisible[2], 1.0f);
						visible_color_skel = ImVec4(PlayerColor::SkeletonVisible[0], PlayerColor::SkeletonVisible[1], PlayerColor::SkeletonVisible[2], 1.0f);
						visible_color_Snapline = ImVec4(PlayerColor::SnaplineVisible[0], PlayerColor::SnaplineVisible[1], PlayerColor::SnaplineVisible[2], 1.0f);
						visible_color_Text = ImVec4(PlayerColor::TopTextVisible[0], PlayerColor::TopTextVisible[1], PlayerColor::TopTextVisible[2], 1.0f);
						visible_color_Text1 = ImVec4(PlayerColor::BottomTextVisible[0], PlayerColor::BottomTextVisible[1], PlayerColor::BottomTextVisible[2], 1.0f);
					}
					else {
						visible_color_box = ImVec4(PlayerColor::BoxNotVisible[0], PlayerColor::BoxNotVisible[1], PlayerColor::BoxNotVisible[2], 1.0f);
						visible_color_skel = ImVec4(PlayerColor::SkeletonNotVisible[0], PlayerColor::SkeletonNotVisible[1], PlayerColor::SkeletonNotVisible[2], 1.0f);
						visible_color_Snapline = ImVec4(PlayerColor::SnaplineNotVisible[0], PlayerColor::SnaplineNotVisible[1], PlayerColor::SnaplineNotVisible[2], 1.0f);
						visible_color_Text = ImVec4(PlayerColor::TopTextNotVisible[0], PlayerColor::TopTextNotVisible[1], PlayerColor::TopTextNotVisible[2], 1.0f);
						visible_color_Text1 = ImVec4(PlayerColor::BottomTextNotVisible[0], PlayerColor::BottomTextNotVisible[1], PlayerColor::BottomTextNotVisible[2], 1.0f);
					}
				}


				if (HeldWeaponType == EFortWeaponType::Shotgun)
				{
					if (Cheat_shi.star)
					{
						if (globals->triggerbot && GetAsyncKeyState_Spoofed(globals->triggerbot_key)) {
							POINT cursor;
							GetCursorPos(&cursor);
							if (Utilities->IsShootable(read<FVector>(pointer->player_controller + offset::location_under_reticle), head_location)) {
								if (distance <= globals->maximum_distance) {
									if (globals->has_clicked) {
										globals->tb_begin = std::chrono::steady_clock::now();
										globals->has_clicked = 0;
									}
									globals->tb_end = std::chrono::steady_clock::now();
									globals->tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(globals->tb_end - globals->tb_begin).count();
									if (globals->tb_time_since >= globals->custom_delay) {
										PostMessage(globals->hwnd, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
										PostMessage(globals->hwnd, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
										globals->has_clicked = 1;
									}
								}
							}
						}
					}
					else if (Cheat_shi.orbit)
					{
						POINT cursor;
						GetCursorPos(&cursor);
						if (Utilities->IsShootable(read<FVector>(pointer->player_controller + offset::location_under_reticle), head_location)) {
							if (distance <= globals->maximum_distance) {
								if (globals->has_clicked) {
									globals->tb_begin = std::chrono::steady_clock::now();
									globals->has_clicked = 0;
								}
								globals->tb_end = std::chrono::steady_clock::now();
								globals->tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(globals->tb_end - globals->tb_begin).count();
								if (globals->tb_time_since >= globals->custom_delay) {
									PostMessage(globals->hwnd, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
									PostMessage(globals->hwnd, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
									globals->has_clicked = 1;
								}
							}
						}
					}

				}


				if (HeldWeaponType == EFortWeaponType::Shotgun)
				{
					if (Cheat_shi.River)
					{
						if (globals->triggerbot && GetAsyncKeyState_Spoofed(globals->triggerbot_key)) {
							POINT cursor;
							GetCursorPos(&cursor);
							if (Utilities->IsShootable(read<FVector>(pointer->player_controller + offset::location_under_reticle), head_location)) {
								if (distance <= globals->maximum_distance) {
									if (globals->has_clicked) {
										globals->tb_begin = std::chrono::steady_clock::now();
										globals->has_clicked = 0;
									}
									globals->tb_end = std::chrono::steady_clock::now();
									globals->tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(globals->tb_end - globals->tb_begin).count();
									if (globals->tb_time_since >= globals->custom_delay) {
										PostMessage(globals->hwnd, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
										PostMessage(globals->hwnd, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
										globals->has_clicked = 1;
									}
								}
							}
						}
					}
					else if (Cheat_shi.orbit)
					{
							POINT cursor;
							GetCursorPos(&cursor);
							if (Utilities->IsShootable(read<FVector>(pointer->player_controller + offset::location_under_reticle), head_location)) {
								if (distance <= globals->maximum_distance) {
									if (globals->has_clicked) {
										globals->tb_begin = std::chrono::steady_clock::now();
										globals->has_clicked = 0;
									}
									globals->tb_end = std::chrono::steady_clock::now();
									globals->tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(globals->tb_end - globals->tb_begin).count();
									if (globals->tb_time_since >= globals->custom_delay) {
										PostMessage(globals->hwnd, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
										PostMessage(globals->hwnd, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
										globals->has_clicked = 1;
									}
								}
							}
					}

				}





				if (globals->username) {

					auto username = Utilities->DecryptName(cached_entity.player_state);

					ImVec2 text_size = ImGui::CalcTextSize(username.c_str());

					if (globals->outline || globals->text_outline)
					{
						DrawString(globals->font_size, head_box.x - (text_size.x / 2), head_box.y - 18, visible_color_Text, false, true, username.c_str());
					}
					else
					{
						DrawString(globals->font_size, head_box.x - (text_size.x / 2), head_box.y - 18, visible_color_Text, false, false, username.c_str());
					}





				}





				if (globals->Cornerbox) {

					auto root_box1 = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 0));


					float CornerHeight = abs(head_box.y - root_box1.y);
					float CornerWidth = box_height * 0.50f;
					if (globals->outline)
					{
						DrawCorneredBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), globals->box_thick + 3.5);


					}
					DrawCorneredBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, visible_color_box, globals->box_thick);
				}
				if (globals->bounding_box) {
					auto root_box1 = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 0));

					float CornerHeight = abs(head_box.y - root_box1.y);
					float CornerWidth = box_height * 0.50f;
					if (globals->outline)
					{
						DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), globals->box_thick + 3.5);


					}
					DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, visible_color_box, globals->box_thick);
				}
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();

				if (globals->snaplines) {
					draw_list->AddLine(ImVec2(globals->width / 2, 0), ImVec2(head_2d.x, head_2d.y), visible_color_Snapline, globals->Linethick);
				}

				int CloseRangeDistanceID = 0;
				float CloseRangeDistance = 50.f;
				int enemyID = 0;
				int enemyIDvis = 0;

				if (globals->enemiesaround && enemyID > 0) {
					std::string s = std::to_string(enemyID);
					std::string a = ("[") + s + (" Enemies]");
					std::string z = std::to_string(CloseRangeDistanceID);
					std::string c = ("[") + z + (" CR]");
					auto sizeC = ImGui::CalcTextSize(c.c_str());
					DrawString((float)(globals->width / 2) - (sizeC.x / 2), 80, 255, 255, 255, 200, c.c_str());

					auto sizeA = ImGui::CalcTextSize(a.c_str());


					DrawString((float)(globals->width / 2) - (sizeA.x / 2), 50, 255, 255, 255, 200, a.c_str());
					std::string b = std::to_string(enemyIDvis);
					std::string d = ("[") + b + (" Visible]");
					auto sizeD = ImGui::CalcTextSize(d.c_str());
					DrawString((float)(globals->width / 2) - (sizeD.x / 2), 20, 255, 255, 255, 200, d.c_str());
				}

				if (enemyID != 0)
					enemyID = 0;

				if (CloseRangeDistanceID != 0)
					CloseRangeDistanceID = 0;

				if (enemyIDvis != 0)
					enemyIDvis = 0;


				if (globals->skeletons) {

					auto neck = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanHead));
					auto chest = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanNeck));

					auto left_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 9));
					auto left_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLForearm23));
					auto left_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLHand));
					auto right_shoulder = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, 38));
					auto right_elbow = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRForearm23));
					auto right_hand = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRHand));
					auto pelvis = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanPelvis));
					auto left_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLThigh1));
					auto left_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLThigh3));
					auto left_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanLFoot2));
					auto right_hip = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRThigh1));
					auto right_knee = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRThigh3));
					auto right_foot = Utilities->WorldToScreen(Utilities->GetBoneLocation(cached_entity.skeletal_mesh, bone::HumanRFoot2));


					if (globals->outline)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);

						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), globals->skeleton_thick + 2.5);

					}
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), visible_color_skel, globals->skeleton_thick);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), visible_color_skel, globals->skeleton_thick);

				}

				if (globals->distance) {


					std::string distance_string = "[" + std::to_string((int)distance) + "m]";

					ImVec2 text_size = ImGui::CalcTextSize(distance_string.c_str());



					if (globals->outline || globals->text_outline)
					{
						DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + 5, visible_color_Text1, false, true, distance_string.c_str());

					}
					else
					{
						DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + 5, visible_color_Text1, false, false, distance_string.c_str());
					}


				}

				if (globals->head) {


					if (!pointer->acknowledged_pawn)
					{
						FVector2d head_2d = Utilities->WorldToScreen(FVector(head_location.x, head_location.y, head_location.z + 20));

						FVector delta = head_location - camera::location;
						float distance = delta.length();

						const float constant_circle_size = 10;

						float circle_radius = constant_circle_size * (globals->height / (2.0f * distance * tanf(camera::fov * (float)M_PI / 360.f)));

						float y_offset = +70.0f;
						head_2d.y += y_offset;

						int segments = 50;
						float thickness = 2.0f;

						if (globals->outline)
						{
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, globals->skeleton_thick + 2);
						}
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, visible_color_skel, segments, globals->skeleton_thick);

					}
					else
					{
						FVector2d head_2d = Utilities->WorldToScreen(FVector(head_location.x, head_location.y, head_location.z));

						FVector delta = head_location - camera::location;
						float distance = delta.length();

						const float constant_circle_size = 10;

						float circle_radius = constant_circle_size * (globals->height / (2.0f * distance * tanf(camera::fov * (float)M_PI / 360.f)));

						float y_offset = +12.0f;



						int segments = 50;
						float thickness = 2.0f;

						if (globals->outline)
						{
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, globals->skeleton_thick + 2);
						}
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, visible_color_skel, segments, globals->skeleton_thick);

					}

				}

				if (globals->weapon_esp)
				{
					if (pointer->acknowledged_pawn)
					{

						WeaponInformation held_weapon{};

						uint64_t player_weapon = read<uint64_t>(cached_entity.actor + offset::current_weapon);

						if (is_valid(player_weapon)) {
							

							uint64_t weapon_data = read<uint64_t>(player_weapon + offset::weapon_data);

							if (is_valid(weapon_data)) {
								held_weapon.tier = read<BYTE>(weapon_data + offset::tier);

								uint64_t ftext_ptr = read<uint64_t>(weapon_data + 0x90);

								if (is_valid(ftext_ptr)) {
									uint64_t ftext_data = read<uint64_t>(ftext_ptr + 0x28);
									int ftext_length = read<int>(ftext_ptr + 0x30);
									if (ftext_length > 0 && ftext_length < 50) {
										wchar_t* ftext_buf = new wchar_t[ftext_length];

										mem::read_physical(reinterpret_cast<void*>(ftext_data), reinterpret_cast<uint8_t*>(ftext_buf), ftext_length * sizeof(wchar_t));
										std::wstring wstr_buf(ftext_buf);



										held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());



										delete[] ftext_buf;
									}
								}

							}



							WeaponInfo = held_weapon;


							std::string final = ("[") + WeaponInfo.weapon_name + ("]");



							ImVec2 TextSize = ImGui::CalcTextSize(final.c_str());


							ImVec2 text_size = ImGui::CalcTextSize(final.c_str());
							int add;
							if (globals->distance)
							{
								add = 20;
							}
							else
							{
								add = 5;
							}





							if (WeaponInfo.tier == 0)
							{
								if (strstr(WeaponInfo.weapon_name.c_str(), ("Pickaxe")) != nullptr)
								{






									if (globals->outline || globals->text_outline)
									{
										DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + add, visible_color_Text1, false, true, final.c_str());

									}
									else
									{
										DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + add, visible_color_Text1, false, false, final.c_str());
									}
								}
								else
								{


									std::string fina1l = ("[Building Plan]");



									ImVec2 text_size1 = ImGui::CalcTextSize(fina1l.c_str());


									if (globals->outline || globals->text_outline)
									{
										DrawString(globals->font_size, root_screen.x - (text_size1.x / 2), root_screen.y + add, visible_color_Text1, false, true, fina1l.c_str());

									}
									else
									{
										DrawString(globals->font_size, root_screen.x - (text_size1.x / 2), root_screen.y + add, visible_color_Text1, false, false, fina1l.c_str());
									}


								}


							}
							else
							{

								if (globals->outline || globals->text_outline)
								{
									DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + add, visible_color_Text1, false, true, final.c_str());

								}
								else
								{
									DrawString(globals->font_size, root_screen.x - (text_size.x / 2), root_screen.y + add, visible_color_Text1, false, false, final.c_str());
								}



							}

						

						}

					}
				}
				if (enemyID != 0)
					enemyID = 0;

				if (InFovEnemy != 0)
					InFovEnemy = 0;

				if (visennemy != 0)
					visennemy = 0;

			}



			if (target_entity && globals->aimbot) {
				auto closest_mesh = read<uint64_t>(target_entity + offset::skeletal_mesh);
				FVector Velocity = read<FVector>(pointer->root_component + offset::velocity);

				FVector hitbox;
				float projectileSpeed = 60000.f;//0;
				float projectileGravityScale = 3.5f;	//0;

				if (globals->HeadBone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanHead);
				}
				else if (globals->NeckBone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanNeck);
				}
				else if (globals->ChestBone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanChest);
				}
				else if (globals->PelvisBone) {
					hitbox = Utilities->GetBoneLocation(closest_mesh, bone::HumanPelvis);
				}

				if (globals->prediction) {
					if (EFortWeaponType::Sniper)
					{
						if (strstr(LocalPlayerWeapon.c_str(), ("Dragon's Breath Sniper")) || strstr(LocalPlayerWeapon.c_str(), ("Storm Scout")) ||
							strstr(LocalPlayerWeapon.c_str(), ("Storm Scout Sniper Rifle")) || strstr(LocalPlayerWeapon.c_str(), ("Hunting Rifle")) ||
							strstr(LocalPlayerWeapon.c_str(), ("Explosive Repeater Rifle")) || strstr(LocalPlayerWeapon.c_str(), ("Bolt-Action Sniper Rifle")) ||
							strstr(LocalPlayerWeapon.c_str(), ("Suppressed Sniper Rifle")) || strstr(LocalPlayerWeapon.c_str(), ("Lever Action Rifle")),
							strstr(LocalPlayerWeapon.c_str(), ("Boom Sniper Rifle"))) {

							projectileSpeed = 65000.f;
							projectileGravityScale = 2.5f;
						}
						if (strstr(LocalPlayerWeapon.c_str(), ("Reaper Sniper Rifle"))) {

							std::cout << (skCrypt("Reaper Sniper Rifle in HAND")) << std::flush;
							projectileSpeed = 60000.f;
							projectileGravityScale = 3.5f;
						}

						else if (strstr(LocalPlayerWeapon.c_str(), ("Heavy Sniper Rifle")) || strstr(LocalPlayerWeapon.c_str(), ("Hunter Bolt-Action Sniper"))) {
							projectileSpeed = 45000.f;
							projectileGravityScale = 0.12f;
						}
						else if (strstr(LocalPlayerWeapon.c_str(), ("Cobra DMR")) || strstr(LocalPlayerWeapon.c_str(), ("DMR")) || strstr(LocalPlayerWeapon.c_str(), ("Thermal DMR"))) {
							projectileSpeed = 53000.f;
							projectileGravityScale = 0.15f;
						}
						else if (strstr(LocalPlayerWeapon.c_str(), ("Automatic Sniper Rifle"))) {
							projectileSpeed = 50000.f;
							projectileGravityScale = 0.12f;
						}
						else {
							projectileSpeed = 0;
							projectileGravityScale = 0;
						}
					}
				}


				int bone;
				if (projectileSpeed) {
					bone = bone::HumanHead;
				}


				auto Distance = camera::location.Distance(hitbox);

				if (projectileSpeed) {

					if (globals->pred1) {
						hitbox = target_prediction(hitbox, Velocity, Distance, projectileSpeed);  //1
					}
					if (globals->pred2) {
						hitbox = CalculatePrediction(hitbox, Velocity, Distance, projectileSpeed);  //2
					}
					if (globals->pred3){
					hitbox = LocationPredict(hitbox, Velocity, projectileSpeed, projectileGravityScale, Distance); //3
					}
				}

				FVector2d hitbox_screen = Utilities->WorldToScreen(hitbox);



				if (hitbox.x != 0 || hitbox.y != 0 && (Utilities->GetCrossDistance(hitbox.x, hitbox.y, globals->width / 2, globals->height / 2) <= globals->field_of_view))
				{

					if (Utilities->IsVisible(closest_mesh)) {

						if (globals->target_line) {
							ImGui::GetForegroundDrawList()->AddLine(ImVec2(globals->width / 2, globals->height / 2), ImVec2(hitbox_screen.x, hitbox_screen.y), ImColor(255, 255, 255, 255), 1);
						}



						if (globals->target_text)
						{

							DrawString(20, hitbox_screen.x - 6, hitbox_screen.y - 50, ImColor(255, 0, 0), true, true, E("[T]"));

						}

						if (GetAsyncKeyState_Spoofed(globals->aimbot_key))
							Utilities->cursor_to(hitbox_screen.x, hitbox_screen.y);


					}
				}

			}
			else {
				target_dist = FLT_MAX;
				target_entity = NULL;
			}



			

		

		}
	};
} static game::c_game* Game = new game::c_game();