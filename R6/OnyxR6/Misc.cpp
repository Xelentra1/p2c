#pragma once
#include "Misc.h"
#include "Shared.h"
#include "Vectors.h"

void Misc::UnlockAll(bool enabled) {
	auto instruction = Offsets::base() + offset_unlocks;
	if (change_protection(currentPID(), instruction, PAGE_EXECUTE_READWRITE, 4) == 0)
	{
		Write<std::uint8_t>(instruction + 3, enabled ? 0 : 1);

		change_protection(currentPID(), instruction, PAGE_EXECUTE_READ, 4);
	}
}

void Misc::TriggerThread() {
	//if (!Offsets::isInGame()) return;
	while (true) {
		if (Offsets::isInGame() && CS::ID(TriggerBot_ID)) {
			uintptr_t forcefireChain = ReadChain(Offsets::base(), std::vector<uintptr_t>{ 0x51B87B0, 0x68, 0x0, 0x28, 0x30, 0x90 });
			uintptr_t crossChain = ReadChain(Offsets::base(), std::vector<uintptr_t>{ offset_trigger_manager, 0x828 });
			//Print("Got initial offsets");

			bool isShooting = true;

			while (Offsets::isInGame() && CS::ID(TriggerBot_ID)) {
				uintptr_t crossID = ReadChain(crossChain, std::vector<uintptr_t>{ 0x80, 0x0 });

				while (crossID != 0) { // If player in crosshair
					//Print("aimed at player");
					uintptr_t teamChain = Read<uintptr_t>(crossID + 0x28);
					int forceFireValue = Read<int>(forcefireChain + 0xca0);
					if ((!isShooting || forceFireValue != 1) && teamChain != NULL) {
						isShooting = true;
						//if (forceFireValue != 1) Sleep(10);
						//Print("Shooting");
						Write<int>(forcefireChain + 0xca0, 1);
					}

					crossID = ReadChain(crossChain, std::vector<uintptr_t>{ 0x80, 0x0 });

					Sleep(10);
				}
				if (isShooting) {
					isShooting = false;
					Write<int>(forcefireChain + 0xca0, 0);
				}
				Sleep(10);
			}
		}
		else if (!Offsets::isInGame() && CS::ID(TriggerBot_ID)) {
			CT::ID(TriggerBot_ID);
		}
		Sleep(1000);
	}
}

#define OPCODE_BULLETANGLE 0xFA5431
#define OFFSET_VIEWANGLES_BULLETANGLES 0x134
void PatchAim() {
	unsigned long long BulletAngle = (unsigned long long)(Offsets::base() + OPCODE_BULLETANGLE + 0x03);

	if (change_protection(currentPID(), BulletAngle, PAGE_EXECUTE_READWRITE, sizeof(unsigned long)) == 0)
	{
		Write<unsigned long>(BulletAngle, OFFSET_VIEWANGLES_BULLETANGLES);

		change_protection(currentPID(), BulletAngle, PAGE_EXECUTE_READ, sizeof(unsigned long));
	}
}


// Experimental

/*
constexpr uintptr_t status_manager = 0x51cb6e0;
constexpr uintptr_t status_manager_container = 0x68;
constexpr uintptr_t status_manager_localentity = 0x28;

constexpr uintptr_t  offset_status_manager_localentity = 0x28;
constexpr uintptr_t  offset_status_manager_container = 0x370;
*/

uintptr_t local_entity() {

	uintptr_t status_manager = Read<uintptr_t>(Offsets::base() + 0x51cb6e0); // status manager

	if (!status_manager)
		return 0;

	uintptr_t entity_container = Read<uintptr_t>(status_manager + 0x68); // status manager base

	if (!entity_container)
		return 0;

	entity_container = Read<uintptr_t>(entity_container);
	if (!entity_container)
		return 0;

	entity_container = Read<uintptr_t>(entity_container + 0x28); // offset_status_manager_localentity

	if (!entity_container)
		return 0;

	return entity_container;
}

unsigned long entity_count() {
	uintptr_t game_manager = Offsets::gameManager();
	if (!game_manager)
		return NULL;

	return Read<unsigned long>(game_manager + offset_object_list_length) & 0x3fffffff;
}

uintptr_t entity_id(unsigned int id) {
	uintptr_t game_manager = Offsets::gameManager();
	if (!game_manager)
		return NULL;

	uintptr_t entity_list = Read<uintptr_t>(game_manager + offset_object_list);
	if (!entity_list)
		return NULL;

	uintptr_t entity = Read<uintptr_t>(entity_list + (sizeof(PVOID) * id));
	if (!entity)
		return NULL;

	return entity;
}

