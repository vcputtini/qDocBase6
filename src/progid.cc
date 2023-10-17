#include "progid.h"

/*!
 * \brief ProgId::ProgId
 */
ProgId::ProgId() {}

ProgId::~ProgId() {}

/*!
 * \brief ProgId::strInternalName
 * \return
 */
const QString
ProgId::strInternalName()
{
  return QString(Name);
}

/*!
 * \brief ProgId::strName
 * \return QString() name program
 */
const QString
ProgId::strName()
{
  return QString(Name);
}

/*!
 * \brief ProgId::strVersion
 * \return QString version program
 */
const QString
ProgId::strVersion()
{
  return QString(Version);
}

/*!
 * \attention The UUID below should not be changed without due care.
 * \brief ProgId::strOrganization
 *
 * \return UUID of organization.
 */
const QString
ProgId::strOrganization()
{
  return QString("015261e5-bded-4c6b-8142-a07be301ba20");
}
