#ifndef I2C_READER_H
#define I2C_READER_H

#include <QDebug>
#include <QObject>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


#define DEV_I2C "/dev/i2c-1"
#define SLAVE_ADDR 0x54 /* EMC1414 I2C slave address */

class I2C_Reader : public QObject
{
    Q_OBJECT
public:
    explicit I2C_Reader(QObject *parent = nullptr);

public:
    QString init() {

        // open device node
        m_fd = open(DEV_I2C, O_RDWR);
        if (m_fd < 0) {
            return QString("ERROR open %1 ret=%2\n").arg(DEV_I2C).arg(m_fd);
        }

        if (ioctl(m_fd, I2C_SLAVE, SLAVE_ADDR) < 0) {
            return QString("ERROR ioctl() set slave address\n");
        }

        return QString();
    }
    void stopReadU() {
        m_stop = true;
    }

public slots:
    void setDefault(unsigned char val = 0XFF) {

    }
    void startReadU() {
        qDebug() << "startReadU";
        int ret;
        unsigned char buf[2];
        m_stop = false;
        buf[0] = 0x03; // Internal Diode High Byte register address
        ret = write(m_fd, &buf[0], 1);
        if (ret != 1) {
            qDebug() << QString("ERROR write() register address\n");
            return;
        }

        while (1) {

            // Read temperature
            // Read data
            buf[1] = 0; // clear receive buffer
            ret = read(m_fd, &buf[1], 1);
            if (ret != 1) {
                qDebug() << QString("ERROR read() data\n");
                return;
            }

            {
                if (m_stop == true) {
                    return;
                }
            }

            emit resultU(buf[1]);
            sleep(1);
        }
    }

signals:
    void resultU(unsigned char val);

private:
    int m_fd = -1;
    std::atomic<bool> m_stop = true;
};

#endif // I2C_READER_H
