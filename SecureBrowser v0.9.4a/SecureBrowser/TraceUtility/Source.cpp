#include "Log.h"
#include "LogDebug.h"
#include "LogError.h"
#include "LogWarning.h"
#include "LogFatal.h"
#include "LogInfo.h"
#include "LogTrace.h"

int main()
{
	tlf << AT << "Text1";
	tlf_e << AT << "Text2";
	tlf_w << AT << "Text3";
	tlf_f << AT << "Text4";
	tlf_d << AT << "Text5";
	tlf_i << AT << "Text6";
	tlf_t << AT << "Text7";

	system("pause");
	return 0;
}