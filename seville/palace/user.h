#ifndef SEVILLE_PALACE_USER_H_
#define SEVILLE_PALACE_USER_H_

#include <QImage>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "seville/base/types.h"
#include "seville/palace/prop.h"

namespace seville
{
   namespace palace
   {
      class User : QObject
      {
      Q_OBJECT

      public:
         static const i32 kNumPropCells = 9;

         User(QObject* parentPtr = nullptr);
         User(const User& user, QObject* parentObjectPtr = nullptr);

         virtual ~User(void);

         User& operator=(const User& value);

         inline i32 color(void) const {
            return my_color;
         }

         inline void setColor(i32 value) {
            my_color = value;
         }

         inline i32 face(void) const {
            return my_face;
         }

         inline void setFace(i32 value) {
            my_face = value;
         }

         inline i32 x(void) const {
            return my_x;
         }

         inline void setX(i32 value) {
            my_x = value;
         }

         inline i32 y(void) const {
            return my_y;
         }

         inline void setY(i32 value) {
            my_y = value;
         }

         inline QString username(void) const {
            return my_username;
         }

         inline void setUsername(const QString& value) {
            my_username = value;
         }

         inline QString wizPass(void) const {
            return my_wizardPassword;
         }

         inline void setWizPass(const QString& value) {
            my_wizardPassword = value;
         }

         inline i32 id(void) const {
            return my_id;
         }

         inline void setId(i32 value) {
            my_id = value;
         }

         inline u16 flags(void) {
            return my_flags;
         }

         inline void setFlags(u16 value) {
            my_flags = value;
         }

         inline u32 idCrc(void) const {
            return my_idCrc;
         }

         inline void setIdCrc(u32 value) {
            my_idCrc = value;
         }

         inline u32 idCounter(void) const {
            return my_idCounter;
         }

         inline void setIdCounter(u32 value) {
            my_idCounter = value;
         }

         inline bool idChanged(void) const {
            return my_idChangedFlag;
         }

         inline void setIdChanged(bool value) {
            my_idChangedFlag = value;
         }

         inline u32 regCrc(void) const {
            return my_regCrc;
         }

         inline void setRegCrc(u32 value) {
            my_regCrc = value;
         }

         inline u32 regCounter(void) const {
            return my_regCounter;
         }

         inline void setRegCounter(u32 value) {
            my_regCounter = value;
         }

         inline i16 roomId(void) const {
            return my_roomId;
         }

         inline void setRoomId(i16 id) {
            my_roomId = id;
         }

         inline std::vector<Prop>* propsPtr(void) {
            return &my_props;
         }

         inline i16 propNum(void) {
            return my_propNum;
         }

         inline void setPropNum(i16 value) {
            my_propNum = value;
         }

         inline void clear(void) {
            do_clear();
         }

         inline i32 hasHeadProp(void) {
            return do_hasHeadProp();
         }

//         inline auto propsPtr(void) -> u32* {
//            return &my_props;
//         }

         // inline void fetchWebPropAsync(const QString& propServerUri) {
         //    do_fetchWebPropAsync(propServerUri);
         // }

      public slots:
         // void on_webPropDidFetchAsync(QNetworkReply* replyPtr);

      private:
         i32 my_id;
         u16 my_flags;
         QString my_username;
         QString my_wizardPassword;
         i32 my_face;
         i32 my_x;
         i32 my_y;
         i32 my_color;
         bool my_idChangedFlag;
         u32 my_idCounter;
         u32 my_idCrc;
         u32 my_regCounter;
         u32 my_regCrc;
         i16 my_roomId;
         std::vector<Prop> my_props;
         i16 my_propNum;
         i32 my_headPropFlag;

         //u32 my_props[kNumPropCells];

         i32 do_hasHeadProp(void);
         // void do_fetchWebPropAsync(const QString& propServerUri);

         void do_clear(void);
         void do_assign(const User& user);

         void do_deinit(void);
         void do_init(void);

         void do_init(const User& user) {
            do_assign(user);
         }
      };
   }
}

#endif   // SEVILLE_PALACE_USER_H
