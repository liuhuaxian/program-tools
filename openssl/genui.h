/****************************************************************************
** Form interface generated from reading ui file 'ui.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORM1_H
#define FORM1_H

#include <qvariant.h>
#include <qdialog.h>
#include "procini.h"
#include "prod_config.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QCheckBox;

class Form1 : public QDialog
{
    Q_OBJECT

public:
    Form1( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );
    ~Form1();

    QLabel* textLabel3;
    QLineEdit* m_pVersion;
    QLabel* textLabel4;
    QLineEdit* m_pRelease;
    QLabel* textLabel5;
    QLabel* textLabel1;
    QLabel* textLabel7;
    QLabel* textLabel6;
    QLineEdit* m_pYear;
    QComboBox* m_pProduct;
	QList<uint> m_arProduct;
    QComboBox* m_pLife;
    QComboBox* m_pType;
	QList<uint> m_arType;
    QComboBox* m_pRegion;
	QList<uint> m_arRegion;
    QComboBox* m_pPlatform;
	QList<uint> m_arPlatform;
    QLabel* textLabel9;
    QLabel* textLabel11;
    QLabel* textLabel20;
    QLabel* textLabel2;
    QLineEdit* m_pCount;
    QLineEdit* m_pMac;
    QLabel* textLabel8;
    QLineEdit* m_pStartNum;
    QPushButton* pushButton1_2;
    QPushButton* pushButton1;
	QCheckBox* m_pYearVersion;
	QCheckBox* m_pIsTesting;
private:
	CProcIni m_Proc;
	struct product_info m_Info;
	uint m_nCount;
protected:

	void LoadCombo();
	QLineEdit* CheckParams();
	void GenKey();
protected slots:
    virtual void languageChange();
	virtual void OnGenerate();
	virtual void OnCancel();

};

#endif // FORM1_H
