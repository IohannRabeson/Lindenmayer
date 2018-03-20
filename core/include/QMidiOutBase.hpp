#ifndef QMIDIOUTBASE_HPP
#define QMIDIOUTBASE_HPP
#include <QAbstractMidiOut.hpp>

class QMidiOutBase : public QAbstractMidiOut
{
public:
    explicit QMidiOutBase(QString const& name);

    bool openPort(int const portIndex) override;
    void closePort() override;
    int portOpened() const override;
    QString portName() const override;
    void setPortEnabled(bool const enabled) override;
    bool isPortEnabled() const override;
private:
    QString m_portName;
    int m_portIndex = -1;
    bool m_portEnabled = true;
};

#endif // QMIDIOUTBASE_HPP
