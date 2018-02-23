//
// Created by Io on 30/12/2017.
//

#ifndef SYSEXVIEWER_QMIDIIN_HPP
#define SYSEXVIEWER_QMIDIIN_HPP
#include "QAbstractMidiIn.hpp"

#include <QtGlobal>
#include <QScopedPointer>

class QMidiMessage;
class QMidiInPrivate;
class QMidiMessageFilterModel;
class QItemSelectionModel;

class QMidiIn : public QAbstractMidiIn
{
    Q_DECLARE_PRIVATE(QMidiIn)
public:
    explicit QMidiIn();
    ~QMidiIn();

    bool openPort(int const portIndex) noexcept override;
    void closePort() noexcept override;
    int portOpened() const noexcept override;
    QString portName() const noexcept override;
    void setPortEnabled(bool const enabled) noexcept override;

    int portCount() const noexcept;

    QMidiMessageFilterModel* filterModel() const;
    QItemSelectionModel* filterSelectionModel() const;
private:
    QScopedPointer<QMidiInPrivate> d_ptr;
};

#endif //SYSEXVIEWER_QMIDIIN_HPP
