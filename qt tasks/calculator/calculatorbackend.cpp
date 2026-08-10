#include "calculatorbackend.h"
#include <cmath>

CalculatorBackend::CalculatorBackend(QObject *parent)
    : QObject(parent),
    m_display("0"),
    m_expression(""),
    m_firstOperand(0.0),
    m_pendingOperator(""),
    m_waitingForOperand(true) {}

QString CalculatorBackend::getDisplay() const { return m_display; }
QString CalculatorBackend::getExpression() const { return m_expression; }
QStringList CalculatorBackend::getHistory() const { return m_history; }

void CalculatorBackend::setDisplay(const QString &text) {
    if (m_display != text) {
        m_display = text;
        emit displayChanged();
    }
}

void CalculatorBackend::setExpression(const QString &text) {
    if (m_expression != text) {
        m_expression = text;
        emit expressionChanged();
    }
}

void CalculatorBackend::digitPressed(const QString &digit) {
    if (m_waitingForOperand) {
        if (digit == ".") {
            setDisplay("0.");
        } else {
            setDisplay(digit);
        }
        m_waitingForOperand = false;
    } else {
        if (digit == "." && m_display.contains(".")) return;
        if (m_display == "0" && digit != ".") {
            setDisplay(digit);
        } else {
            setDisplay(m_display + digit);
        }
    }
}

void CalculatorBackend::operatorPressed(const QString &op) {
    double currentVal = m_display.toDouble();

    if (!m_pendingOperator.isEmpty() && !m_waitingForOperand) {
        m_firstOperand = calculate(m_firstOperand, currentVal, m_pendingOperator);
        setDisplay(QString::number(m_firstOperand));
    } else {
        m_firstOperand = currentVal;
    }

    m_pendingOperator = op;
    m_waitingForOperand = true;
    setExpression(QString::number(m_firstOperand) + " " + op);
}

void CalculatorBackend::percentPressed() {
    double currentVal = m_display.toDouble();

    if (m_pendingOperator.isEmpty()) {

        currentVal = currentVal / 100.0;
    } else if (m_pendingOperator == "+" || m_pendingOperator == "-" || m_pendingOperator == "−") {

        currentVal = m_firstOperand * (currentVal / 100.0);
    } else {

        currentVal = currentVal / 100.0;
    }

    setDisplay(QString::number(currentVal));
}

void CalculatorBackend::equalsPressed() {
    if (m_pendingOperator.isEmpty()) return;

    double currentVal = m_display.toDouble();
    QString fullExpr = QString::number(m_firstOperand) + " " + m_pendingOperator + " " + QString::number(currentVal);
    setExpression(fullExpr);

    double result = calculate(m_firstOperand, currentVal, m_pendingOperator);
    setDisplay(QString::number(result));

    m_history.prepend(fullExpr + " = " + QString::number(result));
    emit historyChanged();

    m_firstOperand = result;
    m_pendingOperator.clear();
    m_waitingForOperand = true;
}

void CalculatorBackend::clearPressed() {
    m_display = "0";
    m_expression = "";
    m_firstOperand = 0.0;
    m_pendingOperator.clear();
    m_waitingForOperand = true;
    emit displayChanged();
    emit expressionChanged();
}

void CalculatorBackend::clearHistory() {
    if (!m_history.isEmpty()) {
        m_history.clear();
        emit historyChanged();
    }
}

void CalculatorBackend::backspacePressed() {
    if (m_waitingForOperand || m_display == "0") return;

    m_display.chop(1);
    if (m_display.isEmpty() || m_display == "-") {
        m_display = "0";
        m_waitingForOperand = true;
    }
    emit displayChanged();
}

void CalculatorBackend::toggleSignPressed() {
    if (m_display == "0") return;

    if (m_display.startsWith("-")) {
        m_display.remove(0, 1);
    } else {
        m_display.prepend("-");
    }
    emit displayChanged();
}

double CalculatorBackend::calculate(double left, double right, const QString &op) {
    if (op == "+") return left + right;
    if (op == "-" || op == "−") return left - right;
    if (op == "*" || op == "×") return left * right;
    if (op == "/" || op == "÷") {
        if (right == 0.0) return 0.0;
        return left / right;
    }
    return right;
}