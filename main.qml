import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Controller 1.0
import Model 1.0
import QtQuick.Layouts 1.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    I2C_Model {
        id: i2c_model
        onU_valuesChanged: {
            let str = ''
            for (let i = 0; i < i2c_model.u_values.length; ++i) {
                str += i2c_model.u_values[i]
                if (i !== i2c_model.u_values.length - 1) str += '\n'
            }
            textArea.text = str
            scrollView.toBottom()
        }
    }

    I2C_Controller {
        id: i2c_controller
        target: i2c_model
    }

    Column {
        Button {
            text: 'start'
            onClicked: {
                i2c_controller.startReadU()
            }
        }
        Button {
            text: 'stop'
            onClicked: {
                i2c_controller.stopReadU()
            }
        }
        Button {
            text: 'toBottom'
            onClicked: {
                scrollView.toBottom()
            }
        }
    }

    ScrollView {
        id: scrollView
        width: 200
        height: 100
        x: 100
        y: 100
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        function toBottom() {
            ScrollBar.vertical.position = 1.0 - ScrollBar.vertical.size
        }

        TextArea {
            id: textArea
        }
    }


    Rectangle {
        width: 100
        height: 200
        x: 640
        y: 100
        color: 'green'
    }
}
