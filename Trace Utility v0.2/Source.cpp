#include "Trace.h"

int main()
{
	tlf << "Text1" << AT;
	tlf_e << "Text2" << AT;
	tlf_w << "Text3" << AT;
	tlf_f << "Text4" << AT;
	tlf_d << "Text5" << AT;
	tlf_d << "Text5.2" << AT;
	tlf_i << "Text6" << AT;
	tlf_t << "Text7" << AT;

	system("pause");
	return 0;
}