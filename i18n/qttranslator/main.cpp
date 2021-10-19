#include "widget.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (QLocale::system().uiLanguages().first() == QString("zh-CN"))
    {
        QTranslator *m_translator = new QTranslator();
        qDebug() << m_translator->load(":/Translate_CN.qm");
        a.installTranslator(m_translator);
    }

    a.setWindowIcon(QIcon("icon.png"));

    Widget w;
    w.show();
    
    return a.exec();
}
