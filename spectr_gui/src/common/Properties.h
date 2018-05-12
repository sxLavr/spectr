#ifndef sxPROPERTIES_H
#define sxPROPERTIES_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSettings>
#include <QJsonObject>


#define SXPROPERTIES_BASE_TYPES QString, QVariant
#define SXPROPERTIES_BASE_CLASS QHash <SXPROPERTIES_BASE_TYPES>

typedef QHashIterator<SXPROPERTIES_BASE_TYPES> sxPropertiesIterator;


class Properties: public SXPROPERTIES_BASE_CLASS
{
//--------------------------------------------------------------------------------------------------
public:     Properties() { }
//--------------------------------------------------------------------------------------------------
public:     Properties(const Properties & props): SXPROPERTIES_BASE_CLASS(props) {
                SXPROPERTIES_BASE_CLASS::operator = (props);
            }
//--------------------------------------------------------------------------------------------------
public:     Properties(const QVariantHash & hash) {
                *this += hash;
            }
//--------------------------------------------------------------------------------------------------
public:     Properties(const QVariant & val) {
                if (QVariant::Hash == val.type()) {
                    *this += val.toHash();
                }
            }
//--------------------------------------------------------------------------------------------------
public:     virtual ~Properties() { }
//--------------------------------------------------------------------------------------------------
public:     void operator += (const Properties & props) {
                    QHashIterator <QString, QVariant> iterator(props);
                    while (iterator.hasNext()) {
                        if (iterator.next().value().isNull() == false) {
                            insert(iterator.key(), iterator.value());
                        }
                    }
                }
//--------------------------------------------------------------------------------------------------
public:     void operator += (const QVariantHash & props) {
                    QHashIterator <QString, QVariant> iterator(props);
                    while (iterator.hasNext()) {
                        if (iterator.next().value().isNull() == false) {
                            insert(iterator.key(), iterator.value());
                        }
                    }
                }
//--------------------------------------------------------------------------------------------------
public:     QString valueS(const QString & key, const QString  & defValue = QString()) const {
                    return contains(key) ? value(key).toString() : defValue;
                }
//--------------------------------------------------------------------------------------------------
public:     int valueI(const QString & key, int defValue = 0) const {
                    return contains(key) ? value(key).toInt() : defValue;
                }
//--------------------------------------------------------------------------------------------------
public:     quint32 valueUi(const QString & key, quint32 defValue = 0) const {
                    return contains(key) ? value(key).value<quint32>() : defValue;
                }
//--------------------------------------------------------------------------------------------------
public:     quint64 valueUi64(const QString & key, quint64 defValue = 0) const {
                return contains(key) ? value(key).value<quint64>() : defValue;
            }
//--------------------------------------------------------------------------------------------------
public:     bool valueB(const QString & key, bool defValue = false) const {
                    return contains(key) ? value(key).toBool() : defValue;
                }
//--------------------------------------------------------------------------------------------------
public:     int removeItem(const QString & key) {
                    return remove(key);
                }
//--------------------------------------------------------------------------------------------------
public:     QString toString() const {
                    QString result;
                    sxPropertiesIterator iterator(*this);
                    while (iterator.hasNext()) {
                        iterator.next();
                        result.append(QString("%1 = %2, ").arg(iterator.key()).arg(iterator.value().toString()));
                    }

                    return result;
                }
//--------------------------------------------------------------------------------------------------
public:     void load( QJsonObject const& json ) {
                    for (QJsonObject::const_iterator it = json.begin(); it != json.end(); it++)
                    {
                        insert( it.key(), it.value() );
                    }
                }
//--------------------------------------------------------------------------------------------------
public:     void save( QJsonObject& json ) {
                    foreach (QString const& key, keys())
                    {
                        json.insert( key, value( key ).toString() );
                    }
                }
//--------------------------------------------------------------------------------------------------
};

#endif//sxPROPERTIES_H
