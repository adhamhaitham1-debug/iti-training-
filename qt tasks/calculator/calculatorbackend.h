#ifndef CALCULATORBACKEND_H
#define CALCULATORBACKEND_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <qqml.h>

class CalculatorBackend : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString display READ getDisplay NOTIFY displayChanged)
    Q_PROPERTY(QString expression READ getExpression NOTIFY expressionChanged)
    Q_PROPERTY(QStringList history READ getHistory NOTIFY historyChanged)

public:
    explicit CalculatorBackend(QObject *parent = nullptr);

    QString getDisplay() const;
    QString getExpression() const;
    QStringList getHistory() const;

    Q_INVOKABLE void digitPressed(const QString &digit);
    Q_INVOKABLE void operatorPressed(const QString &op);
    Q_INVOKABLE void equalsPressed();
    Q_INVOKABLE void clearPressed();
    Q_INVOKABLE void backspacePressed();
    Q_INVOKABLE void toggleSignPressed();
    Q_INVOKABLE void percentPressed();
    Q_INVOKABLE void clearHistory();

signals:
    void displayChanged();
    void expressionChanged();
    void historyChanged();

private:
    void setDisplay(const QString &text);
    void setExpression(const QString &text);
    double calculate(double left, double right, const QString &op);

    QString m_display;
    QString m_expression;
    QStringList m_history;
    double m_firstOperand;
    QString m_pendingOperator;
    bool m_waitingForOperand;
};

#endif