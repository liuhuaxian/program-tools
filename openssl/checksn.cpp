#include <QtCore/qstring.h>
#include "procini.h"
#include "prod_config.h"

QString g_strProduct, g_strPlatform, g_strType,g_strRegion;

static const  char *lives[]={"1 Month", "3 Months","6 Months", "1 Year", "2 Years","3 Years","4 Years","5 Years","6 Years","7 Years"};

static bool FindName(const KeyPair* pair,void* p)
{
    struct product_info* pInfo=(struct product_info*)p;
    if(pair->value==pInfo->name) {
        g_strProduct=pair->key;
        return false;
    }
    return true;
}

static bool FindPT(const KeyPair* pair,void* p)
{
    struct product_info* pInfo=(struct product_info*)p;
    if(pair->value==pInfo->platform) {
        g_strPlatform=pair->key;
        return false;
    }
    return true;
}


static bool FindType(const KeyPair* pair,void* p)
{
    struct product_info* pInfo=(struct product_info*)p;
    if(pair->value==pInfo->type) {
        g_strType=pair->key;
        return false;
    }
    return true;
}

static bool FindRegion(const KeyPair* pair,void* p)
{
    struct product_info* pInfo=(struct product_info*)p;
    if(pair->value==pInfo->region) {
        g_strRegion=pair->key;
        return false;
    }
    return true;
}

int main(int c, char** v)
{
    int i;
    struct product_info info;

    if(c!=2) {
        printf("Usage: ./checksn SN\n");
        return 1;
    }

    if(get_product_info(v[1],&info)) {
        return 1;
    }
    CProcIni proc;
    proc.EnumKeys("name",FindName,&info);
    proc.EnumKeys("platform",FindPT,&info);
    proc.EnumKeys("type",FindType,&info);
    proc.EnumKeys("region",FindRegion,&info);
    QString strYearVersion="";
    QString strTestInfo;
    if(info.yearversion)
        strYearVersion=QString("%1-").arg(info.relyear+2009);
    if(info.istest)
        strTestInfo=QString("Testing SN: yes \nGenerate date: %1-%2-%3").arg(info.testyear+2009).arg(info.testmonth).arg(info.testday);
    else 
        strTestInfo=QString("Testing SN: no");
    printf("name: %s\nversion: %s%s.%s\nplatform: %s\ntype: %s\nrelease year: %s\nlife: %s\nregion: %s\nserial: %s\n%s\n",
           g_strProduct.toLatin1(),strYearVersion.toLatin1(),QString::number(info.version).toLatin1(),QString::number(info.release).toLatin1(),g_strPlatform.toLatin1(),
           g_strType.toLatin1(),QString::number(info.relyear+2009).toLatin1(),lives[info.life],g_strRegion.toLatin1(),QString::number(info.number+1).toLatin1(),strTestInfo.toLatin1());
    printf("Product Mac");
    for (i = 0; i < 6; i++)
        printf(": %02x",info.mac[i]);
    printf("\n");
    return 0;
}
