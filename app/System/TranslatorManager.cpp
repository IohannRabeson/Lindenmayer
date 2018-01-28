//
// Created by Io on 28/01/2018.
//

#include "TranslatorManager.hpp"
#include <QCoreApplication>
#include <QtDebug>

QDir TranslatorManager::makeTranslationDirectory()
{
    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("Resources/translations");
    return dir;
}

TranslatorManager::TranslatorManager()
: m_translationDirectory(makeTranslationDirectory())
{
}

bool TranslatorManager::loadSystemTranslator()
{
    qDebug() << "Using system local:" << QLocale::system().language() << QLocale::system().uiLanguages();

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
        loaded = QCoreApplication::installTranslator(&m_translator);
    }
    return loaded;
}

void TranslatorManager::loadDefaultTranslator()
{
    QLocale const defaultLocale(QLocale::Language::English, QLocale::Country::AnyCountry);

    if (!loadTranslatorImp(defaultLocale))
    {
        qFatal("Unable to load default translation file.");
    }
}