//
// Created by Io on 17/01/2018.
//

#ifndef MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
#define MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
#include <vector>
#include <functional>
#include <memory>
#include <set>
#include <utility>

class QMidiMessageMatrix;

class QMidiMessageMatrix
{
    using Iterator = bool*;
    using iterator = Iterator;
    using Values = std::vector<std::pair<int, int>>;
public:
    using ConstIterator = bool const*;
    using const_iterator = ConstIterator;

    QMidiMessageMatrix();
    QMidiMessageMatrix(int const outputCount, int const inputCount);

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
    void removeInput(int const input);
    void removeOutput(int const output);
    void forachInput(int const inputPortIndex, std::function<void(int const, int const)> const&& f) const;
    void forachOutput(int const outputPortIndex, std::function<void(int const, int const)> const&& f) const;
    void foreach(std::function<void(int const, int const, bool const)> const&& f) const;
    void foreach(std::function<void(int const, int const, bool&)> const&& f);
    void connectInputToOutputs(int const inputPortIndex, bool const value);
    void connectOutputToInputs(int const outputPortIndex, bool const value);
private:
    Values collectValues(bool const valueToCollect) const;
    void restoreValues(Values const& values);
private:
    std::unique_ptr<bool[]> m_connections;
    int m_outputCount;
    int m_inputCount;
};

#endif //MIDIMONITOR_QMIDIMESSAGEMATRIX_HPP
