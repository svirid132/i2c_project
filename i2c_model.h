#ifndef I2C_MODEL_H
#define I2C_MODEL_H

#include <QObject>

class I2C_Model : public QObject
{
    Q_PROPERTY(QList<unsigned char> u_values READ u_values WRITE setU_values NOTIFY u_valuesChanged)

    Q_OBJECT
public:
    explicit I2C_Model(QObject *parent = nullptr);
    QList<unsigned char> u_values() {
        return m_u_values;
    }
    void setU_values(const QList<unsigned char>& u_values) {
        if (m_u_values == u_values) {
            return;
        }
        m_u_values = u_values;
        emit u_valuesChanged();
    }
    void appendU_value(unsigned char value) {
        m_u_values.append(value);
    }

signals:
    void u_valuesChanged();

private:
    QList<unsigned char> m_u_values;
};

#endif // I2C_MODEL_H
