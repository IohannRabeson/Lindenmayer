//
// Created by Io on 17/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
#define MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
#include <vector>
#include <functional>

class QMidiMessageMatrix
{
    using Iterator = bool*;
    using iterator = Iterator;
public:
    using ConstIterator = bool const*;
    using const_iterator = ConstIterator;

    QMidiMessageMatrix();
    QMidiMessageMatrix(int const x, int const y);

    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator begin();
    Iterator end();

    int width() const { return m_width; }
    int height() const { return m_height; }

    void setWidth(int const width);
    void setHeight(int const height);
    void resize(int const width, int const height);
    void set(int const x, int const y, bool const value);
    bool get(int const x, int const y) const;
    void foreach(std::function<void(int const, int const, bool const)> const&& f) const;
    void foreach(std::function<void(int const, int const, bool&)> const&& f);
private:
    std::unique_ptr<bool[]> m_connections;
    int m_width;
    int m_height;
};

#endif //MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
