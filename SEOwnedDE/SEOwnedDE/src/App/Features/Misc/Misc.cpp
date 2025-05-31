#include "Misc.h"

#include "../CFG.h"

void CMisc::Bunnyhop(CUserCmd* pCmd)
{
	if (!CFG::Misc_Bunnyhop)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet))
			return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !(pLocal->m_fFlags() & FL_ONGROUND))
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
		{
			bJumpState = true;
		}
	}
}

void CMisc::AutoStrafer(CUserCmd* pCmd)
{
	//credits: KGB

	if (!CFG::Misc_Auto_Strafe)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || (pLocal->m_fFlags() & FL_ONGROUND))
			return;

		if (pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet) || pLocal->GetMoveType() != MOVETYPE_WALK)
			return;

		if (!(pLocal->m_afButtonLast() & IN_JUMP) && (pCmd->buttons & IN_JUMP))
			return;

		if (pCmd->buttons & IN_MOVELEFT || pCmd->buttons & IN_MOVERIGHT || pCmd->buttons & IN_FORWARD || pCmd->buttons & IN_BACK)
		{
			const float flForwardMove = pCmd->forwardmove;
			const float flSideMove = pCmd->sidemove;

			Vec3 vForward = {}, vRight = {};
			Math::AngleVectors(pCmd->viewangles, &vForward, &vRight, nullptr);

			vForward.z = vRight.z = 0.0f;

			vForward.Normalize();
			vRight.Normalize();

			Vec3 vWishDir = {};
			Math::VectorAngles({(vForward.x * flForwardMove) + (vRight.x * flSideMove), (vForward.y * flForwardMove) + (vRight.y * flSideMove), 0.0f}, vWishDir);

			Vec3 vCurDir = {};
			Math::VectorAngles(pLocal->m_vecVelocity(), vCurDir);

			const float flDirDelta = Math::NormalizeAngle(vWishDir.y - vCurDir.y);
			const float flTurnScale = Math::RemapValClamped(CFG::Misc_Auto_Strafe_Turn_Scale, 0.0f, 1.0f, 0.9f, 1.0f);
			const float flRotation = DEG2RAD((flDirDelta > 0.0f ? -90.0f : 90.f) + (flDirDelta * flTurnScale));

			const float flCosRot = cosf(flRotation);
			const float flSinRot = sinf(flRotation);

			pCmd->forwardmove = (flCosRot * flForwardMove) - (flSinRot * flSideMove);
			pCmd->sidemove = (flSinRot * flForwardMove) + (flCosRot * flSideMove);
		}
	}
}

void CMisc::NoiseMakerSpam()
{
	if (!CFG::Misc_NoiseMaker_Spam)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || pLocal->m_flNextNoiseMakerTime() >= I::GlobalVars->curtime)
			return;

		const auto kv = new KeyValues("use_action_slot_item_server");
		I::EngineClient->ServerCmdKeyValues(kv);
	}
}

void CMisc::FastStop(CUserCmd* pCmd)
{
	if (!CFG::Misc_Fast_Stop)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (!pLocal->deadflag()
			&& pLocal->GetMoveType() == MOVETYPE_WALK
			&& (pLocal->m_fFlags() & FL_ONGROUND)
			&& pLocal->m_vecVelocity().Length2D() >= 10.0f
			&& !(pCmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT)))
		{
			const Vector velocity = pLocal->m_vecVelocity();
			QAngle direction;

			Math::VectorAngles(velocity, direction);

			const float speed = velocity.Length();

			direction.y = I::EngineClient->GetViewAngles().y - direction.y;

			Vec3 forward{};

			Math::AngleVectors(direction, &forward);

			const Vector negatedDirection = forward * -speed;

			pCmd->forwardmove = negatedDirection.x;
			pCmd->sidemove = negatedDirection.y;
		}
	}
}

