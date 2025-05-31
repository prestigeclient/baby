#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/MiscVisuals/MiscVisuals.h"

MAKE_HOOK(ClientModeShared_OverrideView, Memory::GetVFunc(I::ClientModeShared, 16), void, __fastcall,
	CClientModeShared* ecx, CViewSetup* pSetup)
{
	if (!pSetup)
	{
		return;
	}

	//CALL_ORIGINAL(ecx, pSetup);

	F::MiscVisuals->CustomFOV(pSetup);
	F::MiscVisuals->Thirdperson(pSetup);
}
