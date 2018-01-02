//
// Created by Io on 01/01/2018.
//

#include "TextDatabase.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtDebug>

bool TextDatabase::load(QString const jsonFilePath)
{
    static QString const ControlChangeNames = "control_change_names";
    static QString const ControlChangeValues = "control_change_values";
    static QRegExp const MatchCopyExpression("\\[copy (.*)\\]");
    bool isOk = false;
    QFile jsonFile(jsonFilePath);
    QList<QPair<int, int>> aliases;

    if (jsonFile.open(QFile::ReadOnly) && jsonFile.isReadable())
    {
        QJsonParseError jsonError;
        QJsonDocument json = QJsonDocument::fromJson(jsonFile.readAll(), &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            qWarning() << "[TextDatabase]: failed to read" << jsonFilePath << ":" << jsonError.errorString();
        }
        else if (json.isObject())
        {
            auto const jsonObject = json.object();

            if (jsonObject.contains(ControlChangeNames))
            {
                QMap<unsigned char, QString> translations;
                auto const jsonControlChangeNames = jsonObject[ControlChangeNames].toObject();

                isOk = true;
                for (auto it = jsonControlChangeNames.begin(); it != jsonControlChangeNames.end() && isOk; ++it)
                {
                    bool convertInteger = true;
                    unsigned char const key = it.key().toInt(&convertInteger);
                    QString const replacement = it.value().toString();

                    if (!convertInteger)
                    {
                        qWarning() << "[TextDatabase]: invalid control change identifier (should be an integer):"
                                   << it.key() << "=>" << replacement;
                        continue;
                    }
                    if (translations.contains(key))
                    {
                        qWarning() << "[TextDatabase]: key overrided" << key;
                    }
                    translations.insert(key, replacement);
                }
                m_controlChangeNumberTranslations = translations;
            }

            if (jsonObject.contains(ControlChangeValues))
            {
                QMap<unsigned char, QMap<unsigned char, QString>> translations;

                if (jsonObject[ControlChangeValues].isObject())
                {
                    // value for key "control_change_values" is an object
                    // probably a map with control value as key and text as value
                    auto const jsonControlChangeValues = jsonObject[ControlChangeValues].toObject();

                    for (auto controlIt = jsonControlChangeValues.begin(); controlIt != jsonControlChangeValues.end(); ++controlIt)
                    {
                        bool convertInteger = true;
                        unsigned char const controlChangeNumber = controlIt.key().toInt(&convertInteger);

                        if (!convertInteger)
                        {
                            qWarning() << "[TextDatabase]: invalid control change identifier (should be an integer):"
                                       << controlIt.key();
                            continue;
                        }

                        if (controlIt.value().isString())
                        {
                            // value is a special directive such as [alias foo]
                            QString const text = controlIt.value().toString();

                            if (MatchCopyExpression.indexIn(text) != -1 && !MatchCopyExpression.cap(1).trimmed().isEmpty())
                            {
                                bool convertToInt = false;
                                int aliasNumber = MatchCopyExpression.cap(1).toInt(&convertToInt);

                                if (convertToInt)
                                {
                                    aliases.append(qMakePair(controlChangeNumber, aliasNumber));
                                }
                                else
                                {
                                    qWarning() << "Error";
                                }
                            }
                        }
                        else if (controlIt.value().isObject())
                        {
                            QJsonObject const replacements = controlIt.value().toObject();
                            QMap<unsigned char, QString> valueReplacements;

                            for (auto valueIt = replacements.begin(); valueIt != replacements.end(); ++valueIt)
                            {
                                unsigned char const key = valueIt.key().toInt(&convertInteger);
                                QString const replacement = valueIt.value().toString();

                                if (valueReplacements.contains(key))
                                {
                                    qWarning() << "[TextDatabase]: key overrided" << controlIt.key() << key;
                                }
                                valueReplacements.insert(key, replacement);
                            }
                            translations.insert(controlChangeNumber, valueReplacements);
                        }
                    }
                }
                m_controlChangeValueTranslations = translations;
            }
            // Process aliases
            for (auto const aliasPair : aliases)
            {
                if (aliasPair.first == aliasPair.second)
                {
                    qDebug() << "[TextDatabase]: same alias source and target" << aliasPair.second;
                    continue;
                }
                if (!m_controlChangeValueTranslations.contains(aliasPair.second))
                {
                    qDebug() << "[TextDatabase]: invalid alias source" << aliasPair.second;
                    continue;
                }
                m_controlChangeValueTranslations.insert(aliasPair.first, m_controlChangeValueTranslations.value(aliasPair.second));
            }
        }
    }
    return isOk;
}

QString TextDatabase::translateControlChangeNumber(unsigned char const controlNumber) const
{
    auto const translationIt = m_controlChangeNumberTranslations.find(controlNumber);
    QString result;

    if (translationIt != m_controlChangeNumberTranslations.end())
    {
        auto const controlName = translationIt.value();

        result = QString("[CC%0] %1").arg(controlNumber).arg(controlName);
    }
    else
    {
        result = QString("[CC%0]").arg(controlNumber);
    }
    return result;
}

QString TextDatabase::translateControlChangeValue(unsigned char const controlNumber, unsigned char const controlValue) const
{
    auto const translationIt = m_controlChangeValueTranslations.find(controlNumber);
    QString result = QString::number(controlValue);

    if (translationIt != m_controlChangeValueTranslations.end())
    {
        auto const valueTranslations = translationIt.value();

        result = valueTranslations.value(controlValue, result) + QString(" (%0)").arg(controlValue);
    }
    return result;
}
