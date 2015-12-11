/****************************************************************************
**
** This code was generated by a code generator based on imatix/gsl
** Any changes in this code will be lost.
**
****************************************************************************/
#ifndef HAL_REMOTE_COMPONENT_BASE_H
#define HAL_REMOTE_COMPONENT_BASE_H
#include <QObject>
#include <QStateMachine>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include <google/protobuf/text_format.h>
#include "machinetalkrpcclient.h"
#include "machinetalksubscribe.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class HalRemoteComponentBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString halrcmdUri READ halrcmdUri WRITE setHalrcmdUri NOTIFY halrcmdUriChanged)
    Q_PROPERTY(QString halrcompUri READ halrcompUri WRITE setHalrcompUri NOTIFY halrcompUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State)

public:
    explicit HalRemoteComponentBase(QObject *parent = 0);
    ~HalRemoteComponentBase();

    enum State {
        Down = 0,
        Trying = 1,
        Binding = 2,
        Syncing = 3,
        Synced = 4,
        Error = 5,
    };

    QString halrcmdUri() const
    {
        return m_halrcmdChannel->socketUri();
    }

    QString halrcompUri() const
    {
        return m_halrcompChannel->socketUri();
    }

    QString debugName() const
    {
        return m_debugName;
    }

    State state() const
    {
        return m_state;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setHalrcmdUri(QString uri)
    {
        m_halrcmdChannel->setSocketUri(uri);
    }

    void setHalrcompUri(QString uri)
    {
        m_halrcompChannel->setSocketUri(uri);
    }

    void setDebugName(QString debugName)
    {
        if (m_debugName == debugName)
            return;

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready)
            return;

        m_ready = ready;
        emit readyChanged(ready);

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }

    void sendHalrcmdMessage(pb::ContainerType type, pb::Container *tx);
    void sendHalrcompBind(pb::Container *tx);
    void sendHalrcompSet(pb::Container *tx);
    void addHalrcompTopic(const QString &name);
    void removeHalrcompTopic(const QString &name);
    void clearHalrcompTopics();

private:
    bool m_ready;
    QString m_socketUri;
    QString m_debugName;

    MachinetalkRpcClient *m_halrcmdChannel;
    QSet<QString> m_halrcompTopics;  // the topics we are interested in
    MachinetalkSubscribe *m_halrcompChannel;

    State         m_state;
    QStateMachine *m_fsm;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    pb::Container m_halrcmdRx;
    pb::Container m_halrcmdTx;
    pb::Container m_halrcompRx;

    void start();
    void stop();

private slots:

    void startHalrcmdChannel();
    void stopHalrcmdChannel();
    void halrcmdChannelStateChanged(MachinetalkRpcClient::State state);
    void halrcmdChannelMessageReceived(pb::Container *rx);

    void startHalrcompChannel();
    void stopHalrcompChannel();
    void halrcompChannelStateChanged(MachinetalkSubscribe::State state);
    void halrcompChannelMessageReceived(const QByteArray &topic, pb::Container *rx);

    void fsmDownEntered();
    void fsmTryingEntered();
    void fsmBindingEntered();
    void fsmSyncingEntered();
    void fsmSyncedEntered();
    void fsmErrorEntered();
    virtual void bind() { qWarning() << "SLOT bind unimplemented"; }
    virtual void addPins() { qWarning() << "SLOT add pins unimplemented"; }
    virtual void removePins() { qWarning() << "SLOT remove pins unimplemented"; }
    virtual void unsyncPins() { qWarning() << "SLOT unsync pins unimplemented"; }
    virtual void synced() { qWarning() << "SLOT synced unimplemented"; }

signals:

    void halrcmdUriChanged(QString uri);
    void halrcompUriChanged(QString uri);
    void halrcmdMessageReceived(pb::Container *rx);
    void halrcompMessageReceived(QByteArray topic, pb::Container *rx);
    void debugNameChanged(QString debugName);
    void stateChanged(HalRemoteComponentBase::State state);
    void errorStringChanged(QString errorString);
    void readyChanged(bool ready);
    // fsm
    void fsmConnect();
    void fsmHalrcmdUp();
    void fsmDisconnect();
    void fsmBindConfirmed();
    void fsmBindRejected();
    void fsmHalrcmdTrying();
    void fsmHalrcompUp();
    void fsmHalrcompTrying();
};

#endif //HAL_REMOTE_COMPONENT_BASE_H
