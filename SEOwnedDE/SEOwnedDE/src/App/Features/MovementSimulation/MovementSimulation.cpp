#include "MovementSimulation.h"

#include "../LagRecords/LagRecords.h"

#include "../CFG.h"

void CMovementSimulation::CPlayerDataBackup::Store(C_TFPlayer* pPlayer)
{
	m_vecOrigin = pPlayer->m_vecOrigin();
	m_vecVelocity = pPlayer->m_vecVelocity();
	m_vecBaseVelocity = pPlayer->m_vecBaseVelocity();
	m_vecViewOffset = pPlayer->m_vecViewOffset();
	m_hGroundEntity = pPlayer->m_hGroundEntity();
	m_fFlags = pPlayer->m_fFlags();
	m_flDucktime = pPlayer->m_flDucktime();
	m_flDuckJumpTime = pPlayer->m_flDuckJumpTime();
	m_bDucked = pPlayer->m_bDucked();
	m_bDucking = pPlayer->m_bDucking();
	m_bInDuckJump = pPlayer->m_bInDuckJump();
	m_flModelScale = pPlayer->m_flModelScale();
	m_nButtons = pPlayer->m_nButtons();
	m_flLastMovementStunChange = pPlayer->m_flLastMovementStunChange();
	m_flStunLerpTarget = pPlayer->m_flStunLerpTarget();
	m_bStunNeedsFadeOut = pPlayer->m_bStunNeedsFadeOut();
	m_flPrevTauntYaw = pPlayer->m_flPrevTauntYaw();
	m_flTauntYaw = pPlayer->m_flTauntYaw();
	m_flCurrentTauntMoveSpeed = pPlayer->m_flCurrentTauntMoveSpeed();
	m_iKartState = pPlayer->m_iKartState();
	m_flVehicleReverseTime = pPlayer->m_flVehicleReverseTime();
	m_flHypeMeter = pPlayer->m_flHypeMeter();
	m_flMaxspeed = pPlayer->m_flMaxspeed();
	m_nAirDucked = pPlayer->m_nAirDucked();
	m_bJumping = pPlayer->m_bJumping();
	m_iAirDash = pPlayer->m_iAirDash();
	m_flWaterJumpTime = pPlayer->m_flWaterJumpTime();
	m_flSwimSoundTime = pPlayer->m_flSwimSoundTime();
	m_surfaceProps = pPlayer->m_surfaceProps();
	m_pSurfaceData = pPlayer->m_pSurfaceData();
	m_surfaceFriction = pPlayer->m_surfaceFriction();
	m_chTextureType = pPlayer->m_chTextureType();
	m_vecPunchAngle = pPlayer->m_vecPunchAngle();
	m_vecPunchAngleVel = pPlayer->m_vecPunchAngleVel();
	m_MoveType = pPlayer->m_MoveType();
	m_MoveCollide = pPlayer->m_MoveCollide();
	m_vecLadderNormal = pPlayer->m_vecLadderNormal();
	m_flGravity = pPlayer->m_flGravity();
	m_nWaterLevel = pPlayer->m_nWaterLevel_C_BaseEntity();
	m_nWaterType = pPlayer->m_nWaterType();
	m_flFallVelocity = pPlayer->m_flFallVelocity();
	m_nPlayerCond = pPlayer->m_nPlayerCond();
	m_nPlayerCondEx = pPlayer->m_nPlayerCondEx();
	m_nPlayerCondEx2 = pPlayer->m_nPlayerCondEx2();
	m_nPlayerCondEx3 = pPlayer->m_nPlayerCondEx3();
	m_nPlayerCondEx4 = pPlayer->m_nPlayerCondEx4();
	_condition_bits = pPlayer->_condition_bits();
}

