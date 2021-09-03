#ifndef __PROCINI_H__
#define __PROCINI_H__
#include <qhash.h>
#include <qvector.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qlist.h>
#include "prod_config.h"

struct KeyPair
{
    QString key;
    unsigned int value;
};

class CProcIni
{
 public:
    CProcIni(const char* filename=PRODUCT_INI);
    virtual ~CProcIni();
    void EnumSections(bool (*RecvSecName) (QString , void*), void* p);
    void EnumKeys(const QString& strSecName, bool (*RecvKeyName)(const KeyPair*, void *),void *p);
    void GetKeyList(const QString& strSecName, QList<QString> &listRet,QList<unsigned int> &numList);
    QString GetKeyName(const QString& sec, int value);
	
 private:
    QHash<QString , QVector<KeyPair*>* > m_Dict;
};

#endif
