//
// Created by Io on 04/01/2018.
//

#ifndef MIDIMONITOR_SETTINGSUTILS_HPP
#define MIDIMONITOR_SETTINGSUTILS_HPP
#include <QSettings>

template <class T>
T restoreFrom(QSettings& settings, QString const& key)
{
    return settings.value(key).value<T>();
}

#endif //MIDIMONITOR_SETTINGSUTILS_HPP
