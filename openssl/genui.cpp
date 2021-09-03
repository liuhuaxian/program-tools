/****************************************************************************
 ** Form implementation generated from reading ui file 'ui.ui'
 **
 ** Created by User Interface Compiler
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/


#include <QtCore/qvariant.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include <qcheckbox.h>
#include <stdlib.h>
#include <unistd.h>
#include "genui.h"
#include "procini.h"
#include "macchk.h"
#include "prod_config.h"
/*
 *  Constructs a Form1 as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Form1::Form1( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
   //: QDialog( parent, name, modal, fl ),m_Proc(PRODUCT_INI)
{
    if ( !name )
	setWindowTitle("Form1");
	//setName( "Form1" );


    memset(&m_Info,0,sizeof(m_Info));
    textLabel3 = new QLabel( "textLabel3",this );
    textLabel3->setGeometry( QRect( 20, 70, 90, 20 ) );

    m_pVersion= new QLineEdit( "lineEdit1",this );
    m_pVersion->setGeometry( QRect( 120, 70, 50, 21 ) );

    textLabel4 = new QLabel( "textLabel4",this );
    textLabel4->setGeometry( QRect( 180, 70, 90, 20 ) );

    m_pRelease= new QLineEdit( "lineEdit2",this );
    m_pRelease->setGeometry( QRect( 280, 70, 50, 21 ) );

    textLabel5 = new QLabel( "textLabel5",this );
    textLabel5->setGeometry( QRect( 350, 70, 50, 20 ) );

    m_pProduct= new QComboBox();
    m_pProduct->setGeometry( QRect( 100, 24, 200, 20 ) );

    textLabel1 = new QLabel( "textLabel1",this );
    textLabel1->setGeometry( QRect( 17, 24, 75, 21 ) );

    textLabel7 = new QLabel( "textLabel7",this );
    textLabel7->setGeometry( QRect( 248, 120, 70, 20 ) );

    textLabel6 = new QLabel( "textLabel6",this );
    textLabel6->setGeometry( QRect( 20, 120, 144, 20 ) );

    m_pYear= new QLineEdit( "lineEdit3",this );
    m_pYear->setGeometry( QRect( 170, 120, 70, 21 ) );
    m_pYear->setText(QString::number(QDate::currentDate().year()));

    m_pLife= new QComboBox();
    m_pLife->setGeometry( QRect( 320, 120, 71, 21 ) );

    textLabel9 = new QLabel( "textLabel9",this );
    textLabel9->setGeometry( QRect( 399, 120, 50, 20 ) );

    textLabel11 = new QLabel( "textLabel11",this );
    textLabel11->setGeometry( QRect( 200, 190, 50, 20 ) );

    textLabel20 = new QLabel( "textLabel20",this );
    textLabel20->setGeometry( QRect( 350, 190, 50, 20 ) );

    m_pType= new QComboBox();
    m_pType->setGeometry( QRect( 400, 70, 150, 20 ) );

    m_pRegion= new QComboBox();
    m_pRegion->setGeometry( QRect( 448, 120, 103, 21 ) );

    m_pPlatform= new QComboBox();
    m_pPlatform->setGeometry( QRect( 370, 24, 180, 20 ) );

    textLabel2 = new QLabel( "textLabel2",this);
    textLabel2->setGeometry( QRect( 311, 20, 50, 30 ) );

    m_pCount= new QLineEdit("lineEdit5",this);
    m_pCount->setGeometry( QRect( 250, 190, 70, 21 ) );

    m_pMac= new QLineEdit( "lineEdit10",this);
    m_pMac->setGeometry( QRect( 400, 190, 120, 21 ) );
    m_pMac->setText("00:00:00:00:00:00");

    textLabel8 = new QLabel( "textLabel8",this );
    textLabel8->setGeometry( QRect( 23, 190, 61, 21 ) );
	
    m_pStartNum= new QLineEdit( "lineEdit4",this );
    m_pStartNum->setGeometry( QRect( 95, 190, 91, 21 ) );

    pushButton1_2 = new QPushButton( "pushButton1_2",this );
    pushButton1_2->setGeometry( QRect( 340, 240, 91, 30 ) );

    pushButton1 = new QPushButton( "pushButton1",this );
    pushButton1->setGeometry( QRect( 460, 240, 91, 30 ) );

    m_pYearVersion=new QCheckBox("year edtion",this);
    m_pYearVersion->setGeometry(QRect(35,145,190,25));

    m_pIsTesting=new QCheckBox("Generate as testing SN",this);
    m_pIsTesting->setGeometry(QRect(23,230,200,20));	

    languageChange();
    resize( QSize(569, 293).expandedTo(minimumSizeHint()) );
    connect(pushButton1_2, SIGNAL(clicked()),SLOT(OnGenerate()));
    connect(pushButton1, SIGNAL(clicked()),SLOT(OnCancel()));
    //clearWState( WState_Polished );
    LoadCombo();
}

void Form1::LoadCombo()
{
    QStringList list;
    m_Proc.GetKeyList("name",list,m_arProduct);
    m_pProduct->addItems(list);
    list.clear();
    m_Proc.GetKeyList("platform",list,m_arPlatform);
    m_pPlatform->addItems(list);
    list.clear();
    m_Proc.GetKeyList("type",list,m_arType);
    m_pType->addItems(list);
    list.clear();
    m_Proc.GetKeyList("region",list,m_arRegion);
    m_pRegion->addItems(list);
    m_pLife->insertItem(0,"1 Month");
    m_pLife->insertItem(1,"3 Months");
    m_pLife->insertItem(2,"6 Months");
    m_pLife->insertItem(3,"1 Year");
    m_pLife->insertItem(4,"2 Years");
    m_pLife->insertItem(5,"3 Years");
    m_pLife->insertItem(6,"4 Years");
    m_pLife->insertItem(7,"5 Years");
    m_pLife->insertItem(8,"6 Years");
    m_pLife->insertItem(9,"7 Years");
    m_pLife->setCurrentIndex(3);
}

void Form1::GenKey()
{
    char sn[100];
    struct product_info info;
    memcpy(&info,&m_Info,sizeof(info));
    for(int i=0;i<m_nCount;++i) {
        char testinfo[1024]="";
        generate(&info,sn);
        info.number++;
        if(info.istest)
            sprintf(testinfo,"(It's a testing SN, generate date: %d-%02d-%02d)",m_Info.testyear+2009,m_Info.testmonth,m_Info.testday);
        if(!info.yearversion)
            printf("%s \t%s-%s.%s %s on %s\t%d %s\n",sn,m_pProduct->currentText().toLatin1(),m_pVersion->text().toLatin1(),m_pRelease->text().toLatin1(),
                   m_pType->currentText().toLatin1(),m_pPlatform->currentText().toLatin1(),info.number, testinfo);
        else
            printf("%s \t%s-%d-%s.%s %s on %s\t%d %s\n",sn,m_pProduct->currentText().toLatin1(),info.relyear+2009,m_pVersion->text().toLatin1(),m_pRelease->text().toLatin1(),
                   m_pType->currentText().toLatin1(),m_pPlatform->currentText().toLatin1(),info.number,testinfo);
			
    }
}

void Form1::OnGenerate()
{
    QLineEdit* pFocus=CheckParams();
    if(!pFocus) {
        m_Info.name=m_arProduct[m_pProduct->currentIndex()];
        m_Info.platform=m_arPlatform[m_pPlatform->currentIndex()];
        m_Info.type=m_arType[m_pType->currentIndex()];
        m_Info.region=m_arRegion[m_pRegion->currentIndex()];
        m_Info.life=m_pLife->currentIndex();
        m_Info.yearversion=m_pYearVersion->isChecked();
        if(m_pIsTesting->isChecked()) {
            m_Info.istest=1;
            QDate dt=QDate::currentDate();
            m_Info.testyear=dt.year()-2009;
            m_Info.testmonth=dt.month();
            m_Info.testday=dt.day();

        }
        /*
          m_Info.mac[0]=0x00;
          m_Info.mac[1]=0x02;
          m_Info.mac[2]=0xA5;
          m_Info.mac[3]=0x5E;
          m_Info.mac[4]=0x75;
          m_Info.mac[5]=0x05;
        */
        GenKey();
    }
    else {
        pFocus->setFocus();
    }
	
}

