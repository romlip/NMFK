#include "pch.h"
#include "KObliczenia.h"

KObliczenia::KObliczenia()
{
	urMES = nullptr;
}

KObliczenia::~KObliczenia()
{
	if (urMES) delete urMES;
}

KWektor* KObliczenia::Licz(KDane* dane)
{
	return nullptr;
}