void CMovementSimulation::CPlayerDataBackup::Restore(C_TFPlayer* pPlayer)
{
	pPlayer->m_vecOrigin() = m_vecOrigin;
	pPlayer->m_vecVelocity() = m_vecVelocity;
	pPlayer->m_vecBaseVelocity() = m_vecBaseVelocity;
	pPlayer->m_vecViewOffset() = m_vecViewOffset;
	pPlayer->m_hGroundEntity() = m_hGroundEntity;
	pPlayer->m_fFlags() = m_fFlags;
	pPlayer->m_flDucktime() = m_flDucktime;
	pPlayer->m_flDuckJumpTime() = m_flDuckJumpTime;
	pPlayer->m_bDucked() = m_bDucked;
	pPlayer->m_bDucking() = m_bDucking;
	pPlayer->m_bInDuckJump() = m_bInDuckJump;
	pPlayer->m_flModelScale() = m_flModelScale;
	pPlayer->m_nButtons() = m_nButtons;
	pPlayer->m_flLastMovementStunChange() = m_flLastMovementStunChange;
	pPlayer->m_flStunLerpTarget() = m_flStunLerpTarget;
	pPlayer->m_bStunNeedsFadeOut() = m_bStunNeedsFadeOut;
	pPlayer->m_flPrevTauntYaw() = m_flPrevTauntYaw;
	pPlayer->m_flTauntYaw() = m_flTauntYaw;
	pPlayer->m_flCurrentTauntMoveSpeed() = m_flCurrentTauntMoveSpeed;
	pPlayer->m_iKartState() = m_iKartState;
	pPlayer->m_flVehicleReverseTime() = m_flVehicleReverseTime;
	pPlayer->m_flHypeMeter() = m_flHypeMeter;
	pPlayer->m_flMaxspeed() = m_flMaxspeed;
	pPlayer->m_nAirDucked() = m_nAirDucked;
	pPlayer->m_bJumping() = m_bJumping;
	pPlayer->m_iAirDash() = m_iAirDash;
	pPlayer->m_flWaterJumpTime() = m_flWaterJumpTime;
	pPlayer->m_flSwimSoundTime() = m_flSwimSoundTime;
	pPlayer->m_surfaceProps() = m_surfaceProps;
	pPlayer->m_pSurfaceData() = m_pSurfaceData;
	pPlayer->m_surfaceFriction() = m_surfaceFriction;
	pPlayer->m_chTextureType() = m_chTextureType;
	pPlayer->m_vecPunchAngle() = m_vecPunchAngle;
	pPlayer->m_vecPunchAngleVel() = m_vecPunchAngleVel;
	pPlayer->m_flJumpTime() = m_flJumpTime;
	pPlayer->m_MoveType() = m_MoveType;
	pPlayer->m_MoveCollide() = m_MoveCollide;
	pPlayer->m_vecLadderNormal() = m_vecLadderNormal;
	pPlayer->m_flGravity() = m_flGravity;
	pPlayer->m_nWaterLevel_C_BaseEntity() = m_nWaterLevel;
	pPlayer->m_nWaterType() = m_nWaterType;
	pPlayer->m_flFallVelocity() = m_flFallVelocity;
	pPlayer->m_nPlayerCond() = m_nPlayerCond;
	pPlayer->m_nPlayerCondEx() = m_nPlayerCondEx;
	pPlayer->m_nPlayerCondEx2() = m_nPlayerCondEx2;
	pPlayer->m_nPlayerCondEx3() = m_nPlayerCondEx3;
	pPlayer->m_nPlayerCondEx4() = m_nPlayerCondEx4;
	pPlayer->_condition_bits() = _condition_bits;
}

