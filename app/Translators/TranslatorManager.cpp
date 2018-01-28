//
// Created by Io on 28/01/2018.
//

#include "TranslatorManager.hpp"
#include <QCoreApplication>
#include <QtDebug>

QDir TranslatorManager::getTranslationDirectory()
{
    // TODO: OS depedent!
    // This is for OSX bundle
    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("Resources/translations");
    return dir;
}

TranslatorManager::TranslatorManager()
: m_translationDirectory(getTranslationDirectory())
{
}

bool TranslatorManager::loadSystemTranslator()
{
    qDebug() << "[TranslatorManager] Loading system language" << QLocale::system().nativeLanguageName();
    return loadTranslator(QLocale::system());
}

/*!
 * \brief
 * \param translator
 * \param locale
 * \param directory
 * \return
 *
 * \warning I've spent the day to try to understand why the translator failed to load my qm files.
 * Finally, I figured out translator does'nt load empty qm files. Theses files contains only 23 octets and
 * don't have any translation.
 */
bool TranslatorManager::loadTranslator(QLocale const& locale)
{
    bool loaded = false;

    if (loadTranslatorImp(locale))
    {
        loaded = QCoreApplication::installTranslator(&m_translator);
    }
    else
    {
        loadDefaultTranslator();
        qWarning() << "[TranslatorManager]: Unable to load translator for" << locale.uiLanguages();
    }
    return loaded;
}

bool TranslatorManager::loadTranslatorImp(QLocale const& locale)
{
    bool loaded = false;

    if (m_translator.load(locale, QCoreApplication::applicationName(), "_", m_translationDirectory.absolutePath(), ".qm"))
    {
        qInfo() << "[TranslatorManager]: Using " << QLocale::system().language() << QLocale::system().uiLanguages();
        loaded = QCoreApplication::installTranslator(&m_translator);
    }
    return loaded;
}

void TranslatorManager::loadDefaultTranslator()
{
    QLocale const defaultLocale(QLocale::Language::English, QLocale::Country::AnyCountry);

    qDebug() << "[TranslatorManager]: Loading default language" << defaultLocale.nativeLanguageName();
    if (!loadTranslatorImp(defaultLocale))
    {
        qWarning("[TranslatorManager]: Unable to load default translation file.");
    }
}