QLineEdit* Form1::CheckParams()
{
    QLineEdit* pRet=NULL;
    bool bRet;
    QString tmp;
    tmp=m_pVersion->text();
    uint num=tmp.toUInt(&bRet);
    if(bRet&& num>=0 && num <64) {
        m_Info.version=num;
        tmp=m_pRelease->text();
        num=tmp.toUInt(&bRet);
        if(bRet&& num>=0 && num<32) {
            m_Info.release=num;
            tmp=m_pYear->text();
            num=tmp.toUInt(&bRet);
            if( bRet && num>=2009 && num<=2041) {
                m_Info.relyear=num-2009;
                tmp=m_pStartNum->text();
                num=tmp.toUInt(&bRet);
                if(bRet && num>0 && num<=(1<<24)) {
                    m_Info.number=num-1;
                    tmp=m_pCount->text();
                    num=tmp.toUInt(&bRet);
                    if(bRet && num>0 && (m_Info.number+num)<(1<<24)) {
                        m_nCount=num;
						
                        tmp=m_pMac->text();
			char* ch;
			QByteArray ba = tmp.toLatin1();
			ch = ba.data();
                        uchar mac[6];
                        int i;
                        int ret = mac_un2char(ch,mac);
                        if(bRet && ret == 0 ) {
                            for (i=0;i<6;i++)
                                m_Info.mac[i]=mac[i];	
                        }
                        else {
                            QMessageBox::warning(this,"error","Invalid Mac Add");
                            pRet=m_pMac;

                        }
                    }
                    else {
                        QMessageBox::warning(this,"error","Invalid count number");
                        pRet=m_pCount;
                    }
                }
				
                else {
                    QMessageBox::warning(this,"error","Invalid SN error");
                    pRet=m_pStartNum;
                }
            }
            else {
                QMessageBox::warning(this,"error","Invalid release year");
                pRet=m_pYear;
            }
        }
        else {
            QMessageBox::warning(this,"error","Invalid realse number");
            pRet=m_pRelease;
        }
    }
    else {
        QMessageBox::warning(this,"error","Invalid version number");
        pRet=m_pVersion;
    }
    return pRet;
}

