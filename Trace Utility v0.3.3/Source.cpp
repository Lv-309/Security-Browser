#include "Log.h"
#include "LogDebug.h"
#include "LogError.h"
#include "LogWarning.h"
#include "LogFatal.h"
#include "LogInfo.h"
#include "LogTrace.h"

int main()
{
	tlf << AT << "Text8";
	tlf_t << AT << "Text9";
	tlf_d << AT << "Text10";
	tlf_w << AT << "Text11";
	tlf_f << AT << "Text12";
	tlf_i << AT << "Text13";
	tlf_e << AT << "Text14";

	system("pause");
	return 0;
}