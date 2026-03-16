#include <Arkitekt/Shared.hpp>
#include <Arkitekt/SigScan.h>
#pragma warning(disable: 4244)
#pragma warning(disable: 4189)
#pragma warning(disable: 4456)


unsigned char *SigScan::s_pBase = 0;
unsigned char *SigScan::s_pEnd = 0;
size_t SigScan::s_iBaseLen = 0;
unsigned char *SigScan::s_pLastStartAddress = 0;
unsigned char *SigScan::s_pLastAddress = 0;
std::unordered_map<void*, bool> SigScan::s_functions;
std::list<SigScan::Match> SigScan::s_lastMatches;


//=====================================================================

SigScan::SigScan(const char *sig) : m_pAddress(0)
{
	m_bStartFromLastAddress = false;
	m_bNoReturnSeek = false;
	m_fnEntryAlignment = 0x10;
	m_dist = 0;

	// Default signature if nothing was specified
	if(!sig || !sig[0])
	{
		if(s_lastMatches.empty())
		{
			// Look for the first function after the last scanned function
			sig = ".558bec";
		}
		else
		{
			// Get remaining matches from the last scan
			s_lastMatches.pop_front();
			m_matches = s_lastMatches;
			m_iLength = 0;
			m_sig = NULL;
			m_mask = NULL;
			return;
		}
	}

	int len = 0;
	for(const char *p = sig ; *p ; ++p)
	{
		char c = *p;
		len += ((c>='a' && c<='f') || (c>='A' && c<='F') || (c>='0' && c<='9') || c=='?');
	}

	m_iLength = len / 2;
	m_sig = new unsigned char[m_iLength];
	m_mask = new unsigned char[m_iLength];

	unsigned char *ps = m_sig;
	unsigned char *pm = m_mask;

	bool b = false;

	short matchStart = -1;
	int i = 0;

	for(const char *p = sig ; *p ; ++p)
	{
		int c = *p;
		if(c == '.')
		{
			m_bStartFromLastAddress = true;
			continue;
		}

		if(c == '!')
		{
			m_bNoReturnSeek = true;
			continue;
		}

		if (c == '#')
		{
			m_fnEntryAlignment = 0;
			continue;
		}
		if(c == '(')
		{
			matchStart = i;
			continue;
		}
		else if(c == ')')
		{
			m_matches.push_back(Match(matchStart, i - matchStart));

			matchStart = -1;
			continue;
		}

		if(c >= '0' && c <= '9') c -= '0';
		else if(c >= 'a' && c <= 'f') c -= ('a' - 10);
		else if(c >= 'A' && c <= 'F') c -= ('A' - 10);
		else c = -1;
		
		++p;

		int d = *p;
		if(!d) break;
		else if(d >= '0' && d <= '9') d -= '0';
		else if(d >= 'a' && d <= 'f') d -= ('a' - 10);
		else if(d >= 'A' && d <= 'F') d -= ('A' - 10);
		else d = -1;

		if(c >= 0 && d >= 0)
		{
			*(ps++) = (c << 4) | d;
			*(pm++) = 1;
		}
		else
		{
			*(ps++) = 0;
			*(pm++) = 0;
		}
		++i;
	}
}

//=====================================================================

SigScan::~SigScan()
{
	if(m_sig)
		delete[] m_sig;
	if(m_mask)
		delete[] m_mask;
}

bool SigScan::Scan(Callback callback)
{
	constexpr unsigned char c = (unsigned char) '\xc3';
	const unsigned char *usig = m_sig;
	if(!m_iLength)
	{
		m_pAddress = s_pLastStartAddress;
		return true;
	}

	short itn = 0;

	unsigned char *pStart = s_pBase;
	unsigned char *pEnd = s_pBase + s_iBaseLen - m_iLength;

/*
 	if(m_bStartFromLastAddress)
		pStart = (unsigned char*)s_pLastAddress;
*/
	m_matches.clear();
	for(pStart = s_pBase; pStart <= s_pEnd; ++pStart)
	{
		if (s_functions.contains((void*) pStart))
			continue;
		if (m_fnEntryAlignment && m_matches.size() < 1) // unless we are matching variables...
			if ((uintptr_t) pStart % m_fnEntryAlignment != 0)
				continue; // align to m_fnEntryAlignment (default 16 bytes)
		
		const unsigned char *p = pStart;
		const unsigned char *s = usig;
		const unsigned char *m = m_mask;
		size_t i;

		for(i=0 ; i<m_iLength ; ++i, ++p, ++s, ++m)
			if(*m && *s != *p) break;

		if(i == m_iLength)
		{
			
			m_dist = pStart-s_pBase;

			m_pAddress = pStart;
			m_pAddressEnd = reinterpret_cast<unsigned char*>((uint32_t)pStart + m_iLength);
			pStart = reinterpret_cast<unsigned char*>((uint32_t)pStart + m_iLength);
			m_matches.emplace_back(Match(m_pAddress, static_cast<short>( m_iLength & INT16_MAX)));
		}
	}

	if (m_matches.size() && callback) {
		callback(this);
		return true;
	}
	return false;
}


void SigScan::AddFunction(void* start, short length)
{
	for (short idx = 0; idx < length; idx++)
	{
		s_functions[(void*)((int32_t)start + idx)] = true;
	}
}

//=====================================================================

void SigScan::Init()
{
	HMODULE hModule = GetModuleHandle(nullptr);
	if(!hModule)
	{
		s_pBase = 0;
		return;
	}
	s_pBase = (unsigned char*)hModule;
	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER*)s_pBase;
	IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS*)(s_pBase + dos->e_lfanew);
	const auto codeSize = pe->OptionalHeader.SizeOfCode;
	s_pEnd = s_pBase + codeSize;

	if(pe->Signature != IMAGE_NT_SIGNATURE)
	{
		s_pBase = 0;
		return;
	}
	
	s_iBaseLen = pe->OptionalHeader.SizeOfImage;
	s_pLastAddress = s_pBase;
}
