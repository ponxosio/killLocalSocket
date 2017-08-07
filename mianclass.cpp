#include "mianclass.h"

MainClass::MainClass(QObject *parent) :
    QObject(parent)
{
    // get the instance of the main application
    app = QCoreApplication::instance();
    // setup everything here
    // create any global objects
    // setup debug and warning mode

    this->socket = new QLocalSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(signalSended()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(displayError(QLocalSocket::LocalSocketError)));

    this->blockSize = 0;
}

// 10ms after the application starts this method will run
// all QT messaging is running at this point so threads, signals and slots
// will all work as expected.
void MainClass::run()
{
    std::cout << "killing app...";
    socket->connectToServer("finishExecution");
    std::cout << "connection done!...";
}

// call this routine to quit the application
void MainClass::quit()
{
    // you can do some cleanup here
    // then do emit finished to signal CoreApplication to quit
    emit finished();
}

// shortly after quit is called the CoreApplication will signal this routine
// this is a good place to delete any objects that were created in the
// constructor and/or to stop any threads
void MainClass::aboutToQuitApp()
{
    socket->abort();
    delete socket;
}

void MainClass::signalSended() {
    std::cout << "sending data...";

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        // Relies on the fact that QDataStream format streams a quint32 into sizeof(quint32) bytes
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize || in.atEnd())
        return;

    std::cout << "waiting response...";

    QString message;
    in >> message;

    std::cout << message.toStdString() << std::endl;
    // you must call quit when complete or the program will stay in the
    // messaging loop
    quit();
}

void MainClass::displayError(QLocalSocket::LocalSocketError socketError)
{
    std::string str;
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        str = "The host was not found. Please check the host name and port settings.";
        break;
    case QLocalSocket::ConnectionRefusedError:
        str = "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        str = socket->errorString().toStdString();
    }
    std::cout << "ERROR: " << str;
    // you must call quit when complete or the program will stay in the
    // messaging loop
    quit();
}

