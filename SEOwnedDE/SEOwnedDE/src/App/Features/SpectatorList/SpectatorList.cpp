#include "SpectatorList.h"

#include "../CFG.h"
#include "../Menu/Menu.h"
#include "../VisualUtils/VisualUtils.h"

#define LIST_WIDTH CFG::Visuals_SpectatorList_Width

bool CSpectatorList::GetSpectators()
{
	m_vecSpectators.clear();

	const auto pLocal = H::Entities->GetLocal();
	if (!pLocal || pLocal->deadflag())
		return false;

	for (const auto pEntity : H::Entities->GetGroup(EEntGroup::PLAYERS_OBSERVER))
	{
		// Is the local player valid?
		const auto pPlayer = pEntity->As<C_TFPlayer>();
		if (pPlayer->m_hObserverTarget().Get() != pLocal)
			continue;

		// Are they spectating?
		const int nMode = pPlayer->m_iObserverMode();
		if (nMode != OBS_MODE_IN_EYE && nMode != OBS_MODE_CHASE)
			continue;

		player_info_t playerInfo = {};
		if (!I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &playerInfo))
			continue;

		m_vecSpectators.emplace_back(Utils::ConvertUtf8ToWide(playerInfo.name), nMode);
	}

	return !m_vecSpectators.empty();
}

void CSpectatorList::Drag()
{
	const int nMouseX = H::Input->GetMouseX();
	const int nMouseY = H::Input->GetMouseY();

	static bool bDragging = false;

	if (!bDragging && F::Menu->IsMenuWindowHovered())
		return;

	static int nDeltaX = 0;
	static int nDeltaY = 0;

	const int nListX = CFG::Visuals_SpectatorList_Pos_X;
	const int nListY = CFG::Visuals_SpectatorList_Pos_Y;

	const bool bHovered = nMouseX > nListX && nMouseX < nListX + LIST_WIDTH && nMouseY > nListY && nMouseY < nListY + CFG::Menu_Drag_Bar_Height;

	// Start dragging
	if (bHovered && H::Input->IsPressed(VK_LBUTTON))
	{
		nDeltaX = nMouseX - nListX;
		nDeltaY = nMouseY - nListY;
		bDragging = true;
	}

	// Stop dragging
	if (!H::Input->IsPressed(VK_LBUTTON) && !H::Input->IsHeld(VK_LBUTTON))
		bDragging = false;

	// Update the location
	if (bDragging)
	{
		CFG::Visuals_SpectatorList_Pos_X = nMouseX - nDeltaX;
		CFG::Visuals_SpectatorList_Pos_Y = nMouseY - nDeltaY;
	}
}

void CSpectatorList::Run()
{
	if (!CFG::Visuals_SpectatorList_Active)
		return;

	// Anti screenshot?
	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
	{
		return;
	}

	// In menu?
	if (!F::Menu->IsOpen() && (I::EngineVGui->IsGameUIVisible() || SDKUtils::BInEndOfMatch()))
		return;

	if (F::Menu->IsOpen())
		Drag();

	const auto outlineColor = F::VisualUtils->GetAlphaColor(CFG::Menu_Accent_Secondary, CFG::Visuals_SpectatorList_Outline_Alpha);
	const auto bgColor = F::VisualUtils->GetAlphaColor(CFG::Menu_Background, CFG::Visuals_SpectatorList_Background_Alpha);

	// Background
	H::Draw->Rect(
		CFG::Visuals_SpectatorList_Pos_X,
		CFG::Visuals_SpectatorList_Pos_Y,
		LIST_WIDTH,
		CFG::Menu_Drag_Bar_Height,
		bgColor
	);

	// Title
	H::Draw->String(
		H::Fonts->Get(EFonts::Menu),
		CFG::Visuals_SpectatorList_Pos_X + (LIST_WIDTH / 2),
		CFG::Visuals_SpectatorList_Pos_Y + (CFG::Menu_Drag_Bar_Height / 2),
		CFG::Menu_Text,
		POS_CENTERXY,
		"Spectators"
	);

	// Outline
	H::Draw->OutlinedRect(
		CFG::Visuals_SpectatorList_Pos_X,
		CFG::Visuals_SpectatorList_Pos_Y,
		LIST_WIDTH,
		CFG::Menu_Drag_Bar_Height,
		outlineColor
	);

	// Are there any spectators?
	if (!GetSpectators())
		return;

	for (size_t n = 0; n < m_vecSpectators.size(); n++)
	{
		const auto& spectator = m_vecSpectators[n];

		const int iPos = int(n) + 1;

		// Background
		H::Draw->Rect(
			CFG::Visuals_SpectatorList_Pos_X,
			CFG::Visuals_SpectatorList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) - 1,
			LIST_WIDTH,
			CFG::Menu_Drag_Bar_Height + 1,
			bgColor
		);

		const int nModeX = CFG::Visuals_SpectatorList_Pos_X;
		const int nModeOffsetX = LIST_WIDTH / 8;
		const int nTextY = CFG::Visuals_SpectatorList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) - 1;
		const int nTextX = nModeX + nModeOffsetX + CFG::Menu_Spacing_X;
		const int nY = CFG::Visuals_SpectatorList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) - 1;

		// Divider
		H::Draw->Line(nModeX + nModeOffsetX, nY, nModeX + nModeOffsetX, nY + CFG::Menu_Drag_Bar_Height, outlineColor);

		// Spectator mode
		H::Draw->String(
			H::Fonts->Get(EFonts::Menu),
			nModeX + (nModeOffsetX / 2),
			nTextY + (CFG::Menu_Drag_Bar_Height / 2) + 1,
			CFG::Menu_Text_Inactive,
			POS_CENTERXY,
			spectator.m_nMode == OBS_MODE_IN_EYE ? "1st" : "3rd"
		);

		I::MatSystemSurface->DisableClipping(false);
		I::MatSystemSurface->SetClippingRect(nModeX, nTextY, (nModeX + LIST_WIDTH) - (CFG::Menu_Spacing_X + 1), nTextY + CFG::Menu_Drag_Bar_Height);

		// Player name
		H::Draw->String(
			H::Fonts->Get(EFonts::Menu),
			nTextX,
			nTextY + (CFG::Menu_Drag_Bar_Height / 2) + 1,
			CFG::Menu_Text_Inactive,
			POS_CENTERY,
			spectator.Name.c_str()
		);

		I::MatSystemSurface->DisableClipping(true);

		// Outline
		H::Draw->OutlinedRect(
			CFG::Visuals_SpectatorList_Pos_X,
			CFG::Visuals_SpectatorList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) - 1,
			LIST_WIDTH,
			CFG::Menu_Drag_Bar_Height + 1,
			outlineColor
		);
	}
}