std::uint8_t entity_team(uintptr_t entity) {

	if (!entity)
		return 0xFF;

	uintptr_t replication = Read<uintptr_t>(entity + 0xa8); // offsets::entity_replication
	//std::cout << "replication " << replication << std::endl;
	if (!replication) {
		return 0xFF;
	}

	unsigned long online_team_id = Read<unsigned long>(replication + 0x19e); // offsets::entity_replication_team
	//std::cout << "online_team_id " << online_team_id << std::endl;
	if (!online_team_id) {
		return 0xFF;
	}

	return online_team_id;
}

uint32_t entity_health(uintptr_t entity)
{
	auto r1 = Read<uintptr_t>(entity + 0x28);
	auto r2 = Read<uintptr_t>(r1 + 0xd8);
	auto r3 = Read<uintptr_t>(r2 + 0x8);
	return Read<uint32_t>(r3 + 0x148);
}

std::uintptr_t closest_to_fov(Vector3 source_head, Vector3 entity_head, Vector3 source_angle)
{
	const auto calc_angle = [](Vector3 src, Vector3 dest)
	{
		auto delta = src - dest;

		auto y = -asin(delta.z / src.Distance(dest)) * 57.2957795131f;
		auto x = (atan2(delta.y, delta.x) * 57.2957795131f) + 90.f;

		if (x > 180.f) x -= 360.f;
		else if (x < -180.f) x += 360.f;

		auto clamp = [](Vector3& angle)
		{
			if (angle.y > 75.f) angle.y = 75.f;
			else if (angle.y < -75.f) angle.y = -75.f;
			if (angle.x < -180.f) angle.x += -360.f;
			else if (angle.x > 180.f) angle.x -= 360.f;

			angle.z = 0.f;
		};

		auto angle = Vector3(x, y, 0.f);
		clamp(angle);

		return angle;
	};

	auto aim_angle = calc_angle(source_head, entity_head);

	auto calc_fov = [](Vector3 src, Vector3 aim)
	{
		aim -= src;

		if (aim.x > 180.f)
			aim.x -= 360.f;
		else if (aim.x < -180.f)
			aim.x += 360.f;
		if (aim.y > 180.f)
			aim.y -= 360.f;
		else if (aim.y < -180.f)
			aim.y += 360.f;

		return aim;
	};

	auto angle = calc_fov(source_angle, aim_angle);

	angle.x = std::abs(angle.x);
	angle.y = std::abs(angle.y);

	return angle.x + angle.y;
}

Vector3 calculate_euler(Vector4 quat)
{
	auto y_p2 = quat.y * quat.y;

	auto x = std::atan2(2.f * (quat.w * quat.z + quat.x * quat.y), (1.f - 2.f * (y_p2 + quat.z * quat.z))) * 57.2957795131f;
	auto y = std::atan2(2.f * (quat.w * quat.x + quat.y * quat.z), (1.f - 2.f * (quat.x * quat.x + y_p2))) * 57.2957795131f;

	return Vector3(x, y, 0.f);
}

Vector3 entity_head(uintptr_t player)
{
	uint64_t pSkeleton = Read<uint64_t>(player + 0x20);

	if (!pSkeleton)
		return Vector3();

	return Read<Vector3>(pSkeleton + 0x660); // Ox6A0 // offsets::offset_skeleton_headposition
}

uintptr_t get_closest_enemy()
{
	const auto getViewangle = [](std::uintptr_t entity)
	{
		auto r1 = Read<uintptr_t>(entity + 0x20);
		auto r2 = Read<uintptr_t>(r1 + 0x1170);
		return Read<Vector4>(r2 + 0xc0);
	};

	std::uintptr_t resultant_entity = 0;
	static auto resultant_fov = 360.f;

	for (auto i = 0ul; i < entity_count(); i++)
	{
		const auto entity = entity_id(i);

		const auto health = entity_health(entity);

		if (health <= 0 || health > 200)
			continue;

		const auto lentity = local_entity();

		if (entity_team(lentity) == entity_team(entity))
			continue;

		const auto fov_result = closest_to_fov(entity_head(lentity), entity_head(entity), calculate_euler(getViewangle(lentity)));

		if (fov_result < resultant_fov)
		{
			resultant_fov = fov_result;
			resultant_entity = entity;
		}
	}

	return resultant_entity;
}

