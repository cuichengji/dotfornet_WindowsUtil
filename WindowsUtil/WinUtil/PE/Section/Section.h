#pragma once
#include "../Common/Common.h"
#include "../NtHeader/NtHeader.h"
#include "SectionIterator.h"
namespace PE
{
	
	template<typename _PeDecoder>
	class SectionHeaders :
		public PeStruct<_PeDecoder>,
		public InitStruct,
		public GetIterator<SectionIterator<SectionHeaders<_PeDecoder>>, SectionHeaders<_PeDecoder>>
	{
		
		PIMAGE_SECTION_HEADER firstSectionHeader;
		PIMAGE_SECTION_HEADER lastSectionHeader;
		PDWORD sectionAlignment;
		PWORD sectionCount;
	public:
		friend _PeDecoder;
		friend SectionIterator<SectionHeaders<_PeDecoder>>;
		SectionHeaders(_PeDecoder& pe) :PeStruct<_PeDecoder>(pe)
		{
		}

		~SectionHeaders()
		{
		}
		
		
		// ͨ�� InitStruct �̳�
		virtual bool InitFunc() override
		{
			if (!peDecoder.GetNtHeader()->Init())
			{
				return false;
			}
			if (peDecoder.GetNtHeader()->HasNtHeader32())
			{
				auto header32 = peDecoder.GetNtHeader()->GetNtHeader32();
				sectionAlignment = &header32->OptionalHeader.SectionAlignment;
				firstSectionHeader = IMAGE_FIRST_SECTION(header32);
				sectionCount = &header32->FileHeader.NumberOfSections;
			}
			else
			{
				auto header64 = peDecoder.GetNtHeader()->GetNtHeader64();
				sectionAlignment = &header64->OptionalHeader.SectionAlignment;
				firstSectionHeader = IMAGE_FIRST_SECTION(header64);
				sectionCount = &header64->FileHeader.NumberOfSections;
			}
			lastSectionHeader = firstSectionHeader + *sectionCount;
			return true;
		}

		

	};

}