void CMisc::AutoRocketJump(CUserCmd* cmd)
{
	if (!H::Input->IsDown(CFG::Misc_Auto_Rocket_Jump_Key)
		|| I::EngineVGui->IsGameUIVisible()
		|| I::MatSystemSurface->IsCursorVisible()
		|| SDKUtils::BInEndOfMatch())
	{
		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local
		|| local->deadflag()
		|| local->GetMoveType() != MOVETYPE_WALK
		|| local->m_nWaterLevel() > WL_Feet
		|| local->m_fFlags() & FL_DUCKING
		|| !(local->m_fFlags() & FL_ONGROUND)
		|| local->InCond(TF_COND_TAUNTING)
		|| local->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| local->InCond(TF_COND_HALLOWEEN_BOMB_HEAD)
		|| local->InCond(TF_COND_HALLOWEEN_KART))
	{
		return;
	}

	const auto weapon{H::Entities->GetWeapon()};

	if (!weapon
		|| (weapon->GetWeaponID() != TF_WEAPON_ROCKETLAUNCHER && weapon->GetWeaponID() != TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT)
		|| weapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka
		|| !weapon->HasPrimaryAmmoForShot())
	{
		return;
	}

	if (weapon->m_iReloadMode() != 0)
	{
		cmd->buttons |= IN_ATTACK;
	}

	if (local->m_vecViewOffset().z > 45.65f)
	{
		cmd->buttons |= IN_DUCK;
	}

	else
	{
		auto pitch{Math::RemapValClamped(I::EngineClient->GetViewAngles().x, -89.0f, 0.0f, 89.0f, 50.0f)};
		auto yaw{Math::NormalizeAngle(Math::VelocityToAngles(local->m_vecVelocity()).y + 180.0f)};

		if (!(cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
		{
			yaw = Math::NormalizeAngle(I::EngineClient->GetViewAngles().y + 180.0f);
		}

		cmd->viewangles.Set(pitch, yaw, weapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal ? 90.0f : 0.0f);

		cmd->buttons |= IN_ATTACK | IN_DUCK | IN_JUMP;
		cmd->buttons &= ~IN_ATTACK2;

		G::bSilentAngles = true;
	}
}

void CMisc::AutoDisguise(CUserCmd* cmd)
{
	if (!CFG::Misc_Auto_Disguise || I::GlobalVars->tickcount % 20 != 0)
	{
		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local || local->deadflag() || local->m_iClass() != TF_CLASS_SPY || local->InCond(TF_COND_DISGUISED) || local->InCond(TF_COND_DISGUISING))
	{
		return;
	}

	I::EngineClient->ClientCmd_Unrestricted("lastdisguise");
}

void CMisc::AutoMedigun(CUserCmd* cmd)
{
	static std::vector<C_TFPlayer*> potential{};

	if (!H::Input->IsDown(CFG::Misc_Auto_Medigun_Key))
	{
		if (!potential.empty())
		{
			potential.clear();
		}

		return;
	}

	if (I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible() || SDKUtils::BInEndOfMatch())
	{
		potential.clear();

		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local
		|| local->deadflag()
		|| local->InCond(TF_COND_TAUNTING)
		|| local->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| local->InCond(TF_COND_HALLOWEEN_BOMB_HEAD)
		|| local->InCond(TF_COND_HALLOWEEN_KART))
	{
		potential.clear();

		return;
	}

	const auto weapon{H::Entities->GetWeapon()};

	if (!weapon || weapon->GetWeaponID() != TF_WEAPON_MEDIGUN)
	{
		potential.clear();

		return;
	}

	const auto medigun{weapon->As<C_WeaponMedigun>()};

	if (!medigun)
	{
		potential.clear();

		return;
	}

	auto isPlayerGood = [&](C_TFPlayer* pl)
	{
		if (!pl || pl->deadflag() || pl->GetCenter().DistTo(local->GetShootPos()) > 449.0f || pl->InCond(TF_COND_STEALTHED))
		{
			return false;
		}

		if (medigun->m_hHealingTarget().Get())
		{
			static C_BaseEntity* last{};

			if (medigun->m_bChargeRelease())
			{
				if (medigun->m_hHealingTarget().Get() == pl)
				{
					return false;
				}
			}

			else
			{
				if (pl->IsInvulnerable())
				{
					return false;
				}

				if (medigun->m_hHealingTarget().Get() == pl && last == pl)
				{
					return false;
				}

				auto mult{1.44f};

				if (medigun->GetMedigunType() == MEDIGUN_QUICKFIX)
				{
					mult = 1.24f;
				}

				if (static_cast<float>(pl->m_iHealth()) >= static_cast<float>(pl->GetMaxHealth()) * mult)
				{
					return false;
				}
			}

			last = medigun->m_hHealingTarget().Get();
		}

		CTraceFilterHitscan filter{};
		trace_t tr{};

		H::AimUtils->Trace(local->GetShootPos(), pl->GetCenter(), (MASK_SHOT & ~CONTENTS_HITBOX), &filter, &tr);

		return tr.fraction > 0.99f || tr.m_pEnt == pl;
	};

	if (potential.empty())
	{
		for (const auto ent : H::Entities->GetGroup(EEntGroup::PLAYERS_TEAMMATES))
		{
			if (!ent || ent == local)
			{
				continue;
			}

			auto pl{ent->As<C_TFPlayer>()};

			if (!isPlayerGood(pl))
			{
				continue;
			}

			potential.push_back(pl);
		}
	}

	if ((cmd->tick_count % 2) && G::bCanPrimaryAttack && !potential.empty())
	{
		for (int n{0}; n < static_cast<int>(potential.size()); n++)
		{
			const auto pl{potential[n]};

			if (!isPlayerGood(pl))
			{
				potential.erase(potential.begin() + n);

				continue;
			}

			const auto angle{Math::CalcAngle(local->GetShootPos(), pl->GetCenter())};

			H::AimUtils->FixMovement(cmd, angle);

			cmd->viewangles = angle;
			cmd->buttons |= IN_ATTACK;

			G::bPSilentAngles = true;

			if (CFG::Misc_Accuracy_Improvements)
			{
				cmd->tick_count = TIME_TO_TICKS(pl->m_flSimulationTime() + SDKUtils::GetLerp());
			}

			break;
		}
	}

	else
	{
		cmd->buttons &= ~IN_ATTACK;
	}
}

void CMisc::MovementLock(CUserCmd* cmd)
{
	static auto active{false};

	if (!CFG::Misc_Movement_Lock_Key)
	{
		active = false;

		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local || (local->m_fFlags() & FL_ONGROUND))
	{
		active = false;

		return;
	}

	const auto& vel{local->m_vecVelocity()};
	static Vec3 lastTickVel{};

	static auto angles{cmd->viewangles};

	if (!active
		&& static_cast<int>(vel.x) == 0 && static_cast<int>(vel.y) == 0 && static_cast<int>(vel.z) == -6
		&& static_cast<int>(lastTickVel.x) == 0 && static_cast<int>(lastTickVel.y) == 0 && static_cast<int>(lastTickVel.z) == -6)
	{
		active = true;

		angles = cmd->viewangles;

		if (cmd->buttons & IN_MOVELEFT)
		{
			angles.y += 45.0f;
		}

		if (cmd->buttons & IN_MOVERIGHT)
		{
			angles.y -= 45.0f;
		}
	}

	lastTickVel = vel;

	if (active && static_cast<int>(vel.x) != 0 && static_cast<int>(vel.y) != 0 && static_cast<int>(vel.z) != -6)
	{
		active = false;
	}

	if (H::Input->IsPressed(CFG::Misc_Movement_Lock_Key) && !I::MatSystemSurface->IsCursorVisible() && !I::EngineVGui->IsGameUIVisible())
	{
		active = false;
	}

	if (!active)
	{
		return;
	}

	const auto angle = DEG2RAD((angles.y - cmd->viewangles.y) + 90.0f);

	cmd->forwardmove = sinf(angle) * 450.0f;
	cmd->sidemove = cosf(angle) * 450.0f;
}

void CMisc::MvmInstaRespawn()
{
	if (!H::Input->IsDown(CFG::Misc_MVM_Instant_Respawn_Key)
		|| I::EngineVGui->IsGameUIVisible()
		|| I::MatSystemSurface->IsCursorVisible()
		|| SDKUtils::BInEndOfMatch())
	{
		return;
	}

	auto* kv{new KeyValues("MVM_Revive_Response")};

	kv->SetInt("accepted", 1);

	I::EngineClient->ServerCmdKeyValues(kv);
}
