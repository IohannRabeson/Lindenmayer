//
// Created by Io on 02/01/2018.
//

#include "QMidiManufacturerModel.hpp"
#include "Format.hpp"

#include <QFile>
#include <QMidiMessage.hpp>
#include <QtDebug>

#include <set>

namespace
{
    class RadixTree
    {
        struct Node
        {
            Node* getOrCreate(unsigned char const key, bool& created)
            {
                for (auto* next : nexts)
                {
                    if (next->key == key)
                    {
                        created = false;
                        return next;
                    }
                }

                auto* newNode = new Node;

                newNode->key = key;
                nexts.push_back(newNode);
                created = true;
                return newNode;
            }

            Node* get(unsigned char const key)
            {
                for (auto* next : nexts)
                {
                    if (next->key == key)
                    {
                        return next;
                    }
                }
                return nullptr;
            }

            std::vector<Node*> nexts;
            int index = -1;
            unsigned char key = 0u;
        };

    public:
        RadixTree()
        {
            m_nodes.insert(m_root);
        }

        ~RadixTree()
        {
            auto it = m_nodes.begin();

            while (it != m_nodes.end())
            {
                delete *it;
                it = m_nodes.erase(it);
            }
        }

        bool add(int const elementIndex, QVector<unsigned char> const& code)
        {
            Node* current = m_root;
            int codePosition = 0;
            bool created = false;

            while (current && codePosition < code.size())
            {
                bool createdTemp = false;

                Node* nextNode = current->getOrCreate(code[codePosition], createdTemp);

                if (createdTemp)
                {
                    m_nodes.insert(nextNode);
                    created = true;
                }
                current = nextNode;
                ++codePosition;
            }

            // duplicate!
            Q_ASSERT (current->index == -1);

            current->index = elementIndex;
            return created;
        }

        int getElementIndex(QVector<unsigned char> const& code) const
        {
            Node* current = m_root;
            Node* next = nullptr;
            int codePosition = 0;
            // MIDI standard:
            // If the first byte == 0x0 then the code size should be 3 bytes.
            // Otherwise if the first byte is lesser than 0x7D (125) the manufacturer code have only 1 byte.
            auto const expectedSize = code.front() == 0 ? 3 : 1;

            while (current && codePosition < code.size())
            {
                next = current->get(code[codePosition]);

                if (next == nullptr)
                {
                    break;
                }
                current = next;
                ++codePosition;
            }
            return current && (codePosition == expectedSize) ? current->index : -1;
        }

    private:
        std::set<Node*> m_nodes;
        Node* m_root = new Node;
    };
}

class QMidiManufacturerModelPrivate
{
public:
    QVector<QMidiManufacturerModel::Element> m_elements;
    RadixTree m_radixTree;
};

QMidiManufacturerModel::QMidiManufacturerModel(QObject* parent)
: QAbstractListModel(parent)
, d_ptr(new QMidiManufacturerModelPrivate)
{
}

QMidiManufacturerModel::~QMidiManufacturerModel() = default;

int QMidiManufacturerModel::rowCount(QModelIndex const& parent) const
{
    Q_D(const QMidiManufacturerModel);

    return parent.isValid() ? 0 : d->m_elements.size();
}

QVariant QMidiManufacturerModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QMidiManufacturerModel);

    QVariant result;

    if (index.isValid())
    {
        auto const& element = d->m_elements[index.row()];

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

void QMidiManufacturerModel::load(Loader&& loader)
{
    Q_D(QMidiManufacturerModel);

    beginResetModel();

    auto const elements = loader();

    for (auto const& element : elements)
    {
        if (std::find_if(d->m_elements.begin(), d->m_elements.end(), [element](Element const& current)
                         {
                             // Notice it's not a && but a ||.
                             // Equality is computed here using names or codes.
                             return current.name == element.name || current.code == element.code;
                         }) == d->m_elements.end())
        {
            auto const index = d->m_elements.size();

            d->m_elements.push_back(element);
            d->m_radixTree.add(index, element.code);
        }
    }
    endResetModel();
    qDebug() << "[QMidiManufacturerModel]" << d->m_elements.size() << "manufacturers loaded";
}

int QMidiManufacturerModel::findCode(QMidiMessage const& message) const
{
    Q_D(const QMidiManufacturerModel);

    auto const& bytes = message.bytes();

    QVector<unsigned char> code;

    code.reserve(bytes.size() - 1);

    for (auto i = 1; i < bytes.size(); ++i)
    {
        code.append(bytes[i]);
    }

    return d->m_radixTree.getElementIndex(code);
}

QString QMidiManufacturerModel::getName(int const row) const
{
    Q_D(const QMidiManufacturerModel);

    return row == -1 ? QString() : d->m_elements[row].name;
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

namespace
{
    bool checkManufacturerCode(QString const& name, QVector<unsigned char> const& code, unsigned int const lineIndex)
    {
        if (code.isEmpty())
        {
            qWarning() << "[QMidiManufacturerModel]: Null manufacturer code (" << name << ")" << QString("line %0").arg(lineIndex);
            return false;
        }
        if (code.size() == 1 && !(code.front() > 0u && code.front() < 125u))
        {
            qWarning() << "[QMidiManufacturerModel]: Invalid manufacturer code (" << name << ")" << QString("line %0").arg(lineIndex)
                       << Format::formatBytes(code);
            return false;
        }
        if (code.size() == 3 && !(code.front() == 0u))
        {
            qWarning() << "[QMidiManufacturerModel]: Invalid manufacturer code (" << name << ")" << QString("line %0").arg(lineIndex)
                       << Format::formatBytes(code);
            return false;
        }
        return true;
    }
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
        auto lineIndex = 1u;

        while (stream.readLineInto(&line))
        {
            int pos = 0;
            auto const code = parseCode(line, pos);
            auto const name = parseName(line, pos);

            Q_ASSERT( checkManufacturerCode(name, code, lineIndex) );

            results.append(Element{name, code});
        }
    }
    return results;
}
