#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

#include "../Features/Aimbot/Aimbot.h"
#include "../Features/EnginePrediction/EnginePrediction.h"
#include "../Features/Misc/Misc.h"
#include "../Features/RapidFire/RapidFire.h"
#include "../Features/Triggerbot/Triggerbot.h"
#include "../Features/Triggerbot/AutoVaccinator/AutoVaccinator.h"
#include "../Features/SeedPred/SeedPred.h"
#include "../Features/StickyJump/StickyJump.h"
#include "../Features/Crits/Crits.h"

MAKE_HOOK(ClientModeShared_CreateMove, Memory::GetVFunc(I::ClientModeShared, 21), bool, __fastcall,
	CClientModeShared* ecx, float flInputSampleTime, CUserCmd* pCmd)
{
	G::bSilentAngles = false;
	G::bPSilentAngles = false;
	G::bFiring = false;

	if (!pCmd || !pCmd->command_number)
	{
		return CALL_ORIGINAL(ecx, flInputSampleTime, pCmd);
	}

	I::Prediction->Update
	(
		I::ClientState->m_nDeltaTick,
		I::ClientState->m_nDeltaTick > 0,
		I::ClientState->last_command_ack,
		I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands
	);

	F::AutoVaccinator->PreventReload(pCmd);

	if (F::RapidFire->ShouldExitCreateMove(pCmd))
	{
		F::Crits->Run(pCmd);

		return F::RapidFire->GetShiftSilentAngles() ? false : CALL_ORIGINAL(ecx, flInputSampleTime, pCmd);
	}

	if (Shifting::bRecharging)
	{
		if (pCmd->buttons & IN_JUMP)
		{
			pCmd->buttons &= ~IN_JUMP;
		}

		return CALL_ORIGINAL(ecx, flInputSampleTime, pCmd);
	}

	bool* pSendPacket = reinterpret_cast<bool*>(uintptr_t(_AddressOfReturnAddress()) + 0x128);

	Vec3 vOldAngles = pCmd->viewangles;
	float flOldSide = pCmd->sidemove;
	float flOldForward = pCmd->forwardmove;

	if (auto pLocal = H::Entities->GetLocal())
	{
		if (auto pWeapon = H::Entities->GetWeapon())
		{
			//TODO?: do we really need to cache these?
			G::bCanPrimaryAttack = pWeapon->CanPrimaryAttack(pLocal);
			G::bCanSecondaryAttack = pWeapon->CanSecondaryAttack(pLocal);
			G::bCanHeadshot = pWeapon->CanHeadShot(pLocal);
		}
	}

	//nTicksSinceCanFire
	{
		static bool bOldCanFire = G::bCanPrimaryAttack;

		if (G::bCanPrimaryAttack != bOldCanFire)
		{
			G::nTicksSinceCanFire = 0;
			bOldCanFire = G::bCanPrimaryAttack;
		}

		else
		{
			if (G::bCanPrimaryAttack)
				G::nTicksSinceCanFire++;

			else G::nTicksSinceCanFire = 0;
		}
	}

	F::Misc->Bunnyhop(pCmd);
	F::Misc->AutoStrafer(pCmd);
	F::Misc->FastStop(pCmd);
	F::Misc->NoiseMakerSpam();
	F::Misc->AutoRocketJump(pCmd);
	F::Misc->AutoDisguise(pCmd);
	F::Misc->MovementLock(pCmd);
	F::Misc->MvmInstaRespawn();

	F::EnginePrediction->Start(pCmd);
	{
		if (CFG::Misc_Choke_On_Bhop && CFG::Misc_Bunnyhop)
		{
			if (C_TFPlayer* const local{ H::Entities->GetLocal() })
			{
				if ((local->m_fFlags() & FL_ONGROUND) && !(F::EnginePrediction->flags & FL_ONGROUND))
				{
					*pSendPacket = false;
				}
			}
		}

		F::Misc->AutoMedigun(pCmd);
		F::Aimbot->Run(pCmd);
		F::Triggerbot->Run(pCmd);
	}
	F::EnginePrediction->End();

	F::SeedPred->AdjustAngles(pCmd);
	F::StickyJump->Run(pCmd);

	F::Crits->Run(pCmd);

	//nTicksTargetSame
	{
		static int nOldTargetIndex = G::nTargetIndexEarly;

		if (G::nTargetIndexEarly != nOldTargetIndex)
		{
			G::nTicksTargetSame = 0;
			nOldTargetIndex = G::nTargetIndexEarly;
		}

		else
		{
			G::nTicksTargetSame++;
		}

		if (G::nTargetIndexEarly <= 1)
			G::nTicksTargetSame = 0;
	}

	/* Taunt Slide */
	if (CFG::Misc_Taunt_Slide)
	{
		if (auto pLocal = H::Entities->GetLocal())
		{
			if (pLocal->InCond(TF_COND_TAUNTING) && pLocal->m_bAllowMoveDuringTaunt())
			{
				static float flYaw = pCmd->viewangles.y;

				if (H::Input->IsDown(CFG::Misc_Taunt_Spin_Key) && fabsf(CFG::Misc_Taunt_Spin_Speed))
				{
					float yaw{ CFG::Misc_Taunt_Spin_Speed };

					if (CFG::Misc_Taunt_Spin_Sine)
					{
						yaw = sinf(I::GlobalVars->curtime) * CFG::Misc_Taunt_Spin_Speed;
					}

					flYaw -= yaw;
					flYaw = Math::NormalizeAngle(flYaw);

					pCmd->viewangles.y = flYaw;
				}

				else
				{
					flYaw = pCmd->viewangles.y;
				}

				if (CFG::Misc_Taunt_Slide_Control)
					pCmd->viewangles.x = (pCmd->buttons & IN_BACK) ? 91.0f : (pCmd->buttons & IN_FORWARD) ? 0.0f : 90.0f;

				G::bSilentAngles = true;
			}
		}
	}

	/* Warp */
	if (CFG::Exploits_Warp_Exploit && CFG::Exploits_Warp_Mode == 1 && Shifting::bShiftingWarp)
	{
		if (auto pLocal = H::Entities->GetLocal())
		{
			if (CFG::Exploits_Warp_Exploit == 1)
			{
				if (Shifting::nAvailableTicks <= (MAX_COMMANDS - 1))
				{
					Vec3 vAngle = {};
					Math::VectorAngles(pLocal->m_vecVelocity(), vAngle);
					pCmd->viewangles.x = 90.0f;
					pCmd->viewangles.y = vAngle.y;
					G::bSilentAngles = true;
					pCmd->sidemove = pCmd->forwardmove = 0;
				}
			}

			if (CFG::Exploits_Warp_Exploit == 2)
			{
				if (Shifting::nAvailableTicks <= 1)
				{
					Vec3 vAngle = {};
					Math::VectorAngles(pLocal->m_vecVelocity(), vAngle);
					pCmd->viewangles.x = 90.0f;
					pCmd->viewangles.y = vAngle.y;
					G::bSilentAngles = true;
					pCmd->sidemove = pCmd->forwardmove = 0;
				}
			}
		}
	}

	//pSilent
	{
		static bool bWasSet = false;

		if (G::bPSilentAngles)
		{
			*pSendPacket = false;
			bWasSet = true;
		}

		else
		{
			if (bWasSet)
			{
				*pSendPacket = true;
				pCmd->viewangles = vOldAngles;
				pCmd->sidemove = flOldSide;
				pCmd->forwardmove = flOldForward;
				bWasSet = false;
			}
		}
	}

	// Don't choke too much
	if (I::ClientState->chokedcommands > 22)
	{
		*pSendPacket = true;
	}

	F::RapidFire->Run(pCmd, pSendPacket);

	G::nOldButtons = pCmd->buttons;
	G::vUserCmdAngles = pCmd->viewangles;

	return (G::bSilentAngles || G::bPSilentAngles) ? false : CALL_ORIGINAL(ecx, flInputSampleTime, pCmd);
}
