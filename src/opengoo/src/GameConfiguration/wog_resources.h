#ifndef WOG_RESOURCES_H
#define WOG_RESOURCES_H

#include <QString>
#include <QList>
#include <QDebug>

struct WOGResource
{
    enum Type {IMAGE, SOUND, FONT};

    Type type;
    QString id;
    QString path;
};

struct WOGResourceGroup
{
    QString id;
    QList<WOGResource*> resource;

    ~WOGResourceGroup();
};

class WOGResources
{
public:
    QList<WOGResourceGroup*> group;

    QString GetResource(WOGResource::Type type, const QString & id
                        , const QString & groupid=QString()) const;

    QString GetImage(const QString & id
                     , const QString & groupid=QString()) const
    {
        return GetResource(WOGResource::IMAGE, id, groupid);
    }

    ~WOGResources();
};

#endif // WOG_RESOURCES_H
