#include <Windows.h>


namespace PlayerColor {
	float BoxVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float BoxNotVisible[4] = { 1.f, 0.0f, 0.0f, 1.0f };
	float targetcol[4] = { 1.f, 0, 0, 1.0f };
	
	float accentcolor[4] = { 1.f, 1.f, 1.f, 1.0f };

	float SkeletonVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float SkeletonNotVisible[4] =  { 1.f, 0.0f, 0.0f, 1.0f };

	float SnaplineVisible[4];
	float SnaplineNotVisible[4];

	float TopTextVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float TopTextNotVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };

	float BottomTextVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
	float BottomTextNotVisible[4] = { 1.f, 1.0f, 1.0f, 1.0f };
}

namespace rifle
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool render_fov = 0;

	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

namespace shotgun
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool render_fov = 0;

	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;



}

namespace smg
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool render_fov = 0;

	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline 	float aim_fov = 15;
	inline 	float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;


}

namespace sniper
{

	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool render_fov = 0;

	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline	float aim_fov = 15;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;



}


class c_globals {
public:
	int width, height;
	bool render_menu = 0;
	HWND hwnd = 0;


	bool pred1 = true;
	bool pred2 = false;
	bool pred3 = false;

	bool skeletons = 1;
	bool bounding_box = 1;
	bool username = 0;
	bool plat = 0;

	bool distance = 1;
	bool enemiesaround = 0;
	bool outline = 1;
	bool text_outline = 0;

	int font_size = 20;
	bool finger = 0;
	int add = 3;
	bool head = 0;
	bool weapon_rarity = 0;
	bool snaplines = 0;
	bool Cornerbox = 0;
	int Smoothing = 1;
	bool weapon_esp = 0;
	bool aimbot = 1;
	bool memory = 0;

	bool HeadBone = 1;
	bool target_line = 0;
	bool target_text = 0;
	bool player_info = 0;

	bool NeckBone = 0;
	bool ChestBone = 0;
	bool PelvisBone = 0;
	bool chests = 0;
	bool testt_darw_loot = 0;

	bool ammo_box = 0;
	bool pickups = 0;
	bool uncommon = 0;
	bool commom = 0;
	bool rare = 0;
	bool epic = 0;
	bool lengendery = 0;
	bool mythic = 0;

	bool vehicle = 0;
	bool Lamma = 0;
	int max_distance = 100;
	int max_distance1 = 100;
	int max_distance2 = 250;
	int max_distance3 = 500;
	int max_distance4 = 100;


	int smooth = 2.0f;
	int field_of_view = 150.0f;
	bool render_field_of_view = 1;
	bool rendercrosshair = 0;
	bool rendercrosshairefk = 0;

	bool magicbullet = 0;
	bool Slowmo = 0;
	bool PlayerFly = 0;
	bool FirstPerson = 0;
	bool WireFrame2 = 0;
	bool BigPlayer = 0;
	bool creativemod = 0;
	bool bcheatflying = 0;
	bool niggerfly = 0;
	bool creativefly = 0;
	bool teleportplayers = 0;
	bool noclip = 0;
	bool noclipinit = 0;
	bool CreativeZipline = 0;
	bool Ziplinenigga = 0;
	bool anotherzipline = 0;
	bool nigger = 0;
	bool VehicleFly = 0;
	bool InstantRevive = 0;
	float speed = 30;
	float PlayerFlySpeedValue = 100;
	float BigNiggersFloat = 2.50;


	bool normal_fov = 1;
	bool star_fov = 0;
	bool chrome_fov = 0;
	bool dick_fov = 0;
	bool triangle_fov = 0;
	bool naze_fov = 0;
	bool weapon_cfg = 0;
	int aimbot_bone = 0;
	bool prediction = 1;

	bool color_crosshair = 1;
	bool box_color = 1;
	
	int aimbot_key = VK_MENU;
	int triggerbot_key;
	bool showSelectKeyModal;
	bool DoVisibleCheck = 1;
	bool keyselected;
	bool triggerbot = 1;
	int custom_delay = 1;
	int maximum_distance = 20;
	bool has_clicked;
	std::chrono::steady_clock::time_point tb_begin;
	std::chrono::steady_clock::time_point tb_end;
	int tb_time_since;

	int Linethick = 2.f;

	int box_thick = 1.f, skeleton_thick = 2.f;
}; static c_globals* globals = new c_globals();