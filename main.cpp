#include "quickcontact.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationDisplayName("Contacts");
    a.setApplicationVersion( "1.0");
    a.setOrganizationName("sdutz");
    a.setApplicationName("Contacts");

    QuickContact w;
    w.show();

    return a.exec();
}