void ChangeView() {
	PatchAim();

}

/*
void Aimbot(int AimType) // 0 -> aimbot, 1 -> spinbot
{
	unsigned long long lEntity = GetLocalPlayer();

	if (GetEntityHealth(lEntity) > 0)
	{
		BYTE lTeam = GetEntityTeam(lEntity);
		BYTE pTeam;
		int pHealth;
		Vector3 pTargetPos;

		if (AimType == 0)
		{
			unsigned long long pEntity = get_closest_enemy();

			if (!pEntity)
				return;

			pTeam = GetEntityTeam(pEntity);

			if (pTeam != lTeam)
			{
				pTargetPos = Vector3();

				if (!GetAsyncKeyState(Keybinds::handAimbotKey))
				{
					switch (AimbotSettings::iBone)
					{
					case BONE_HEAD:
						pTargetPos = GetEntityHeadPosition(pEntity);
						break;
					case BONE_NECK:
						pTargetPos = GetEntityNeckPosition(pEntity);
						break;
					case BONE_RIGHTHAND:
						pTargetPos = GetEntityRightHandPosition(pEntity);
						break;
					case BONE_CHEST:
						pTargetPos = GetEntityChestPosition(pEntity);
						break;
					case BONE_STOMACH:
						pTargetPos = GetEntityStomachPosition(pEntity);
						break;
					case BONE_PELVIS:
						pTargetPos = GetEntityPelvisPosition(pEntity);
						break;
					default:
						pTargetPos = GetEntityHeadPosition(pEntity);
						break;
					}
				}
				else
					pTargetPos = GetEntityRightHandPosition(pEntity);

				float DistanceFromCross = W2SDistance(pTargetPos);
				Vector4 ViewAngles = GetViewAngles(lEntity);
				Vector3 ViewTranslation = Camera::ViewTranslation;

				if (DistanceFromCross <= AimbotSettings::fov || !AimbotSettings::fovCheckEnabled)
				{
					if (AimbotSettings::silentAim)
					{
						// calculate difference from 0x134 angles to enemy 
						// set 0x134 angles
					}

					else
					{
						// calculate difference from 0xC0 angles to enemy 
						// set 0x134 angles
						// set 0xC0 angles
					}
				}

				else
					// set 0x134 angles to 0xC0 angles
			}
		}

		else if (AimType == 1)
		{
			Vector4 qAngles = GetViewAngles(lEntity);
			Vector3 ViewTranslation = Camera::ViewTranslation;

			if (AimbotSettings::aimbotTarget >= Global::EntityCount)
				AimbotSettings::aimbotTarget = 0;

			unsigned long long targetEntity = GetEntity(AimbotSettings::aimbotTarget);

			pTeam = GetEntityTeam(targetEntity);
			pHealth = GetEntityHealth(targetEntity);

			pTargetPos = Vector3();

			if (!GetAsyncKeyState(Keybinds::handAimbotKey))
			{
				switch (AimbotSettings::iBone)
				{
				case BONE_HEAD:
					pTargetPos = GetEntityHeadPosition(targetEntity);
					break;
				case BONE_NECK:
					pTargetPos = GetEntityNeckPosition(targetEntity);
					break;
				case BONE_RIGHTHAND:
					pTargetPos = GetEntityRightHandPosition(targetEntity);
					break;
				case BONE_CHEST:
					pTargetPos = GetEntityChestPosition(targetEntity);
					break;
				case BONE_STOMACH:
					pTargetPos = GetEntityStomachPosition(targetEntity);
					break;
				case BONE_PELVIS:
					pTargetPos = GetEntityPelvisPosition(targetEntity);
					break;
				default:
					pTargetPos = GetEntityHeadPosition(targetEntity);
					break;
				}
			}
			else
				pTargetPos = GetEntityRightHandPosition(targetEntity);

			if (!targetEntity || pHealth < 1 || pHealth > 700 || targetEntity == lEntity || (pTeam == lTeam && !AimbotSettings::aimbotOnTeam))
			{
				AimbotSettings::aimbotTarget++;
				return;
			}

			if (AimbotSettings::silentAim)
			{
				// calculate difference from 0x134 angles to enemy 
				// set 0x134 angles
			}

			else
			{
				// calculate difference from 0xC0 angles to enemy 
				// set 0x134 angles
				// set 0xC0 angles
			}

			AimbotSettings::aimbotTarget++;
		}
	}
	
}
*/