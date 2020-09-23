/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "property_enumeration.h"
#include <QtCore/QVariant>

namespace Mayo {

Enumeration::Enumeration(std::initializer_list<Item> listItem)
    : m_vecItem(listItem)
{
}

void Enumeration::addItem(Value value, const TextId& name)
{
    const Item item = { value, name };
    m_vecItem.emplace_back(std::move(item));
}

int Enumeration::size() const
{
    return int(m_vecItem.size());
}

const Enumeration::Item& Enumeration::findItem(Enumeration::Value value) const
{
    const int index = this->findIndex(value);
    Expects(index != -1);
    return this->itemAt(index);
}

int Enumeration::findIndex(Value value) const
{
    auto it = std::find_if(
                m_vecItem.cbegin(),
                m_vecItem.cend(),
                [=](const Item& item) { return item.value == value; });
    return it != m_vecItem.cend() ? it - m_vecItem.cbegin() : -1;
}

Enumeration::Value Enumeration::findValue(const QByteArray& name) const
{
    for (const Item& item : m_vecItem) {
        if (name == static_cast<QByteArray>(item.name))
            return item.value;
    }

    Q_ASSERT(false);
    return -1;
}

QByteArray Enumeration::findName(Value value) const
{
    const int index = this->findIndex(value);
    if (index != -1)
        return static_cast<QByteArray>(this->itemAt(index).name);

    return QByteArray();
}

const Enumeration::Item& Enumeration::itemAt(int index) const
{
    return m_vecItem.at(index);
}

Span<const Enumeration::Item> Enumeration::items() const
{
    return m_vecItem;
}

PropertyEnumeration::PropertyEnumeration(
        PropertyGroup* grp, const TextId& name, const Enumeration* enumeration)
    : Property(grp, name)
{
    this->setEnumeration(enumeration);
}

const Enumeration* PropertyEnumeration::enumeration() const
{
    return m_enumeration;
}

void PropertyEnumeration::setEnumeration(const Enumeration* enumeration)
{
    m_enumeration = enumeration;
    if (m_enumeration && m_enumeration->size() > 0)
        m_value = m_enumeration->itemAt(0).value;
}

QByteArray PropertyEnumeration::name() const
{
    return m_enumeration ? m_enumeration->findName(m_value) : QByteArray();
}

Enumeration::Value PropertyEnumeration::value() const
{
    return m_value;
}

Result<void> PropertyEnumeration::setValue(Enumeration::Value v)
{
    // TODO: check v is an enumerated value of m_enumeration
    return Property::setValueHelper(this, &m_value, v);
}

QVariant PropertyEnumeration::valueAsVariant() const
{
    return QVariant::fromValue(m_value);
}

Result<void> PropertyEnumeration::setValueFromVariant(const QVariant& value)
{
    return this->setValue(value.toInt());
}

const char* PropertyEnumeration::dynTypeName() const
{
    return PropertyEnumeration::TypeName;
}

const char PropertyEnumeration::TypeName[] = "Mayo::PropertyEnumeration";

} // namespace Mayo
