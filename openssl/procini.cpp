#include <QtCore/qregexp.h>

#include "procini.h"


CProcIni::CProcIni(const char* filename)
{
	char szBuf[1024];
	QRegExp commend("^\\s*#.+"), sec("^\\s*\\[(.+)\\]\\s*"), keys("^\\s*([^=]+)=(\\d+)\\s*");
	QVector<KeyPair*> *pCur=NULL;
	FILE *fp=fopen(filename,"r");
	if(fp)
	{
		while(fgets(szBuf,1023,fp))
		{
			if(commend.exactMatch(szBuf)) continue;
			else if(sec.exactMatch(szBuf))
			{
				// Create new array and add to dict
				pCur=new QVector<KeyPair*>;
				m_Dict.insert(sec.cap(1),pCur);
			}
			else if(keys.exactMatch(szBuf))
			{
				if(pCur)
				{
					KeyPair* pair=new KeyPair;
					pair->key=keys.cap(1);
					pair->value=keys.cap(2).simplified().toInt();
					pCur->push_back(pair);
				}
			}
		}
		fclose(fp);
	}

}

CProcIni::~CProcIni()
{
	QHash<QString, QVector<KeyPair*>*>::const_iterator it;
	for(it = m_Dict.constBegin(); it != m_Dict.constEnd(); ++it)
	{
		int cnt=it.value()->count();
		for(int i=0;i<cnt;++i)
			delete it.value()->at(i);
	}
}

void CProcIni::EnumSections(bool (*RecvSecName)(QString name, void *param), void *p)
{
    QHash<QString, QVector<KeyPair*>*>::const_iterator it;	
    for(it = m_Dict.constBegin(); it != m_Dict.constEnd(); ++it)
    {
	    if(!RecvSecName(it.key(),p))
		    return ;
    }
}

void CProcIni::EnumKeys(const QString& strSecName, bool (*RecvKeyName)(const KeyPair*key, void *param), void*p)
{
	QHash<QString, QVector<KeyPair*>*>::const_iterator it;
	it=m_Dict.find(strSecName);
	if(it != m_Dict.end() && it.key() == strSecName)
	{
		int cnt=it.value()->count();
		for(int i=0;i<cnt;++i)
			if(!RecvKeyName(it.value()->at(i),p)) return ;
	}
}

void CProcIni::GetKeyList(const QString& strSecName, QList<QString> &listRet,QList<unsigned int> &numList)
{
	QHash<QString, QVector<KeyPair*>*>::const_iterator it;
	it=m_Dict.find(strSecName);
	if(it != m_Dict.end() && it.key() == strSecName)
	{
		int cnt=it.value()->count();
		for(int i=0;i<cnt;++i)
		{
			listRet.append(it.value()->at(i)->key);
			numList.append(it.value()->at(i)->value);
		}
	}
}

QString CProcIni::GetKeyName(const QString& sec, int value)
{
	QHash<QString, QVector<KeyPair*>*>::const_iterator it;
	it=m_Dict.find(sec);
	if(it != m_Dict.end() && it.key() == sec)
	{
		int cnt=it.value()->count();
		for(int i=0;i<cnt;++i)
			if(it.value()->at(i)->value==value)
				return it.value()->at(i)->key;
	}
	return "";
}

