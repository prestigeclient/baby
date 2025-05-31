#include "../../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

#include "../Features/CFG.h"
#include "../Features/SpyCamera/SpyCamera.h"

MAKE_SIGNATURE(CBaseAnimating_DrawModel, "client.dll", "4C 8B DC 49 89 5B ? 89 54 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 8D 3D", 0x0);
MAKE_SIGNATURE(ViewmodelAttachment_DrawModel, "client.dll", "41 8B D5 FF 50 ? 8B 97", 0x6);

MAKE_HOOK(IVModelRender_DrawModelExecute, Memory::GetVFunc(I::ModelRender, 19), void, __fastcall,
	IVModelRender* ecx, const DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!F::SpyCamera->IsRendering())
	{
		const auto pClientEntity = I::ClientEntityList->GetClientEntity(pInfo.entity_index);

		if (pClientEntity)
		{
			if (CFG::Visuals_Disable_Dropped_Weapons && pClientEntity->GetClassId() == ETFClassIds::CTFDroppedWeapon)
				return;

			const bool clean_ss = CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot();

			if (CFG::Materials_ViewModel_Active && !clean_ss && pClientEntity->GetClassId() == ETFClassIds::CTFViewModel)
			{
				if (const auto pLocal = H::Entities->GetLocal())
				{
					if (!pLocal->IsUbered() && !pLocal->deadflag())
					{
						auto getMaterial = [&](int nIndex) -> IMaterial* {
							switch (nIndex)
							{
								case 0: return nullptr;
								case 1: return F::Materials->m_pFlat;
								case 2: return F::Materials->m_pShaded;
								case 3: return F::Materials->m_pGlossy;
								case 4: return F::Materials->m_pGlow;
								case 5: return F::Materials->m_pPlastic;
								default: return nullptr;
							}
						};

						const auto mat = getMaterial(CFG::Materials_ViewModel_Hands_Material);

						if (mat)
						{
							I::ModelRender->ForcedMaterialOverride(mat);
						}

						if (CFG::Materials_ViewModel_Hands_Alpha < 1.0f)
						{
							I::RenderView->SetBlend(CFG::Materials_ViewModel_Hands_Alpha);
						}

						if (mat)
						{
							const auto& base = CFG::Color_Hands;

							if (mat != F::Materials->m_pGlow)
							{
								I::RenderView->SetColorModulation(base);
							}

							else
							{
								if (F::Materials->m_pGlowEnvmapTint && F::Materials->m_pGlowSelfillumTint)
								{
									const auto& sheen = CFG::Color_Hands_Sheen;

									I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

									F::Materials->m_pGlowSelfillumTint
									            ->SetVecValue(ColorUtils::ToFloat(base.r), ColorUtils::ToFloat(base.g), ColorUtils::ToFloat(base.b));

									F::Materials->m_pGlowEnvmapTint
									            ->SetVecValue(ColorUtils::ToFloat(sheen.r), ColorUtils::ToFloat(sheen.g), ColorUtils::ToFloat(sheen.b));
								}
							}
						}

						CALL_ORIGINAL(ecx, state, pInfo, pCustomBoneToWorld);

						I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

						if (CFG::Materials_ViewModel_Hands_Alpha < 1.0f)
						{
							I::RenderView->SetBlend(1.0f);
						}

						if (mat)
						{
							I::ModelRender->ForcedMaterialOverride(nullptr);
						}

						return;
					}
				}
			}

			if (CFG::Visuals_Disable_Wearables && pClientEntity->GetClassId() == ETFClassIds::CTFWearable)
				return;

			if (pClientEntity->GetClassId() == ETFClassIds::CDynamicProp)
			{
				if (CFG::Visuals_World_Modulation_Mode == 0)
				{
					if (const auto flNightMode = CFG::Visuals_Night_Mode)
					{
						const auto col = static_cast<byte>(Math::RemapValClamped(flNightMode, 0.0f, 100.0f, 255.0f, 50.0f));

						I::RenderView->SetColorModulation({ col, col, col, static_cast<byte>(255) });
					}
				}

				else
				{
					I::RenderView->SetColorModulation(CFG::Color_Props);
				}

				CALL_ORIGINAL(ecx, state, pInfo, pCustomBoneToWorld);

				I::RenderView->SetColorModulation({ 255, 255, 255, 255 });

				return;
			}

			if (!I::EngineClient->IsTakingScreenshot())
			{
				const auto pEntity = pClientEntity->As<C_BaseEntity>();

				if (!F::Materials->IsRendering() && !F::Outlines->IsRendering() && (F::Outlines->HasDrawn(pEntity) || F::Materials->HasDrawn(pEntity)))
					return;
			}
		}
	}

	if (CFG::Visuals_Simple_Models)
		*const_cast<int*>(&state.m_lod) = 5;

	CALL_ORIGINAL(ecx, state, pInfo, pCustomBoneToWorld);
}

MAKE_HOOK(CBaseAnimating_DrawModel, Signatures::CBaseAnimating_DrawModel.Get(), int, __fastcall,
	void *ecx, int flags)
{
	const bool clean_ss = CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot();

	if (CFG::Materials_ViewModel_Active
		&& !clean_ss
		&& (flags & STUDIO_RENDER)
		&& reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == Signatures::ViewmodelAttachment_DrawModel.Get())
	{
		if (const auto pLocal = H::Entities->GetLocal())
		{
			if (!pLocal->IsUbered() && !pLocal->deadflag())
			{
				auto getMaterial = [&](int nIndex) -> IMaterial* {
					switch (nIndex)
					{
						case 0: return nullptr;
						case 1: return F::Materials->m_pFlat;
						case 2: return F::Materials->m_pShaded;
						case 3: return F::Materials->m_pGlossy;
						case 4: return F::Materials->m_pGlow;
						case 5: return F::Materials->m_pPlastic;
						default: return nullptr;
					}
				};

				const auto mat = getMaterial(CFG::Materials_ViewModel_Weapon_Material);

				if (mat)
				{
					I::ModelRender->ForcedMaterialOverride(mat);
				}

				if (CFG::Materials_ViewModel_Weapon_Alpha < 1.0f)
				{
					I::RenderView->SetBlend(CFG::Materials_ViewModel_Weapon_Alpha);
				}

				if (mat)
				{
					const auto& base = CFG::Color_Weapon;

					if (mat != F::Materials->m_pGlow)
					{
						I::RenderView->SetColorModulation(base);
					}

					else
					{
						if (F::Materials->m_pGlowEnvmapTint && F::Materials->m_pGlowSelfillumTint)
						{
							const auto& sheen = CFG::Color_Weapon_Sheen;

							I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

							F::Materials->m_pGlowSelfillumTint
							            ->SetVecValue(ColorUtils::ToFloat(base.r), ColorUtils::ToFloat(base.g), ColorUtils::ToFloat(base.b));

							F::Materials->m_pGlowEnvmapTint
							            ->SetVecValue(ColorUtils::ToFloat(sheen.r), ColorUtils::ToFloat(sheen.g), ColorUtils::ToFloat(sheen.b));
						}
					}
				}

				const auto result = CALL_ORIGINAL(ecx, flags);

				I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

				if (CFG::Materials_ViewModel_Weapon_Alpha < 1.0f)
				{
					I::RenderView->SetBlend(1.0f);
				}

				if (mat)
				{
					I::ModelRender->ForcedMaterialOverride(nullptr);
				}

				return result;
			}
		}
	}

	return CALL_ORIGINAL(ecx, flags);
}
