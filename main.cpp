#include "xmlelement.hpp"
#include "abstractxmlelement.hpp"
#include "folder.hpp"
#include "project.hpp"

#include <QCoreApplication>
#include <QFile>

#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("../QtXmlParser/example.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Unable to open file" << std::endl;
        return 0;
    }

    QXmlStreamReader reader(&file);

    try {
        const auto project = LoadProject(reader);
        ShowProject(*project);
    }  catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return a.exec();
}