void CMovementSimulation::SetupMoveData(C_TFPlayer* pPlayer, CMoveData* pMoveData)
{
	if (!pPlayer || !pMoveData)
		return;

	pMoveData->m_bFirstRunOfFunctions = false;
	pMoveData->m_bGameCodeMovedPlayer = false;
	pMoveData->m_nPlayerHandle = pPlayer->GetRefEHandle();
	pMoveData->m_vecVelocity = pPlayer->m_vecVelocity();
	pMoveData->m_vecAbsOrigin = pPlayer->m_vecOrigin();
	pMoveData->m_flMaxSpeed = pPlayer->TeamFortress_CalculateMaxSpeed();

	if (m_PlayerDataBackup.m_fFlags & FL_DUCKING)
		pMoveData->m_flMaxSpeed *= 0.3333f;

	pMoveData->m_flClientMaxSpeed = pMoveData->m_flMaxSpeed;

	pMoveData->m_vecViewAngles = {0.0f, Math::VelocityToAngles(pMoveData->m_vecVelocity).y, 0.0f};

	if (CFG::Aimbot_Projectile_Aim_Prediction_Method == 0)
	{
		pMoveData->m_flForwardMove = 450.0f;
		pMoveData->m_flSideMove = 0.0f;
	}

	else
	{
		Vec3 vForward = {}, vRight = {};
		Math::AngleVectors(pMoveData->m_vecViewAngles, &vForward, &vRight, nullptr);

		pMoveData->m_flForwardMove = (pMoveData->m_vecVelocity.y - vRight.y / vRight.x * pMoveData->m_vecVelocity.x) / (vForward.y - vRight.y / vRight.x * vForward.x);
		pMoveData->m_flSideMove = (pMoveData->m_vecVelocity.x - vForward.x * pMoveData->m_flForwardMove) / vRight.x;
	}

	const float flSpeed = pPlayer->m_vecVelocity().Length2D();

	if (flSpeed <= pMoveData->m_flMaxSpeed * 0.1f)
		pMoveData->m_flForwardMove = pMoveData->m_flSideMove = 0.0f;

	pMoveData->m_vecAngles = pMoveData->m_vecViewAngles;
	pMoveData->m_vecOldAngles = pMoveData->m_vecAngles;

	if (pPlayer->m_hConstraintEntity())
		pMoveData->m_vecConstraintCenter = pPlayer->m_hConstraintEntity()->GetAbsOrigin();

	else pMoveData->m_vecConstraintCenter = pPlayer->m_vecConstraintCenter();

	pMoveData->m_flConstraintRadius = pPlayer->m_flConstraintRadius();
	pMoveData->m_flConstraintWidth = pPlayer->m_flConstraintWidth();
	pMoveData->m_flConstraintSpeedFactor = pPlayer->m_flConstraintSpeedFactor();

	m_flYawTurnRate = 0.0f;

	if (CFG::Aimbot_Projectile_Ground_Strafe_Prediction && (m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && F::LagRecords->HasRecords(pPlayer))
	{
		if (m_MoveData.m_vecVelocity.Length2D() < (m_MoveData.m_flMaxSpeed * 0.85f))
		{
			return;
		}

		const auto pRecord0 = F::LagRecords->GetRecord(pPlayer, 0);
		const auto pRecord1 = F::LagRecords->GetRecord(pPlayer, 1);
		const auto pRecord2 = F::LagRecords->GetRecord(pPlayer, 2);
		const auto pRecord3 = F::LagRecords->GetRecord(pPlayer, 3);
		const auto pRecord4 = F::LagRecords->GetRecord(pPlayer, 4);

		if (pRecord0 && pRecord1 && pRecord2 && pRecord3 && pRecord4)
		{
			const float flYaw0 = Math::VelocityToAngles(pRecord0->Velocity).y;
			const float flYaw1 = Math::VelocityToAngles(pRecord1->Velocity).y;
			const float flYaw2 = Math::VelocityToAngles(pRecord2->Velocity).y;
			const float flYaw3 = Math::VelocityToAngles(pRecord3->Velocity).y;
			const float flYaw4 = Math::VelocityToAngles(pRecord4->Velocity).y;

			const auto inc{flYaw4 > flYaw3 && flYaw3 > flYaw2 && flYaw2 > flYaw1 && flYaw1 > flYaw0};
			const auto dec{flYaw4 < flYaw3 && flYaw3 < flYaw2 && flYaw2 < flYaw1 && flYaw1 < flYaw0};

			if (!inc && !dec)
			{
				return;
			}

			const float flYawRate = (((flYaw0 - flYaw1) + (flYaw2 - flYaw3) + (flYaw3 - flYaw4)) / 3) / (TICK_INTERVAL * 50.0f);

			if (fabsf(flYawRate) < 1.0f)
			{
				return;
			}

			m_flYawTurnRate = std::clamp(flYawRate, -4.3f, 4.3f);
		}
	}

	if (CFG::Aimbot_Projectile_Air_Strafe_Prediction && !(m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && F::LagRecords->HasRecords(pPlayer))
	{
		const LagRecord_t* rec0{F::LagRecords->GetRecord(pPlayer, 0)};
		const LagRecord_t* rec1{F::LagRecords->GetRecord(pPlayer, 1)};
		const LagRecord_t* rec2{F::LagRecords->GetRecord(pPlayer, 2)};
		const LagRecord_t* rec3{F::LagRecords->GetRecord(pPlayer, 3)};
		//const LagRecord_t* rec4{F::LagRecords->GetRecord(pPlayer, 4)};

		if (rec0 && rec1 && rec2 && rec3 /*&& rec4*/)
		{
			const float yaw0{Math::VelocityToAngles(rec0->Velocity).y};
			const float yaw1{Math::VelocityToAngles(rec1->Velocity).y};
			const float yaw2{Math::VelocityToAngles(rec2->Velocity).y};
			const float yaw3{Math::VelocityToAngles(rec3->Velocity).y};
			//float yaw4{Math::VelocityToAngles(rec4->m_vVelocity).y};

			const bool inc{/*yaw4 > yaw3 &&*/ yaw3 > yaw2 && yaw2 > yaw1 && yaw1 > yaw0};
			const bool dec{/*yaw4 < yaw3 &&*/ yaw3 < yaw2 && yaw2 < yaw1 && yaw1 < yaw0};

			if (!inc && !dec)
			{
				return;
			}

			const float delta{(((yaw0 - yaw1) + (yaw2 - yaw3) /*+ (yaw3 - yaw4)*/) / 2)};

			m_flYawTurnRate = delta;

			if (m_flYawTurnRate > 0.0f)
			{
				m_MoveData.m_flSideMove = -450.0f;
			}

			if (m_flYawTurnRate < 0.0f)
			{
				m_MoveData.m_flSideMove = 450.0f;
			}

			m_MoveData.m_flForwardMove = 0.0f;
		}
	}
}

bool CMovementSimulation::Initialize(C_TFPlayer* pPlayer)
{
	if (!pPlayer || pPlayer->deadflag())
		return false;

	//set player
	m_pPlayer = pPlayer;

	//set current command
	//we'll use this to set current player's command, without it CGameMovement::CheckInterval will try to access a nullptr
	static CUserCmd dummyCmd = {};

	I::MoveHelper->SetHost(m_pPlayer);
	m_pPlayer->SetCurrentCommand(&dummyCmd);

	//store player's data
	m_PlayerDataBackup.Store(m_pPlayer);

	//store vars
	m_bOldInPrediction = I::Prediction->m_bInPrediction;
	m_bOldFirstTimePredicted = I::Prediction->m_bFirstTimePredicted;
	m_flOldFrametime = I::GlobalVars->frametime;

	//the hacks that make it work
	{
		if (pPlayer->m_fFlags() & FL_DUCKING)
		{
			pPlayer->m_fFlags() &= ~FL_DUCKING; //breaks origin's z if FL_DUCKING is not removed
			pPlayer->m_bDucked() = true; //(mins/maxs will be fine when ducking as long as m_bDucked is true)
			pPlayer->m_flDucktime() = 0.0f;
			pPlayer->m_flDuckJumpTime() = 0.0f;
			pPlayer->m_bDucking() = false;
			pPlayer->m_bInDuckJump() = true;
		}

		if (pPlayer != H::Entities->GetLocal())
			pPlayer->m_hGroundEntity() = nullptr; //without this nonlocal entities get snapped to the floor

		pPlayer->m_flModelScale() -= 0.03125f; //fixes issues with corners

		if (pPlayer->m_fFlags() & FL_ONGROUND)
			pPlayer->m_vecOrigin().z += 0.03125f * 3.0f; //to prevent getting stuck in the ground

		//for some reason if xy vel is zero it doesn't predict
		if (fabsf(pPlayer->m_vecVelocity().x) < 0.01f)
			pPlayer->m_vecVelocity().x = 0.015f;

		if (fabsf(pPlayer->m_vecVelocity().y) < 0.01f)
			pPlayer->m_vecVelocity().y = 0.015f;

		if ((pPlayer->m_fFlags() & FL_ONGROUND) || pPlayer->m_hGroundEntity().Get())
		{
			pPlayer->m_vecVelocity().z = 0.0f;
		}
	}

	//setup move data
	SetupMoveData(m_pPlayer, &m_MoveData);

	return true;
}

void CMovementSimulation::Restore()
{
	if (!m_pPlayer)
		return;

	I::MoveHelper->SetHost(nullptr);
	m_pPlayer->SetCurrentCommand(nullptr);

	m_PlayerDataBackup.Restore(m_pPlayer);

	I::Prediction->m_bInPrediction = m_bOldInPrediction;
	I::Prediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	I::GlobalVars->frametime = m_flOldFrametime;

	m_pPlayer = nullptr;
	m_flYawTurnRate = 0.0f;

	std::memset(&m_MoveData, 0, sizeof(CMoveData));
	std::memset(&m_PlayerDataBackup, 0, sizeof(CPlayerDataBackup));
}

void CMovementSimulation::RunTick(float flTimeToTarget)
{
	if (!m_pPlayer)
	{
		return;
	}

	//make sure frametime and prediction vars are right
	I::Prediction->m_bInPrediction = true;
	I::Prediction->m_bFirstTimePredicted = false;
	I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	if (m_MoveData.m_vecVelocity.Length() < 15.0f && (m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		return;
	}

	if (CFG::Aimbot_Projectile_Ground_Strafe_Prediction && (m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && (m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		m_MoveData.m_vecViewAngles.y += m_flYawTurnRate * Math::RemapValClamped(flTimeToTarget, 0.0f, 1.0f, 1.0f, 0.5f);
	}

	if (CFG::Aimbot_Projectile_Air_Strafe_Prediction && !(m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && !(m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		m_MoveData.m_vecViewAngles.y += m_flYawTurnRate;
	}

	m_bRunning = true;

	I::GameMovement->ProcessMovement(m_pPlayer, &m_MoveData);

	m_bRunning = false;
}
