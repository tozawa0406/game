#include "DontDestroyOnLoad.h"

DontDestroyOnLoad::DontDestroyOnLoad(void)
{
}

DontDestroyOnLoad::~DontDestroyOnLoad(void)
{
}

void DontDestroyOnLoad::Init(void)
{
}

void DontDestroyOnLoad::Uninit(void)
{
	intList_.clear();
	floatList_.clear();
}

void DontDestroyOnLoad::Reset(void)
{
	intList_.clear();
	floatList_.clear();
}
