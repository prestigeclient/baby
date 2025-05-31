#include "InterfaceManager.h"
#include "../Memory/Memory.h"
#include "../Assert/Assert.h"
#include <string>
#include <format>

#pragma warning (disable: 26451)

InterfaceInit_t::InterfaceInit_t(void **pPtr, const char *sDLLName, const char *sVersion, int nOffset, int nDereferenceCount)
{
	m_pPtr = pPtr;
	m_pszDLLName = sDLLName;
	m_pszVersion = sVersion;
	m_nOffset = nOffset;
	m_nDereferenceCount = nDereferenceCount;

	U::InterfaceManager->AddInterface(this);
}

void CInterfaceManager::InitializeAllInterfaces()
{
	for (auto &Interface : m_vecInterfaces)
	{
		if (!Interface->m_pPtr || !Interface->m_pszDLLName || !Interface->m_pszVersion)
			continue;

		if (Interface->m_nOffset == -1)
			*Interface->m_pPtr = Memory::FindInterface(Interface->m_pszDLLName, Interface->m_pszVersion);
		else
		{
			auto dwDest = Memory::FindSignature(Interface->m_pszDLLName, Interface->m_pszVersion);
			if (!dwDest)
			{
				AssertCustom(dwDest, std::format("CInterfaces::Initialize() failed to find signature:\n  {}\n  {}", Interface->m_pszDLLName, Interface->m_pszVersion).c_str());
				continue;
			}

			auto dwAddress = Memory::RelToAbs(dwDest);
			*Interface->m_pPtr = reinterpret_cast<void *>(dwAddress + Interface->m_nOffset);

			for (int n = 0; n < Interface->m_nDereferenceCount; n++)
			{
				if (Interface->m_pPtr)
					*Interface->m_pPtr = *reinterpret_cast<void **>(*Interface->m_pPtr);
			}
		}

		AssertCustom(*Interface->m_pPtr, std::format("CInterfaces::Initialize() failed to initialize:\n  {}\n  {}", Interface->m_pszDLLName, Interface->m_pszVersion).c_str());
	}
}