#include <stdio.h>
#include <libintl.h>
#include <locale.h>

#define _(string) gettext(string)

int main()
{
	bindtextdomain("myLang", "Lang/locale");
	textdomain("myLang");

	
	bindtextdomain("test", "Lang/locale");
	textdomain("test");

//	setlocale(LC_ALL, "zh_CN.UTF-8");
	setlocale(LC_ALL, "en_CA.utf8");

	char notice[200] = {0};
	sprintf(notice, "%s", _("hello world"));
	printf("%s\n", notice);

	return 0;
}
