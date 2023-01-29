#ifndef I2C_CONTROLLER_H
#define I2C_CONTROLLER_H

#include "i2c_model.h"
#include "i2c_reader.h"

#include <QObject>
#include <QThread>

class I2C_Controller : public QObject
{
    Q_PROPERTY(I2C_Model* target READ target WRITE setTarget NOTIFY targetChanged)
    Q_OBJECT

public:
    explicit I2C_Controller(QObject *parent = nullptr) : QObject{parent} {
        m_i2c_reader = new I2C_Reader();

        QString error = m_i2c_reader->init();

        if (!error.isEmpty()) {
            qDebug() << "init error" << error;
        }

        m_i2c_reader->moveToThread(&m_thread_i2c_reader);
        connect(this, &I2C_Controller::startReadU, m_i2c_reader, &I2C_Reader::startReadU );
        connect(this, &I2C_Controller::stopReadU, this, [this](){ m_i2c_reader->stopReadU();} );
        connect(m_i2c_reader, &I2C_Reader::resultU, this, [this](unsigned char val) {
            QList<unsigned char> values = m_target->u_values();
            values.append(val);
            m_target->setU_values(values);
//            m_target->appendU_value(val);
        });
        m_thread_i2c_reader.start();
    }
    ~I2C_Controller() {
//        m_i2c_reader->stopReadU();
        m_thread_i2c_reader.quit();
        m_thread_i2c_reader.wait();
    }

    I2C_Model* target() {
        return m_target;
    }
    void setTarget(I2C_Model* target) {
        if (m_target == target) {
            return;
        }
        m_target = target;
        emit targetChanged();
    }

signals:
    void targetChanged();
    void startReadU();
    void stopReadU();

private:
    I2C_Model* m_target;
    I2C_Reader* m_i2c_reader;
    QThread m_thread_i2c_reader;
};

#endif // I2C_CONTROLLER_H
