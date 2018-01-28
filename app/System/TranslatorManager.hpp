//
// Created by Io on 28/01/2018.
//

#ifndef MIDIMONITOR_TRANSLATORMANAGER_HPP
#define MIDIMONITOR_TRANSLATORMANAGER_HPP
#include <QDir>
#include <QTranslator>

class TranslatorManager : public QObject
{
    static QDir makeTranslationDirectory();
public:
    TranslatorManager();

    /*!
     * \brief Load translation file using the system locale.
     *
     * The application name is used as base for the filename.
     */
    bool loadSystemTranslator();

    /*!
     * \brief Try to load a translation file based on a specific locale.
     * \param translator
     * \param locale
     * \param directory
     * \return
     *
     * If this function fail to load the locale specified then it fallback by trying
     * to load the system locale and finally the default locale (en).
     * \warning I've spent the day to try to understand why the translator failed to load my qm files.
     * Finally, I figured out translator does'nt load empty qm files. Theses files contains only 23 octets and
     * don't have any translation.
     */
    bool loadTranslator(QLocale const& locale);

    bool loadTranslatorImp(QLocale const& locale);
    void loadDefaultTranslator();
private:
    QDir const m_translationDirectory;
    QTranslator m_translator;
};

#endif //MIDIMONITOR_TRANSLATORMANAGER_HPP
