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

    /*!
     * \brief Outputs count or columns count
     */
    int outputCount() const { return m_outputCount; }

    /*!
     * \brief Inputs count or rows count
     */
    int inputCount() const { return m_inputCount; }

    void setOutputCount(int const output);
    void setInputCount(int const input);
    void resize(int const output, int const input);
    void set(int const x, int const y, bool const value);
    bool get(int const x, int const y) const;
    void forachInput(int const inputPortIndex, std::function<void(int const, int const)> const&& f) const;
    void foreach(std::function<void(int const, int const, bool const)> const&& f) const;
    void foreach(std::function<void(int const, int const, bool&)> const&& f);
private:
    std::unique_ptr<bool[]> m_connections;
    int m_outputCount;
    int m_inputCount;
};

#endif //MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
