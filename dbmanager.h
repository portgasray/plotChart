#ifndef DBMANAGER_H
#define DBMANAGER_H
#include<QtSql>
#include<QtDebug>
#include<QFileInfo>

class DbManager
{
public:
    //DbManager(const QString& path);
    bool connectDB(const QString& dbname);
    bool addPerson(const QString& name);

private:
    QSqlDatabase m_db;
};
#endif // DBMANAGER_H