void Form1::OnCancel()
{
    close();
}

/*
 *  Destroys the object and frees any allocated resources
 */
Form1::~Form1()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Form1::languageChange()
{
    //setCaption( tr( "Sn Generater" ) );
    setWindowTitle("Sn Generater");
    textLabel3->setText( tr( "version(0-63)" ) );
    textLabel4->setText( tr( "release(0-31)" ) );
    textLabel5->setText( tr( "type" ) );
    textLabel1->setText( tr( "product name" ) );
    textLabel7->setText( tr( "life cycle" ) );
    textLabel6->setText( tr( "release year(2009-2041):" ) );
    textLabel9->setText( tr( "region:" ) );
    textLabel11->setText( tr( "count:" ) );
    textLabel2->setText( tr( "platform" ) );
    textLabel8->setText( tr( "start sn" ) );
    textLabel20->setText( tr( "Mac:" ) );
    m_pIsTesting->setText( tr("Generate as test SN"));
    pushButton1_2->setText( tr( "Generate" ) );
    pushButton1->setText( tr( "Quit" ) );
    m_pYearVersion->setText(tr("Use year as main version"));
}

int main(int c, char** v)
{
    if(c > 1) {      /* cmd mode */
        int opt;
        int cnt=0;
        int count;
        struct product_info info;
        memset(&info,0,sizeof(info));
        while((opt=getopt(c,v,"n:p:v:r:t:y:l:g:m:s:c:T:M:h"))!=-1) {
            int verify;
            switch(opt) {
            case 'n':
                verify=atoi(optarg);
                if(verify<0 || verify>63) {
                    fprintf(stderr,"Invalid name number.\n");
                    return 1;
                }
                info.name=verify;
                cnt++;
                break;
            case 'p':
                verify=atoi(optarg);
                if(verify<0 || verify>31) {
                    fprintf(stderr,"Invalid platform number.\n");
                    return 1;
                }
                info.platform=verify;
                cnt++;
                break;
            case 'v':
                verify=atoi(optarg);
                if(verify<0 || verify >63) {
                    fprintf(stderr,"version shoud >=0 && <=63.\n");
                    return 1;
                }
                info.version=verify;
                cnt++;
                break;
            case 'r':
                verify=atoi(optarg);
                if(verify<0 || verify > 31) {
                    fprintf(stderr,"release number should >=0 && <=31\n");
                    return 1;
                }
                info.release=verify;
                cnt++;
                break;
            case 't':
                verify=atoi(optarg);
                if(verify<0 || verify>63) {
                    fprintf(stderr,"release type should >=0 && <=63.\n");
                    return 1;
                }
                info.type=verify;
                cnt++;
                break;
            case 'y':
                verify=atoi(optarg);
                if(verify<2009 || verify > 2040) {
                    fprintf(stderr,"release year should >=2009 && <=2040.\n");
                    return 1;
                }
                info.relyear=verify-2009;
                cnt++;
                break;
            case 'l':
                verify=atoi(optarg);
                if(verify<0 || verify>15) {
                    fprintf(stderr,"lifecycle number should >=0 && <=15.\n");
                    return 1;
                }
                info.life=verify;
                cnt++;
                break;
            case 'g':
                verify=atoi(optarg);
                if(verify<0 || verify>63) {
                    fprintf(stderr,"region should >=0 && <=63 \n");
                    return 1;
                }
                info.region=verify;
                cnt++;
                break;
            case 'm':
                verify=atoi(optarg);
                if(verify!=0 && verify!=1) {
                    fprintf(stderr,"Year as main version shoud be 0 or 1.\n");
                    return 1;
                }
                info.yearversion=verify;
                cnt++;
                break;
            case 's':
                verify=atoi(optarg);
                if(verify<1  || verify>= (1<<24)) {
                    fprintf(stderr,"Start number should >=1 && <= %u\n",1<<24);
                    return 1;
                }
                info.number=verify-1;
                cnt++;
                break;
            case 'c':
                verify=atoi(optarg);
                if(verify<1 || verify>=(1<<24)) {
                    fprintf(stderr,"Count should >=1 && <=%u\n",1<<24);
                    return 1;
                }
                count=verify;
                cnt++;
                break;
            case 'T':
                verify=atoi(optarg);
                if(verify !=1 && verify!=0) {
                    fprintf(stderr,"IsTestSN should be 0 or 1.\n");
                    return 1;
                }
                if(verify) {
                    info.istest=1;
                    time_t t;
                    time(&t);
                    struct tm  tm;
                    localtime_r(&t,&tm);
                    info.testyear=tm.tm_year+1900-2009;
                    info.testmonth=tm.tm_mon+1;
                    info.testday=tm.tm_mday;
                }
                cnt++;
                break;
            case 'M':
                unsigned char dsig[6];
                int i;

                if( mac_un2char(optarg, dsig) == -1) {
                    fprintf(stderr,"Invalid Mac Address,should be 00:00:00:00:00:00\n");
                    return 1;
                }
                for(i=0; i < 6; i++)	
                    info.mac[i] = dsig[i];
                cnt++;
                break;
            case 'h':
            default:
                fprintf(stderr,"Usage:\ngensn -h: see this infomation.\ngensn -n NumberOfName -p NumberOfPlatform -v Version -r Release -t NumberOfReleaseType -y ReleaseYear -l LifeCircle -g NumberOfRegion -m IfUseYearMainVersion -s StartSN -c Count -T IsTestSN -M Mac\nSuch as: gensn -n 0 -p 4 -v 6 -r 3 -t 0 -y 2010 -l 3 -g 1 -m 1 -s 1 -c 10 -T 0 -M 00:22:68:12:c3:b4\nMeans:\nproduct name: Red Flag Linux Desktop\nplatform: longsoon\nversion: 6.3\ntype: Standard release\nrelease year: 2010\nlife cycle: 1 Year\nregion: Beijing\nuse year as main version: yes\nstartsn: 0\ncount: 10\nIs test SN: no.\nMac: 00:22:68:12:c3:b4\n(See also /etc/product.ini)\n");
                return 1;
            }
        }
        if(cnt==13) {
            char ret[100];
            const unsigned int max=1<<24;
            for(int i=0;i<count && info.number<max;++i) {
                generate(&info,ret);
                printf("%u:\t%s\n",info.number+1,ret);
                info.number++;
            }
            return 0;
        }
        fprintf(stderr,"Usage:\ngensn -h: see this infomation.\ngensn -n NumberOfName -p NumberOfPlatform -v Version -r Release -t NumberOfReleaseType -y ReleaseYear -l LifeCircle -g NumberOfRegion -m IfUseYearMainVersion -s StartSN -c Count -T IsTestSN -M Mac\nSuch as: gensn -n 0 -p 4 -v 6 -r 3 -t 0 -y 2010 -l 3 -g 1 -m 1 -s 1 -c 10 -T 0 -M 00:22:68:12:c3:b4\nMeans:\nproduct name: Red Flag Linux Desktop\nplatform: longsoon\nversion: 6.3\ntype: Standard release\nrelease year: 2010\nlife cycle: 1 Year\nregion: Beijing\nuse year as main version: yes\nstartsn: 0\ncount: 10\nIs test SN: no.\nMac: 00:22:68:12:c3:b4\n(See also /etc/product.ini)\n");
        return 1;
    }
    QApplication app(c,v);
    Form1 form(NULL);
    //app.setMainWidget(&form);
    form.show();
    return app.exec();
}


#include "genui.moc"
