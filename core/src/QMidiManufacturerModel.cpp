//
// Created by Io on 02/01/2018.
//

#include "QMidiManufacturerModel.hpp"
#include "../../app/Ui/Format.hpp"

#include <QFile>
#include <QMidiMessage.hpp>
#include <QtDebug>

int QMidiManufacturerModel::rowCount(QModelIndex const& parent) const
{
    return parent.isValid() ? 0 : m_elements.size();
}

QVariant QMidiManufacturerModel::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        auto const& element = m_elements[index.row()];

        switch (role)
        {
            case Roles::Name:
                result = element.name;
                break;
            case Roles::Code:
                result = Format::formatBytes(element.code);
                break;
            default:
                break;
        }
    }
    return result;
}

void QMidiManufacturerModel::load(std::function<QVector<QMidiManufacturerModel::Element>()>&& loader)
{
    beginResetModel();
    m_elements = loader();
    endResetModel();
    qDebug() << "[QMidiManufacturerModel]" << m_elements.size() << "manufacturers loaded";
}

int QMidiManufacturerModel::findCode(QMidiMessage const& message) const
{
    auto const& bytes = message.bytes();

    for (auto i = 0; i < m_elements.size(); ++i)
    {
        auto const& element = m_elements[i];
        auto const& code = element.code;
        int j = 0;
        int bytePosition = 1;
        bool found = false;

        while (j < code.size() && bytePosition < bytes.size())
        {
            if (code[j] != bytes[bytePosition])
            {
                break;
            }
            else
            {
                ++j;
                ++bytePosition;
            }
        }
        if (j == code.size())
        {
            return i;
        }
    }
    return -1;
}

QString QMidiManufacturerModel::getName(int const row) const
{
    return row == -1 ? QString() : m_elements[row].name;
}

static QVector<unsigned char> parseCode(QString const& line, int& pos)
{
    QVector<unsigned char> results;

    auto const commaPosition = line.indexOf(',');
    auto const utilPart = line.mid(0, commaPosition);
    auto parts = utilPart.split(' ');

    pos = commaPosition + 1;
    for (auto& part : parts)
    {
        if (!part.endsWith("H", Qt::CaseSensitive))
        {
            qWarning() << "[QMidiManufacturerModel]: Invalid manufacturer code:" << part;
            break;
        }
        part.remove(part.size() - 1, 1);
        results.append(part.toInt(nullptr, 16));
    }
    return results;
}

static QString parseName(QString const& line, int& pos)
{
    auto temp = line.mid(pos);

    if (temp.startsWith('\"') && temp.endsWith('\"'))
    {
        temp = temp.mid(1, temp.size() - 2);
    }
    return temp;
}
QMidiManufacturerModel::LoadFromCSV::LoadFromCSV(QString const& csvFilePath)
: m_csvFilePath(csvFilePath)
{
}

QVector<QMidiManufacturerModel::Element> QMidiManufacturerModel::LoadFromCSV::operator()() const
{
    QVector<QMidiManufacturerModel::Element> results;
    QFile file(m_csvFilePath);

    if (file.open(QFile::ReadOnly))
    {
        Element element;
        QVector<unsigned char> code;
        QTextStream stream(&file);
        QString line;

        while (stream.readLineInto(&line))
        {
            int pos = 0;
            auto const code = parseCode(line, pos);
            auto const name = parseName(line, pos);

            results.append(Element{name, code});
        }
    }
    return results;
}
