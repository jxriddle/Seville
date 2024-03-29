#include "user.h"

namespace seville
{
    namespace palace
    {
        User& User::operator=(const User& value)
        {
            do_assign(value);
            return *this;
        }

        void User::do_clear(void)
        {
            my_id = 0;
            my_flags = 0;
            my_color = 1;
            my_face = 1;
            my_x = 0;
            my_y = 0;
            my_idChangedFlag = false;
            my_idCounter = 0xf5dc385e;
            my_idCrc = 0xc144c580;
            my_regCounter = 0xcf07309c;
            my_regCrc = 0x5905f923;
            my_username = "";
            my_wizardPassword = "";
            my_roomId = 0;
            my_headPropFlag = 0;

            my_props.clear();
        }

        void User::do_assign(const User& user)
        {
            my_id = user.my_id;
            my_flags = user.my_flags;
            my_color = user.my_color;
            my_face = user.my_face;
            my_x = user.my_x;
            my_y = user.my_y;
            my_idChangedFlag = user.my_idChangedFlag;
            my_idCounter = user.my_idCounter;
            my_idCrc = user.my_idCrc;
            my_regCounter = user.my_regCounter;
            my_regCrc = user.my_regCrc;
            my_username = user.my_username;
            my_wizardPassword = user.my_wizardPassword;
            my_roomId = user.my_roomId;

            my_props.clear();
            for (auto prop: user.my_props) {
                my_props.push_back(prop);
            }
        }

        void User::do_init(void)
        {
        }

        void User::do_deinit(void)
        {
        }

//      void User::do_fetchWebPropAsync(const QString& propServerUri)
//      {
//         auto request = QNetworkRequest(propServerUri);
//         request.setRawHeader("Content-Type", "application/json");
//         auto data = QString("{\"props\"[");

//         data.append(QString("{\"id\": %1}").arg(my_id));

//         data.append("]}");
//         // auto postReply = my_webPropFetcher.post(request, data);
//         // auto json = postReply;
//      }

//      void User::on_webPropDidFetchAsync(QNetworkReply* replyPtr)
//      {
//         if (replyPtr->error()) {
//            // my_logger.appendErrorMessage(
//                     //QString("Background failed to load."));
//            //          QString("Prop load error: %1")
//            //          .arg(replyPtr->errorString()));
//         }
//         else {
//            auto imageByteArray = replyPtr->readAll();
//            auto const imageByteArrayPtr =
//                  reinterpret_cast<QByteArray*>(&imageByteArray);
//            (void)imageByteArrayPtr;

//            // my_room.setBackgroundImageByteArray(*imageByteArrayPtr);

//            // emit backgroundImageDidLoadEvent();
//            // emit viewNeedsUpdatingEvent();
//         }

//         replyPtr->deleteLater();
//      }

        User::~User(void)
        {
            do_deinit();
        }

        User::User(QObject* parentPtr)
         : QObject(parentPtr)
        {
            do_init();
            do_clear();
        }

        User::User(const User& user, QObject* parentPtr)
            : QObject(parentPtr)
        {
            do_init(user);
        }
    }
}
